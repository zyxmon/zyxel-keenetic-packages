/*
  Mode switching tool for controlling flip flop (multiple device) USB gear
  Version 0.9.7beta, 2009/03/14

  Copyright (C) 2007, 2008, 2009 Josua Dietze (mail to "usb_admin" at the
  domain from the README; please do not post the complete address to The Net!
  Or write a personal message through the forum to "Josh")

  Command line parsing, decent usage/config output/handling, bugfixes and advanced
  options added by:
    Joakim Wennergren (jokedst) (gmail.com)

  TargetClass parameter implementation to support new Option devices/firmware:
    Paul Hardwick (http://www.pharscape.org)

  Created with initial help from:
    "usbsnoop2libusb.pl" by Timo Lindfors (http://iki.fi/lindi/usb/usbsnoop2libusb.pl)

  Config file parsing stuff borrowed from:
    Guillaume Dargaud (http://www.gdargaud.net/Hack/SourceCode.html)

  Hexstr2bin function borrowed from:
    Jouni Malinen (http://hostap.epitest.fi/wpa_supplicant, from "common.c")

  Code, fixes and ideas from:
    Aki Makkonen
    Denis Sutter 
    Lucas Benedicic 
    Roman Laube 
	Vincent Teoh
	Tommy Cheng
	Daniel Cooper

  More contributors are listed in the config file.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details:

  http://www.gnu.org/licenses/gpl.txt

*/

/* Recommended tab size: 4 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <getopt.h>
#include <usb.h>
#include "old_usb_modeswitch.h"

#define LINE_DIM 1024
#define BUF_SIZE 4096

int write_bulk(int endpoint, char *message, int length);
int read_bulk(int endpoint, char *buffer, int length);

char *TempPP=NULL;

struct usb_dev_handle *devh;

int DefaultVendor=0, DefaultProduct=0, TargetVendor=0, TargetProduct=0, TargetClass=0;
int MessageEndpoint=0, ResponseEndpoint=-1;
int targetDeviceCount=0;
int ret;
char DetachStorageOnly=0, HuaweiMode=0, SierraMode=0, SonyMode=0;
char verbose=0, show_progress=1, ResetUSB=0, CheckSuccess=0, config_read=0;
char MessageContent[LINE_DIM];
char ByteString[LINE_DIM/2];
char buf[BUF_SIZE];

// Settable Interface and Configuration (for debugging mostly) (jmw)
int Interface = 0, Configuration = -1, AltSetting = -1;

static struct option long_options[] =
{
	{"help",				no_argument, 0, 'h'},
	{"DefaultVendor",		required_argument, 0, 'v'},
	{"DefaultProduct",		required_argument, 0, 'p'},
	{"TargetVendor",		required_argument, 0, 'V'},
	{"TargetProduct",		required_argument, 0, 'P'},
	{"TargetClass",			required_argument, 0, 'C'},
	{"MessageEndpoint",		required_argument, 0, 'm'},
	{"MessageContent",		required_argument, 0, 'M'},
	{"ResponseEndpoint",	required_argument, 0, 'r'},
	{"DetachStorageOnly",	required_argument, 0, 'd'},
	{"HuaweiMode",			required_argument, 0, 'H'},
	{"SierraMode",			required_argument, 0, 'S'},
	{"SonyMode",			required_argument, 0, 'O'},
	{"NeedResponse",		required_argument, 0, 'n'},
	{"ResetUSB",			required_argument, 0, 'R'},
	{"config",				required_argument, 0, 'c'},
	{"verbose",				no_argument, 0, 'W'},
	{"quiet",				no_argument, 0, 'Q'},
	{"success",				required_argument, 0, 's'},
	{"Interface",			required_argument, 0, 'i'},
	{"Configuration",		required_argument, 0, 'u'},
	{"AltSetting",			required_argument, 0, 'a'},
	{0, 0, 0, 0}
};

void readConfigFile(const char *configFilename)
{
	if(verbose) printf("Reading config file: %s\n", configFilename);
	ParseParamHex(configFilename, TargetVendor);
	ParseParamHex(configFilename, TargetProduct);
	ParseParamHex(configFilename, TargetClass);
	ParseParamHex(configFilename, DefaultVendor);
	ParseParamHex(configFilename, DefaultProduct);
	ParseParamBool(configFilename, DetachStorageOnly);
	ParseParamBool(configFilename, HuaweiMode);
	ParseParamBool(configFilename, SierraMode);
	ParseParamBool(configFilename, SonyMode);
	ParseParamHex(configFilename, MessageEndpoint);
	ParseParamString(configFilename, MessageContent);
//    ParseParamHex(configFilename, NeedResponse);
	ParseParamHex(configFilename, ResponseEndpoint);
	ParseParamHex(configFilename, ResetUSB);
	ParseParamHex(configFilename, CheckSuccess);
	ParseParamHex(configFilename, Interface);
	ParseParamHex(configFilename, Configuration);
	ParseParamHex(configFilename, AltSetting);
	config_read = 1;
}

void printConfig()
{
	printf ("DefaultVendor=0x%x\n",		DefaultVendor);
	printf ("DefaultProduct=0x%x\n",	DefaultProduct);
	printf ("TargetVendor=0x%x\n",		TargetVendor);
	printf ("TargetProduct=0x%x\n",		TargetProduct);
	printf ("TargetClass=0x%x\n",		TargetClass);
	printf ("DetachStorageOnly=%i\n",	(int)DetachStorageOnly);
	printf ("HuaweiMode=%i\n",			(int)HuaweiMode);
	printf ("SierraMode=%i\n",			(int)SierraMode);
	printf ("SonyMode=%i\n",			(int)SonyMode);
	printf ("MessageEndpoint=0x%x\n",	MessageEndpoint);
	printf ("MessageContent=\"%s\"\n",	MessageContent);
//	printf ("NeedResponse=%i\n",		(int)NeedResponse);
	if ( ResponseEndpoint > -1 )
		printf ("ResponseEndpoint=0x%x\n",	ResponseEndpoint);
	printf ("Interface=0x%x\n",			Interface);
	if ( Configuration > -1 )
		printf ("Configuration=0x%x\n",	Configuration);
	if ( AltSetting > -1 )
		printf ("AltSetting=0x%x\n",	AltSetting);
	if ( CheckSuccess )
		printf ("\nSuccess check enabled, settle time %d seconds\n", CheckSuccess);
	else
		printf ("\nSuccess check disabled\n");
	printf ("\n");
}

int readArguments(int argc, char **argv)
{
	int c, option_index = 0, count=0;
	if(argc==1) return 0;

	while (1)
	{
		c = getopt_long (argc, argv, "hWQs:v:p:V:P:C:m:M:r:d:H:S:O:n:c:R:i:u:a:",
						long_options, &option_index);
	
		/* Detect the end of the options. */
		if (c == -1)
			break;
		count++;
		switch (c)
		{
			case 'R': ResetUSB = strtol(optarg, NULL, 16); break;
			case 'v': DefaultVendor = strtol(optarg, NULL, 16); break;
			case 'p': DefaultProduct = strtol(optarg, NULL, 16); break;
			case 'V': TargetVendor = strtol(optarg, NULL, 16); break;
			case 'P': TargetProduct = strtol(optarg, NULL, 16); break;
			case 'C': TargetClass = strtol(optarg, NULL, 16); break;
			case 'm': MessageEndpoint = strtol(optarg, NULL, 16); break;
			case 'M': strcpy(MessageContent, optarg); break;
			case 'r': ResponseEndpoint = strtol(optarg, NULL, 16); break;
			case 'd': DetachStorageOnly = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
			case 'H': HuaweiMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
			case 'S': SierraMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
			case 'O': SonyMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
			case 'n': break;
			case 'c': readConfigFile(optarg); break;
			case 'W': verbose = 1; show_progress = 1; count--; break;
			case 'Q': show_progress = 0; verbose = 0; count--; break;
			case 's': CheckSuccess= strtol(optarg, NULL, 16); count--; break;

			case 'i': Interface = strtol(optarg, NULL, 16); break;
			case 'u': Configuration = strtol(optarg, NULL, 16); break;
			case 'a': AltSetting = strtol(optarg, NULL, 16); break;
	
			case 'h':
				printf ("Usage: usb_modeswitch [-hvpVPmMrdHn] [-c filename]\n\n");
				printf (" -h, --help                    this help\n");
				printf (" -v, --DefaultVendor [nr]      set vendor number to look for\n");
				printf (" -p, --DefaultProduct [nr]     set model number to look for\n");
				printf (" -V, --TargetVendor [nr]       target vendor; needed for success check\n");
				printf (" -P, --TargetProduct [nr]      target model; needed for success check\n");
				printf (" -C, --TargetClass [nr]        target device class\n");
				printf (" -m, --MessageEndpoint [nr]    where to direct command sequence\n");
				printf (" -M, --MessageContent [nr]     command sequence to send\n");
//				printf (" -n, --NeedResponse [1|0]      whether to try to read a response - OBSOLETE\n");
				printf (" -r, --ResponseEndpoint [nr]   if given, read response from there\n");
				printf (" -d, --DetachStorageOnly [1|0] whether to just detach the storage driver\n");
				printf (" -H, --HuaweiMode [1|0]        whether to just apply a special sequence\n");
				printf (" -S, --SierraMode [1|0]        whether to just apply a special sequence\n");
				printf (" -O, --SonyMode [1|0]          whether to just apply a special sequence\n");
				printf (" -R, --ResetUSB [1|0]          whether to reset the device in the end\n");
				printf (" -c, --config [filename]       load different config file\n");
				printf (" -Q, --quiet                   don't show progress or error messages\n");
				printf (" -W, --verbose                 print all settings before running\n");
				printf (" -s, --success [nr]            check switching result after [nr] secs\n\n");
				printf (" -i, --Interface               select initial USB interface (default 0)\n");
				printf (" -u, --Configuration           select USB configuration\n");
				printf (" -a, --AltSetting              select alternative USB interface setting\n\n");
				exit(0);
				break;
		
			default: //Unsupported - error message has already been printed
				printf ("\n");
				exit(1);
		}
	}

	return count;
}


int main(int argc, char **argv) {
	int numDefaults = 0, specialMode = 0;

	struct usb_device *dev;

	printf("\n * old_usb_modeswitch: tool for controlling \"flip flop\" mode USB devices\n");
   	printf(" * Version 0.9.7beta (C) Josua Dietze 2009\n");
   	printf(" * Works with libusb 0.1.12 and probably other versions\n\n");


	/*
	 * Parameter parsing, USB preparation/diagnosis, plausibility checks
	 */

	// Check command arguments, use params instead of config file when given
	switch (readArguments(argc, argv)) {
		case 0:						// no argument or -W, -q or -s
			readConfigFile("/etc/usb_modeswitch.conf");
			break;
		default:					// one or more arguments except -W, -q or -s 
			if (!config_read)		// if arguments contain -c, the config file was already processed
				if(verbose) printf("Taking all parameters from the command line\n\n");
	}

	if(verbose)
		printConfig();

	// libusb initialization
	usb_init();

	if (verbose)
		usb_set_debug(15);

	usb_find_busses();
	usb_find_devices();

	if (verbose)
		printf("\n");

	// Plausibility checks. The default IDs are mandatory
	if (!(DefaultVendor && DefaultProduct)) {
		if (show_progress) printf("No default vendor/product ID given. Aborting.\n\n");
		exit(1);
	}
	if (strlen(MessageContent)) {
		if (strlen(MessageContent) % 2 != 0) {
			if (show_progress) fprintf(stderr, "Error: MessageContent hex string has uneven length. Aborting.\n\n");
			exit(1);
		}
		if (!MessageEndpoint) {
			if (show_progress) fprintf(stderr, "Error: no MessageEndpoint given. Aborting.\n\n");
			exit(1);
		}
		if ( hexstr2bin(MessageContent, ByteString, strlen(MessageContent)/2) == -1) {
			if (show_progress) fprintf(stderr, "Error: MessageContent %s\n is not a hex string. Aborting.\n\n", MessageContent);
			exit(1);
		}
	}
	if (show_progress)
		if (CheckSuccess && !(TargetVendor && TargetProduct) && !TargetClass)
			printf("Note: target parameter missing; success check limited\n");

	// Count existing target devices (remember for success check)
	if ((TargetVendor && TargetProduct) || TargetClass) {
		if (show_progress) printf("Looking for target devices ...\n");
		search_devices(&targetDeviceCount, TargetVendor, TargetProduct, TargetClass);
		if (targetDeviceCount) {
			if (show_progress) printf(" Found devices in target mode or class (%d)\n", targetDeviceCount);
		} else {
			if (show_progress) printf(" No devices in target mode or class found\n");
		}
	}

	// Count default devices, return the last one found
	if (show_progress) printf("Looking for default devices ...\n");
	dev = search_devices(&numDefaults, DefaultVendor, DefaultProduct, TargetClass);
	if (numDefaults) {
		if (show_progress) printf(" Found default devices (%d)\n", numDefaults);
		if (TargetClass && !(TargetVendor && TargetProduct)) {
			if ( dev != NULL ) {
				if (show_progress) printf(" Found a default device NOT in target class mode\n");
			} else {
				if (show_progress) printf(" All devices in target class mode. Nothing to do. Bye!\n\n");
				exit(0);
			}
		}
	}
	if (dev != NULL) {
		if (show_progress) printf("Prepare switching, accessing device %03d on bus %03d ...\n", dev->devnum, strtol(dev->bus->dirname,NULL,10));
		devh = usb_open(dev);
	} else {
		if (show_progress) printf(" No default device found. Is it connected? Bye!\n\n");
		exit(0);
	}

	// Interface different from default, test it
	if (Interface > 0) {
		ret = usb_claim_interface(devh, Interface);
		if (ret != 0) {
			if (show_progress) printf("Could not claim interface %d (error %d). Aborting.\n\n", Interface, ret);
			exit(1);
		}
	}

	// Some scenarios are exclusive, so check for unwanted combinations
	specialMode = DetachStorageOnly + HuaweiMode + SierraMode + SonyMode;
	if ( specialMode > 1 ) {
		if (show_progress) printf("Invalid mode combination. Check your configuration. Aborting.\n\n");
		exit(1);
	}

	/*
	 * The switching actions
	 */

	if (DetachStorageOnly) {
		if (show_progress) printf("Only detaching storage driver for switching ...\n");
		ret = detachDriver();
		if (ret == 2) {
			if (show_progress) printf(" You may want to remove the storage driver manually. Bye\n\n");
//			exit(0);
		}
	}

	if (HuaweiMode) {
		switchHuaweiMode();
	}
	if (SierraMode) {
		switchSierraMode();
	}
	if (SonyMode) {
		switchSonyMode();
	}

	if (strlen(MessageContent) && MessageEndpoint) {
		if (specialMode == 0) {
			detachDriver();
			ret = switchSendMessage();
			if (ret && (ResponseEndpoint != -1))
				switchReadResponse();
		} else {
			if (show_progress) printf("Note: ignoring MessageContent. Can't combine with special mode\n");
		}
	}

	if (Configuration != -1) {
		switchConfiguration ();
	}

	if (AltSetting != -1) {
		switchAltSetting();
	}

	if (ResetUSB) {
		resetUSB();
	}

	if (devh)
		usb_close(devh);

	if (CheckSuccess) {
		signal(SIGTERM, release_usb_device);
		if (checkSuccess(dev))
			exit(0);
		else
			exit(1);
	} else {
		if (show_progress) printf("-> Run lsusb to note any changes. Bye\n\n");
		exit(0);
	}
}


int resetUSB () {
	int success;
	int bpoint = 0;

	if (show_progress) printf("Resetting usb device ");

	sleep( 1 );
	do {
		success = usb_reset(devh);
		if (show_progress) printf(".");
		bpoint++;
		if (bpoint > 100)
			success = 1;
	} while (success < 0);

	if ( success ) {
		if (show_progress) printf("\n Reset failed. Can be ignored if device switched OK.\n");
	} else {
		if (show_progress) printf("\n OK, device was reset\n");
	}
}

int switchSendMessage () {
	int message_length;

	if (show_progress) printf("Setting up communication with interface %d ...\n", Interface);
	ret = usb_claim_interface(devh, Interface);
	if (ret != 0) {
		if (show_progress) printf(" Could not claim interface (error %d). Skipping message sending\n", ret);
		return 0;
	}
	if (show_progress) printf("Trying to send the message to endpoint 0x%02x ...\n", MessageEndpoint);
	message_length = strlen(MessageContent) / 2;
	usb_clear_halt(devh, MessageEndpoint);
	write_bulk(MessageEndpoint, ByteString, message_length);

	usb_clear_halt(devh, MessageEndpoint);
	usb_release_interface(devh, Interface);
}

int switchReadResponse () {
	if (show_progress) printf("Reading the response to the message ...\n");
	return read_bulk(ResponseEndpoint, ByteString, LINE_DIM/2);
}

int switchConfiguration () {

	if (show_progress) printf("Changing configuration to %i ...\n", Configuration);
	ret = usb_set_configuration(devh, Configuration);
	if (ret == 0 ) {
		if (show_progress) printf(" OK, configuration set\n");
		return 1;
	} else {
		if (show_progress) printf(" Setting the configuration returned error %d. Trying to continue\n", ret);
		return 0;
	}
}

int switchAltSetting () {

	if (show_progress) printf("Changing to alt setting %i ...\n", AltSetting);
	ret = usb_claim_interface(devh, Interface);
	ret = usb_set_altinterface(devh, AltSetting);
	usb_release_interface(devh, Interface);
	if (ret != 0) {
		if (show_progress) fprintf(stderr, " Changing to alt setting returned error %d. Trying to continue\n", ret);
		return 0;
	} else {
		if (show_progress) printf(" OK, changed to alt setting\n");
		return 1;
	}
}

int switchHuaweiMode () {

	if (show_progress) printf("Sending Huawei control message ...\n");
	ret = usb_control_msg(devh, USB_TYPE_STANDARD + USB_RECIP_DEVICE, USB_REQ_SET_FEATURE, 00000001, 0, buf, 0, 1000);
	if (ret != 0) {
		if (show_progress) fprintf(stderr, "Error: sending Huawei control message failed (error %d). Aborting.\n\n", ret);
		exit(1);
	} else
		if (show_progress) printf(" OK, Huawei control message sent\n");
}

int switchSierraMode () {

	if (show_progress) printf("Trying to send Sierra control message\n");
	ret = usb_control_msg(devh, 0x40, 0x0b, 00000001, 0, buf, 0, 1000);
	if (ret != 0) {
		if (show_progress) fprintf(stderr, "Error: sending Sierra control message failed (error %d). Aborting.\n\n", ret);
	    exit(1);
	} else
		if (show_progress) printf(" OK, Sierra control message sent\n");
}

int switchSonyMode () {

	if (show_progress) printf("Trying to send Sony control message\n");
	buf[0] = 0x5a;
	buf[1] = 0x11;
	buf[2] = 0x02;
	ret = usb_control_msg(devh, 0xc0, 0x11, 00000002, 0, buf, 3, 1000);
	if (ret < 0) {
		if (show_progress) fprintf(stderr, "Error: sending Sony control message failed (error %d). Aborting.\n\n", ret);
		exit(1);
	} else
		if (show_progress) printf(" OK, Sony control message sent\n");
}


// Detach driver either as the main action or as preparation for other modes
int detachDriver() {

#ifndef LIBUSB_HAS_GET_DRIVER_NP
	printf(" Cant't do driver detection and detaching on this platform.\n");
	return 2;
#endif

	if (show_progress) printf("Looking for active driver ...\n");
	ret = usb_get_driver_np(devh, Interface, buf, BUF_SIZE);
	if (ret != 0) {
		if (show_progress) printf(" No driver found. Either detached before or never attached\n");
		return 1;
	}
	if (show_progress) printf(" OK, driver found (\"%s\")\n", buf);
	if (DetachStorageOnly && strcmp(buf,"usb-storage")) {
		if (show_progress) printf(" Driver is not usb-storage, leaving it alone\n");
		return 1;
	}

#ifndef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
	if (show_progress) printf(" Can't do driver detaching on this platform\n");
	return 2;
#endif

	ret = usb_detach_kernel_driver_np(devh, Interface);
	if (ret == 0) {
		if (show_progress) printf(" OK, driver \"%s\" detached\n", buf);
	} else {
		if (show_progress) printf(" Driver \"%s\" detach failed with error %d. Trying to continue\n", buf, ret);
	}
	return 1;
}

int checkSuccess(struct usb_device *dev) {
	
	if (show_progress) printf("Checking for mode switch after %d seconds settling time ...\n", CheckSuccess);
	sleep(CheckSuccess);

	// Test if default device still can be accessed; positive result does
    // not necessarily mean failure
	devh = usb_open(dev);
	ret = usb_claim_interface(devh, Interface);
	if (ret < 0) {
		if (show_progress) printf(" Original device can't be accessed anymore. Good.\n");
	} else {
		if (show_progress) printf(" Original device can still be accessed. Hmm.\n");
		usb_release_interface(devh, Interface);
	}

	// Recount target devices (compare with previous count) if target data is given
	int newTargetCount;
	if ((TargetVendor && TargetProduct) || TargetClass) {
		usb_find_devices();
		search_devices(&newTargetCount, TargetVendor, TargetProduct, TargetClass);
		if (newTargetCount > targetDeviceCount) {
			if (show_progress) printf(" Found a new device in target mode or class\n\nMode switch was successful. Bye!\n\n");
			return 1;
		} else {
			if (show_progress) printf(" No new devices in target mode or class found\n\nMode switch seems to have failed. Bye!\n\n");
			return 0;
		}
	} else {
		if (show_progress) printf("For a better success check provide target IDs or class. Bye!\n\n");
		if (ret < 0)
			return 1;
		else
			return 0;
	}
}

int write_bulk(int endpoint, char *message, int length) {
	int ret;
	ret = usb_bulk_write(devh, endpoint, message, length, 1000);
	if (ret >= 0 ) {
		if (show_progress) printf(" OK, message successfully sent\n");
		return 1;
	} else {
		if (show_progress) printf(" Sending the message returned error %d. Trying to continue\n", ret);
		return 0;
	}
}

int read_bulk(int endpoint, char *buffer, int length) {
	int ret;
	ret = usb_bulk_read(devh, endpoint, buffer, length, 1000);
	if (ret >= 0 ) {
		if (show_progress) printf(" OK, response successfully read (%d bytes).\n", ret);
	} else {
		if (show_progress) printf(" Reading the response returned error %d, trying to continue\n", ret);
	}
	return ret;
}

void release_usb_device(int dummy) {
	int ret;
	if (show_progress) printf("Program cancelled by system. Bye!\n\n");
	usb_release_interface(devh, Interface);
	usb_close(devh);
	exit(0);
}


// iterates over busses and devices, counting the found and returning the last one of them

struct usb_device* search_devices( int *numFound, int vendor, int product, int targetClass) {
	struct usb_bus *bus;
	int devClass;
	struct usb_device* right_dev = NULL;
	
	if ( targetClass && !(vendor && product) ) {
		vendor = DefaultVendor;
		product = DefaultProduct;
	}
	*numFound = 0;
	for (bus = usb_get_busses(); bus; bus = bus->next) {
		struct usb_device *dev;
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product) {
				(*numFound)++;
				devClass = dev->descriptor.bDeviceClass;
				if (devClass == 0)
					devClass = dev->config[0].interface[0].altsetting[0].bInterfaceClass;
				if (devClass != targetClass || targetClass == 0)
					right_dev = dev;
			}
		}
	}
	return right_dev;
}


// the parameter parsing stuff

char* ReadParseParam(const char* FileName, char *VariableName) {
	static char Str[LINE_DIM];
	char *VarName, *Comment=NULL, *Equal=NULL;
	char *FirstQuote, *LastQuote, *P1, *P2;
	int Line=0, Len=0, Pos=0;
	FILE *file=fopen(FileName, "r");
	
	if (file==NULL) {
		if (show_progress) fprintf(stderr, "Error: Could not find file %s\n\n", FileName);
		exit(1);
	}

	while (fgets(Str, LINE_DIM-1, file) != NULL) {
		Line++;
		Len=strlen(Str);
		if (Len==0) goto Next;
		if (Str[Len-1]=='\n' or Str[Len-1]=='\r') Str[--Len]='\0';
		Equal = strchr (Str, '=');			// search for equal sign
		Pos = strcspn (Str, ";#!");			// search for comment
		Comment = (Pos==Len) ? NULL : Str+Pos;
		if (Equal==NULL or ( Comment!=NULL and Comment<=Equal)) goto Next;	// Only comment
		*Equal++ = '\0';
		if (Comment!=NULL) *Comment='\0';

		// String
		FirstQuote=strchr (Equal, '"');		// search for double quote char
		LastQuote=strrchr (Equal, '"');
		if (FirstQuote!=NULL) {
			if (LastQuote==NULL) {
				if (show_progress) fprintf(stderr, "Error reading parameter file %s line %d - Missing end quote.\n", FileName, Line);
				goto Next;
			}
			*FirstQuote=*LastQuote='\0';
			Equal=FirstQuote+1;
		}
		
		// removes leading/trailing spaces
		Pos=strspn (Str, " \t");
		if (Pos==strlen(Str)) {
			if (show_progress) fprintf(stderr, "Error reading parameter file %s line %d - Missing variable name.\n", FileName, Line);
			goto Next;		// No function name
		}
		while ((P1=strrchr(Str, ' '))!=NULL or (P2=strrchr(Str, '\t'))!=NULL)
			if (P1!=NULL) *P1='\0';
			else if (P2!=NULL) *P2='\0';
		VarName=Str+Pos;
		//while (strspn(VarName, " \t")==strlen(VarName)) VarName++;

		Pos=strspn (Equal, " \t");
		if (Pos==strlen(Equal)) {
			if (show_progress) fprintf(stderr, "Error reading parameter file %s line %d - Missing value.\n", FileName, Line);
			goto Next;		// No function name
		}
		Equal+=Pos;

//		printf("%s=%s\n", VarName, Equal);
		if (strcmp(VarName, VariableName)==0) {		// Found it
			fclose(file);
			return Equal;
		}
		Next:;
	}
	
	// not found
//	fprintf(stderr, "Error reading parameter file %s - Variable %s not found.", 
//				FileName, VariableName);
	fclose(file);
	return NULL;
}

int hex2num(char c)
{
	if (c >= '0' && c <= '9')
	return c - '0';
	if (c >= 'a' && c <= 'f')
	return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
	return c - 'A' + 10;
	return -1;
}


int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
	return -1;
	b = hex2num(*hex++);
	if (b < 0)
	return -1;
	return (a << 4) | b;
}

int hexstr2bin(const char *hex, char *buf, int len)
{
	int i;
	int a;
	const char *ipos = hex;
	char *opos = buf;
//    printf("Debug: hexstr2bin bytestring is ");

	for (i = 0; i < len; i++) {
	a = hex2byte(ipos);
//        printf("%02X", a);
	if (a < 0)
		return -1;
	*opos++ = a;
	ipos += 2;
	}
//    printf(" \n");
	return 0;
}


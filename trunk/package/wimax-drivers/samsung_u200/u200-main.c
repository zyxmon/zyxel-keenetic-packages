/*
 *
 *
 * Copyright (C) 2008 Michail Yakushin <silicium@altlinux.ru>
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 * This driver is based on drivers/usb/usb-skeleton.c
 * Wimax protocol from madwimax by Alexander Gordeev <lasaine@lvk.cs.msu.su>
 *
 * Adapt this driver by Mokrushin I.V. <mcmcc@mail.ru> for ZyXEL KEENETIC.
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <asm/uaccess.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/timer.h>
#include <net/iw_handler.h>
#include <linux/if_arp.h>
#include <linux/workqueue.h>

//#define DEBUG      1
#define FOR_ZYXEL  1

#ifdef DEBUG
#define dprintk(str...) (printk (str))
#else
#define dprintk(str...) ;/* nothing */
#endif

/* Define these values to match your devices */
#define MAX_FW_STRING           80
#define USB_U200_VENDOR_ID	0x04e9
#define USB_U200_PRODUCT_ID	0x6761
#define ESSID_AUTH                   "@yota.ru"
#define ESSID                        "YOTA"
//#define ESSID_AUTH                   "@aksoran.kz"
//#define ESSID                        "DTV"
unsigned char init_data1[] = {0x57, 0x45, 0x04, 0x00, 0x00, 0x02, 0x00, 0x74};
unsigned char init_data2[] = {0x57, 0x50, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char init_data3[] = {0x57, 0x43, 0x12, 0x00, 0x15, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x04, 0x50, 0x04, 0x00, 0x00};
char *wimax_states[] = {"INIT", "SYNC", "NEGO", "NORMAL", "SLEEP", "IDLE", "HHO", "FBSS", "RESET", "RESERVED", "UNDEFINED", "BE", "NRTPS", "RTPS", "ERTPS", "UGS", "INITIAL_RNG", "BASIC", "PRIMARY", "SECONDARY", "MULTICAST", "NORMAL_MULTICAST", "SLEEP_MULTICAST", "IDLE_MULTICAST", "FRAG_BROADCAST", "BROADCAST", "MANAGEMENT", "TRANSPORT"};
#ifdef FOR_ZYXEL
extern int wifi_link_monitor;
#endif
/* table of devices that work with this driver */
static struct usb_device_id  u200_table [] = {
	{ USB_DEVICE(USB_U200_VENDOR_ID, USB_U200_PRODUCT_ID) },
        { USB_DEVICE(0x04e8, 0x6761) },
	{ USB_DEVICE(0x04e8, 0x6731) },
	{ USB_DEVICE(0x04e8, 0x6780) },
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, u200_table);
struct workqueue_struct * u200_wq;
#define SUPPORTED_WIRELESS_EXT                  22

/* our private defines. if this grows any larger, use your own .h file */
#define MAX_TRANSFER		(PAGE_SIZE - 512)
/* MAX_TRANSFER is chosen so that the VM is not stressed by
   allocations > PAGE_SIZE and the number of packets in a page
   is an integer 512 is the largest possible packet on EHCI */
struct u200_buf_head
{
	u8 head;
	u8 type;
	u16 length;
};
/* Structure to hold all of our device specific stuff */
struct usb_u200 {
	struct usb_device	*udev;			/* the usb device for this device */
	struct usb_interface	*interface;		/* the interface for this device */
	struct usb_anchor	submitted;		/* in case we need to retract our submissions */
	size_t			bulk_in_size,bulk_out_size;		/* the size of the receive buffer */
	__u8			bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	__u8			bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	struct kref		kref;
	/*U200 device state from madwimax*/	
        char 			chip[0x40];
        char 			firmware[0x40];
        int 			net_found;
        short 			rssi;
        int 			cinr;
        unsigned char 		bsid[6];
        unsigned short 		txpwr;
        unsigned int 		freq;
        int 			state;
	
	/*net related stuffs*/
	struct net_device *net;
	struct net_device_stats stats;
	char* recv_buff;
	size_t recv_buff_size;//maximal size of recive buffer
	size_t recv_buff_seek;//current position 
	size_t recv_buff_len;//Awaiting data length
	struct work_struct 	recv_work;
	spinlock_t		recv_lock;
	struct iw_statistics iwstat;
};
#define to_u200_dev(d) container_of(d, struct usb_u200, kref)
static int
u200_get_name(struct net_device *ndev, struct iw_request_info *info,
		                 char *cwrq, char *extra);
static int u200_set_mode(struct net_device *ndev, struct iw_request_info *info,
		                 __u32 * uwrq, char *extra);
static int
u200_get_mode(struct net_device *ndev, struct iw_request_info *info,
		                 __u32 * uwrq, char *extra);
static int
u200_get_freq(struct net_device *ndev, struct iw_request_info *info,
		                 struct iw_freq *fwrq, char *extra);
static int
u200_get_range(struct net_device *ndev, struct iw_request_info *info,
		                  struct iw_point *dwrq, char *extra);
static int
u200_get_wap(struct net_device *ndev, struct iw_request_info *info,
		                struct sockaddr *awrq, char *extra);
static int
u200_set_scan(struct net_device *ndev, struct iw_request_info *info,
		                 struct iw_point *dwrq, char *extra);
static int
u200_get_scan(struct net_device *ndev, struct iw_request_info *info,
		                 union iwreq_data *dwrq, char *extra);
static int
u200_set_essid(struct net_device *ndev, struct iw_request_info *info,
		                  struct iw_point *dwrq, char *extra);
static int
u200_get_essid(struct net_device *ndev, struct iw_request_info *info,
		                  struct iw_point *dwrq, char *extra);
static int 
u200_ioctl_bigdiode(struct net_device *ndev, struct iw_request_info *info,
				  void *wrqu, char *extra);
static int
u200_ioctl_statconnect(struct net_device *ndev, struct iw_request_info *info,
				  void *wrqu, char *extra);
struct iw_statistics *u200_get_wireless_stats(struct net_device *ndev);
static const iw_handler u200_handler[] = {
	        (iw_handler) NULL,    /* SIOCSIWCOMMIT */
		(iw_handler) u200_get_name,  /* SIOCGIWNAME */
		(iw_handler) NULL,      /* SIOCSIWNWID */
		(iw_handler) NULL,      /* SIOCGIWNWID */
		(iw_handler) NULL,  /* SIOCSIWFREQ */
		(iw_handler) u200_get_freq,  /* SIOCGIWFREQ */
		(iw_handler) u200_set_mode,  /* SIOCSIWMODE */
		(iw_handler) u200_get_mode,  /* SIOCGIWMODE */
		(iw_handler) NULL,  /* SIOCSIWSENS */
		(iw_handler) NULL,  /* SIOCGIWSENS */
		(iw_handler) NULL,      /* SIOCSIWRANGE */
		(iw_handler) u200_get_range, /* SIOCGIWRANGE */
		(iw_handler) NULL,      /* SIOCSIWPRIV */
		(iw_handler) NULL,      /* SIOCGIWPRIV */
		(iw_handler) NULL,      /* SIOCSIWSTATS */
		(iw_handler) NULL,      /* SIOCGIWSTATS */
		(iw_handler) NULL,        /* SIOCSIWSPY */
		(iw_handler) NULL,     /* SIOCGIWSPY */
		(iw_handler) NULL,  /* SIOCSIWTHRSPY */
		(iw_handler) NULL,  /* SIOCGIWTHRSPY */
		(iw_handler) NULL,   /* SIOCSIWAP */
		(iw_handler) u200_get_wap,   /* SIOCGIWAP */
		(iw_handler) NULL,   /*  */
		(iw_handler) NULL,   /* SIOCGIWAPLIST  */
		(iw_handler) u200_set_scan,   /* SIOCSIWSCAN */	
		(iw_handler) u200_get_scan,   /* SIOCGIWSCAN */
		(iw_handler) u200_set_essid, /*SIOCSIWESSID*/
		(iw_handler) u200_get_essid,
};

#define PRIV_IOCTL_FWVER (SIOCIWFIRSTPRIV + 0xD)
static const struct iw_priv_args u200_privtab[] = {
		{ SIOCIWFIRSTPRIV + 0x0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "big_diode" },
		{ SIOCIWFIRSTPRIV + 0x1, 0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "stat_connect" },
		{ PRIV_IOCTL_FWVER, 0, IW_PRIV_TYPE_CHAR | IW_PRIV_SIZE_FIXED | MAX_FW_STRING, "fw_ver" },
};

static const iw_handler u200_private_handler[] = {
		[0] = (iw_handler) u200_ioctl_bigdiode,
		[1] = (iw_handler) u200_ioctl_statconnect,
};

const struct iw_handler_def u200_handler_def = {
	.num_standard = ARRAY_SIZE(u200_handler),
	.num_private = ARRAY_SIZE(u200_private_handler),
	.num_private_args = ARRAY_SIZE(u200_privtab),
	.standard = (iw_handler *) u200_handler,
	.private = u200_private_handler,
	.private_args = u200_privtab,
	.get_wireless_stats = u200_get_wireless_stats,
};


static struct usb_driver u200_driver;
static void u200_draw_down(struct usb_u200 *dev);
static ssize_t u200_write(struct usb_u200* dev, const char *inbuf, size_t count,int sync);
/*from maxwimax:protocol.c*/
static void fill_C_req(unsigned char *buf, int len)
{
	        len -= 6;
		buf[0x00] = 0x57;
		buf[0x01] = 0x43;
		buf[0x02] = len & 0xff;
		buf[0x03] = (len >> 8) & 0xff;
		memset(buf + 6, 0, 12);
}

static int fill_normal_C_req(unsigned char *buf, unsigned short type_a, unsigned short type_b, unsigned short param_len, unsigned char *param)
{
	        int len = 0x1a + param_len;
		fill_C_req(buf, len);
		buf[0x04] = 0x15;
		buf[0x05] = 0x00;
		buf[0x12] = 0x15;
		buf[0x13] = 0x00;
		buf[0x14] = type_a >> 8;
		buf[0x15] = type_a & 0xff;
		buf[0x16] = type_b >> 8;
		buf[0x17] = type_b & 0xff;
		buf[0x18] = param_len >> 8;
		buf[0x19] = param_len & 0xff;
		memcpy(buf + 0x1a, param, param_len);
		return len;
}

int fill_string_info_req(unsigned char *buf)
{
	return fill_normal_C_req(buf, 0x8, 0x1, 0x0, NULL);
}

int fill_init1_req(unsigned char *buf)
{
	unsigned char param[0x2] = {0x0, 0x1};
	return fill_normal_C_req(buf, 0x30, 0x1, sizeof(param), param);
}

int fill_mac_req(unsigned char *buf)
{
	return fill_normal_C_req(buf, 0x3, 0x1, 0x0, NULL);
}

int fill_init2_req(unsigned char *buf)
{
	return fill_normal_C_req(buf, 0x20, 0x8, 0x0, NULL);
}

int fill_init3_req(unsigned char *buf)
{
	return fill_normal_C_req(buf, 0x20, 0xc, 0x0, NULL);
}

int fill_authorization_data_req(unsigned char *buf, char *netid)
{
    short netid_len = strlen(netid) + 1;
	unsigned char param[netid_len + 4];
	param[0] = 0x0;
	param[1] = 0x10;
	param[2] = netid_len >> 8;
	param[3] = netid_len & 0xff;
	memcpy(param + 4, netid, netid_len);
	//unsigned char param[0xd] = {0x00, 0x10, 0x00, 0x09, 0x40, 0x79, 0x6f, 0x74, 0x61, 0x2e, 0x72, 0x75, 0x00};
	//unsigned char param[16] = {0x00, 0x10, 0x00, 0x0c, 0x40, 0x61, 0x6b, 0x73, 0x6f, 0x72, 0x61, 0x6e, 0x2e, 0x6b, 0x7a, 0x00};
	return fill_normal_C_req(buf, 0x20, 0x20, sizeof(param), param);
}

int fill_find_network_req(unsigned char *buf, unsigned short level)
{
	unsigned char param[0x2] = {level >> 8, level & 0xff};
	return fill_normal_C_req(buf, 0x1, 0x1, sizeof(param), param);
}

int fill_state_req(unsigned char *buf)
{
	return fill_normal_C_req(buf, 0x1, 0xb, 0x0, NULL);
}

int fill_connection_params2_req(unsigned char *buf)
{
	unsigned char param[0x2] = {0x00, 0x00};
	return fill_normal_C_req(buf, 0x1, 0x9, sizeof(param), param);
}

int fill_diode_control_cmd(unsigned char *buf, int turn_on)
{
        unsigned char param[0x2] = {0x0, turn_on ? 0x1 : 0x0};
	return fill_normal_C_req(buf, 0x30, 0x1, sizeof(param), param);
}
		
static int process_normal_C_response(struct usb_u200 *dev, const unsigned char *buf, size_t len)
{
	short type_a = (buf[0x14] << 8) + buf[0x15];
	short type_b = (buf[0x16] << 8) + buf[0x17];
	short param_len = (buf[0x18] << 8) + buf[0x19];

	if (type_a == 0x8 && type_b == 0x2) {
		if (param_len != 0x80) {
			dprintk("U200:process_normal_C_response:Bad param_len\n");
			return -EIO;

		}
		memcpy(dev->chip, buf + 0x1a, 0x40);
		memcpy(dev->firmware, buf + 0x5a, 0x40);
		printk("U200: Chip=%s, Firmware=%s.\n",dev->chip,dev->firmware);
		return 0;
	}

        if (type_a == 0x3 && type_b == 0x2) {
		if (param_len != 0x6) {
			dprintk("U200:process_normal_C_response:Bad param_len\n");
			return -EIO;
		}
		memcpy(dev->net->dev_addr,buf+0x1a,0x6);
		printk("U200: Registering network device.\n");
		if(register_netdev(dev->net)!=0)
		{
			printk("U200:Error registering netdev.\n");
			return -EIO;
		}

//		printk("net->dev_addr=%x:%x:%x:%x:%x:%x\n",dev->net->dev_addr[0],dev->net->dev_addr[1],dev->net->dev_addr[2],dev->net->dev_addr[3],dev->net->dev_addr[4],dev->net->dev_addr[5]);
		return 0;
	}								            


       if (type_a == 0x1 && type_b == 0x2) {
	       if (param_len != 0x2) {
		       dprintk( "U200:process_normal_C_response:Bad param_len\n");
		       return -EIO;
	       }
	       dev->net_found = (buf[0x1a] << 8) + buf[0x1b];
	       printk("U200: %i nets found.\n",dev->net_found);
	       return 0;
       }
       
       if (type_a == 0x1 && type_b == 0x3) {
	       if (param_len != 0x4) {
		       dprintk( "U200:process_normal_C_response:Bad param_len\n");
		       return -EIO;
	       }
	       dev->net_found = 0;
	       netif_carrier_off(dev->net);
#ifdef FOR_ZYXEL
	       wifi_link_monitor = wifi_link_monitor ? 0 : 1;
#endif
	       printk("U200: Net lost.\n");
	       return 0;
       }
       if (type_a == 0x1 && type_b == 0xa) {
	       if (param_len != 0x16) {
		       dprintk("U200:process_normal_C_response:Bad param_len\n");
		       return -EIO;
	       }
	       dev->iwstat.qual.qual = (short)((buf[0x1c] << 8) + buf[0x1d]) / 8; // CINR
	       dev->iwstat.qual.level = (buf[0x1a] << 8) + buf[0x1b]; // RSSI
	       dev->iwstat.qual.noise  = (190 + (dev->iwstat.qual.level - dev->iwstat.qual.qual)) * (-1);
	       if ( dev->iwstat.qual.qual > 25 )
	       	 dev->iwstat.qual.qual = 25;
	       dev->iwstat.qual.updated = IW_QUAL_DBM | IW_QUAL_ALL_UPDATED;
	       memcpy(dev->bsid, buf + 0x1e, 0x6);
#ifdef FOR_ZYXEL
	       if(!memcmp(dev->bsid, "\x00\x00\x00\x00\x00\x00", 6)) {
		 printk("U200: BSID is 00:00:00:00:00:00, link is %d, scan new network...\n", wifi_link_monitor);
	    	 wifi_link_monitor = wifi_link_monitor ? 0 : 1;
	       }
#endif
	       dev->txpwr = (buf[0x26] << 8) + buf[0x27];
	       dev->freq = (buf[0x28] << 24) + (buf[0x29] << 16) + (buf[0x2a] << 8) + buf[0x2b]; // in MHz
	       dev->iwstat.discard.nwid = 0;
	       dev->iwstat.miss.beacon = 0;
	   //    printk("U200: rssi:%i cinr:%i bsid:%s txpwr:%i freq:%i\n",dev->rssi,(int)dev->cinr,dev->bsid,dev->txpwr,dev->freq);
	       return 0;
       }
	  if (type_a == 0x1 && type_b == 0xc) {
		  int oldstate;
		  if (param_len != 0x2) {
			  dprintk("U200:process_normal_C_response:Bad param_len\n");
			  return -1;
		  }
		  oldstate=dev->state;
		  dev->state = (buf[0x1a] << 8) + buf[0x1b];
		  if(oldstate!=dev->state)
		  {
			  
		  	  dprintk("U200:State changed to %s(%i)\n",wimax_states[dev->state],dev->state);
			  if(dev->state==3)
			  {
				union iwreq_data iwrd;
			  	printk("U200: We connected!\n");
			  	netif_carrier_on(dev->net);
				dev->net->operstate=IF_OPER_UP;
				u200_get_wap(dev->net,NULL,(struct sockaddr *)&iwrd,NULL);
				wireless_send_event(dev->net,SIOCGIWAP,&iwrd,NULL);
			  }
		  }
		  return 0;
		}

#ifdef FOR_ZYXEL
	if (type_a == 0x1 && type_b == 0x8 && param_len == 0x2) {
		wifi_link_monitor = 1;
		return 0;
	}
	if (type_a == 0x6 && type_b == 0x7 && param_len == 0x0) {
		wifi_link_monitor = 0;
		return 0;
	}
#endif
	dprintk("U200:process_normal_C_response:Unknow C respone(type_a=%x type_b=%x param_len=%x)\n",type_a,type_b,param_len);
	//printk("U200:process_normal_C_response:other respone\n");
	return 0;
}
static int process_debug_C_response(struct usb_u200 *dev, const unsigned char *buf, size_t len)
{
		dprintk("U200:process_debug_C_response\n");
	        return 0;
}

static int process_C_response(struct usb_u200 *dev, const unsigned char *buf, size_t len)
{
	//printk("U200:process_C_response\n");
	
	if (buf[0x12] != 0x15) {
		dprintk( "U200:bad C response:Bad format\n");
		return -EIO;
	}
	
	switch (buf[0x13]) {
		case 0x00:
			return process_normal_C_response(dev, buf, len);
		case 0x02:
			return process_debug_C_response(dev, buf, len);
		case 0x04:
			dprintk("U200:process_C_response:Unknow type 0x04\n");
			return 0;
		default:
			dprintk( "U200:bad C response:Bad format\n");
			return -EIO;
	}

	return -EINVAL;
}
static int process_D_response(struct usb_u200 *dev, const unsigned char *buf, size_t len)
{

	struct sk_buff* skb;
	int retval=0;
	skb=dev_alloc_skb(len-6);
	if(!skb)
	{
		dprintk("U200:process_D_response:Can`t alloc skb\n");
		return -ENOMEM;
	}
	skb_put(skb,len-6);
	memcpy(skb->data,buf+6,len-6);
	skb->protocol=eth_type_trans(skb,dev->net);
	retval=netif_rx(skb);
	if(retval!=0)
	{
		dprintk("U200:process_D_response:netif_rx returns=%i\n",retval);
		return retval;
	}
	dev->stats.rx_packets++;
	dev->stats.rx_bytes+=len-6;
	return 0;
}

static int process_E_response(struct usb_u200 *dev, const unsigned char *buf, int len)
{
//	printk("U200:process_E_response(len=%i)\n",len);
	        return 0;
}

static int process_P_response(struct usb_u200 *dev, const unsigned char *buf, int len)
{
	
//	printk("U200:process_P_response (len=%i)\n",len);
	        return 0;
}

int update_network(struct usb_u200* dev)
{
	char*buf;
	int ret,len;
	buf=kmalloc(2048,GFP_KERNEL);
	if(buf==0)
	{
		dprintk("U200:update_network:Can`t get memory\n");
		return -ENOMEM;
	}
	
	if(dev->net_found==0)
	{
	len = fill_find_network_req(buf,1);
	if(len<0) {
		kfree(buf);
		return len;
	}
	ret=u200_write(dev,buf,len,1);
	if(ret<0)
		dprintk("U200:update_network:Can`t write message %i\n",ret);
	}
	else
	{
		len=fill_connection_params2_req(buf);
		if(len<0) {
			kfree(buf);
			return len;
		}
		ret=u200_write(dev,buf,len,1);
		if(ret<0)
			dprintk("U200:update_network:Can`t write message %i\n",ret);
		len = fill_state_req(buf);
		if(len<0) {
			kfree(buf);
			return len;
		}
		ret=u200_write(dev,buf,len,1);
		if(ret<0)
			dprintk("U200:update_network:Can`t write message %i\n",ret);
	}
	kfree(buf);
	return ret;
}
int u200_open(struct net_device* net)
{
	dprintk("U200: Open\n");
	if(net==0)
		return -EINVAL;
	if(net->state<2)
		netif_carrier_off(net);
	else
		netif_carrier_on(net);
	return 0;
}
int u200_close(struct net_device* net)
{
//	printk("u200:close\n");
	return 0;
}
// Odem
void u200_tx_timeout(struct net_device* net)
{
	struct usb_u200 *priv = netdev_priv(net);

	priv->stats.tx_errors++;
	netif_wake_queue(net);
	dprintk("U200: Tx_timeout\n");
}

static int
u200_ioctl_statconnect(struct net_device *ndev, struct iw_request_info *info, void *wrqu, char *extra)
{
	struct usb_u200 *dev = netdev_priv(ndev);
	int *val = (int *) extra;
	
	if(dev->state == 3)
	    *val = 1;
	else
	    *val = 0;
	
	return 0;
}

static int
u200_ioctl_bigdiode(struct net_device *ndev, struct iw_request_info *info, void *wrqu, char *extra)
{
	struct usb_u200 *dev = netdev_priv(ndev);
	int ret, len, val = *( (int *) extra );
	char *buf = kmalloc(2048, GFP_KERNEL);
	
	if(buf==0)
	{
		dprintk("U200:ioctl_bigdiode:Can`t get memory\n");
		return -ENOMEM;
	}
	len = fill_diode_control_cmd(buf, val);
	if(len < 0) {
	    kfree(buf);
	    return len;
	}
	ret = u200_write(dev, buf, len, 1);
	if(ret < 0)
		dprintk("U200:ioctl_bigdiode:Can`t write message %i\n", ret);
	kfree(buf);
	printk("U200: Big Diode is %s\n", val ? "ON" : "OFF");	
	return 0;
}

int u200_ioctl(struct net_device *net, struct ifreq *ifr, int cmd)
{
	struct usb_u200 *dev = netdev_priv(net);
	int status = 0;
	
	switch(cmd)
	{
	    case SIOCSIWAP:
		{
		    int len, ret;
		    char *buf;
		    
		    dev->state = 1;
		    buf = kmalloc(2048, GFP_KERNEL);
		    len = fill_find_network_req(buf, 2);
		    if(len < 0) {
			kfree(buf);
			return len;
		    }
		    ret = u200_write(dev, buf, len, 1);
		    if(ret < 0)
			dprintk("U200:u200_ioctl:Can`t write message %i\n", ret);
		    kfree(buf);
		    status = 0;
		    break;
		}
	    case SIOCSIWENCODE:
		    status = 0;
		    break;
	    case SIOCSIWAUTH:
		    status = 0;
		    break;
	    case PRIV_IOCTL_FWVER:
		{
		    char msg[MAX_FW_STRING];
		    struct iwreq *wrqu = (struct iwreq *) ifr;

		    memset(msg, 0, MAX_FW_STRING);
	
		    if(dev->firmware)
			    sprintf(msg, 
			    "\nVER: %s\nMAC: %02X%02X%02X%02X%02X%02X",
			    dev->firmware,   
			    dev->net->dev_addr[0], 
			    dev->net->dev_addr[1],
			    dev->net->dev_addr[2],
			    dev->net->dev_addr[3],
			    dev->net->dev_addr[4],
			    dev->net->dev_addr[5]);
		    else
			    sprintf(msg, "Unknown");

		    wrqu->u.data.length = strlen(msg);
		    copy_to_user(wrqu->u.data.pointer, msg, wrqu->u.data.length);
		    status = 0;
		    break;
		}
	    default:
		    status = -EOPNOTSUPP;
	}

	return status;
}

static void u200_send_callback(struct urb *urb)
{
	struct usb_u200 *dev;

	dev = urb->context;
	//printk("U200:write_bulk_callback\n");
	/* sync/async unlink faults aren't errors */
	if (urb->status) {
		if(!(urb->status == -ENOENT ||
		    urb->status == -ECONNRESET ||
		    urb->status == -ESHUTDOWN))
			dprintk("U200:u200_send_callback:%s - nonzero write bulk status received: %d",
			    __func__, urb->status);
	}

	/* free up our allocated buffer */
//	printk("u200_send_callback:%i bytes of data transmitted\n",urb->actual_length);
	usb_buffer_free(urb->dev, urb->transfer_buffer_length,
			urb->transfer_buffer, urb->transfer_dma);
}

int u200_start_xmit(struct sk_buff *skb, struct net_device *net)
{
	struct usb_u200* dev=netdev_priv(net);
	struct urb* urb;
	int retval;
	char* buf;
	const size_t header_size=6;
	size_t allsize=skb->len+header_size,sentsize=0;
	size_t sendsize=min(allsize,dev->bulk_in_size);
//	printk("u200:start_xmit (len=%i data_len=%i\n",skb->len,skb->data_len);
	urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!urb) {
		retval = -ENOMEM;
		dprintk("U200:u200_start_xmit:Can`t allocate URB\n");
		goto error;
	}

	buf = usb_buffer_alloc(dev->udev, sendsize, GFP_KERNEL, &urb->transfer_dma);
	if (!buf) {
		retval = -ENOMEM;
		dprintk("U200:u200_start_xmit:Can`t allocate buffer\n");
		goto error;
	}

	buf[0]=0x57;
	buf[1]=0x44;
	buf[2]=skb->len&0xff;
	buf[3]=(skb->len >> 8)&0xff;
	memcpy(buf+header_size, skb->data, sendsize-header_size);

	/* this lock makes sure we don't submit URBs to gone devices */
	if (!dev->interface) {		/* disconnect() was called */
		retval = -ENODEV;
		goto error;
	}

	/* initialize the urb properly */
	usb_fill_bulk_urb(urb, dev->udev,
			  usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
			  buf, sendsize, u200_send_callback, dev);
	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	usb_anchor_urb(urb, &dev->submitted);

	/* send the data out the bulk port */
	retval = usb_submit_urb(urb, GFP_KERNEL);
	if (retval) {
		dprintk("U200:u200_start_xmit:%s - failed submitting write urb, error %d", __func__, retval);
		goto error_unanchor;
	}
	sentsize=sendsize;
	/* release our reference to this urb, the USB core will eventually free it entirely */
	usb_free_urb(urb);
	while(sentsize!=allsize)
	{
		sendsize=min(allsize-sentsize,dev->bulk_out_size);
		urb = usb_alloc_urb(0, GFP_KERNEL);
		if (!urb) {
			retval = -ENOMEM;
			dprintk("U200:u200_start_xmit:Can`t allocate URB\n");
			goto error;
		}
		buf = usb_buffer_alloc(dev->udev, sendsize, GFP_KERNEL, &urb->transfer_dma);
		if (!buf) {
			retval = -ENOMEM;
			dprintk("U200:u200_start_xmit:Can`t allocate buffer\n");
			goto error;
		}
		
		memcpy(buf, skb->data+sentsize-header_size, sendsize);

		/* initialize the urb properly */
		usb_fill_bulk_urb(urb, dev->udev,
				usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
				buf, sendsize, u200_send_callback, dev);
		urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
		usb_anchor_urb(urb, &dev->submitted);
		/* send the data out the bulk port */
		retval = usb_submit_urb(urb, GFP_KERNEL);
		if (retval) {
			dprintk("U200:u200_start_xmit:%s - failed submitting write urb, error %d", __func__, retval);
			goto error_unanchor;
		}
		sentsize+=sendsize;
		/* release our reference to this urb, the USB core will eventually free it entirely */
		usb_free_urb(urb);

	}
	dev->stats.tx_packets++;
	dev->stats.tx_bytes+=sentsize;
	dev_kfree_skb(skb);
	return 0;

error_unanchor:
	usb_unanchor_urb(urb);
error:
	if (urb) {
		usb_buffer_free(dev->udev, sendsize, buf, urb->transfer_dma);
		usb_free_urb(urb);
	}
	return retval;
}

void u200_set_multicast(struct net_device* net)
{
//	printk("u200:set_multicast\n");
}
struct net_device_stats*  u200_netdev_stats(struct net_device *net)
{
	struct usb_u200* dev;
//	printk("u200:get_status carrier=%i dormant=%i present=%i running=%i \n",netif_carrier_ok(net),netif_dormant(net),netif_device_present(net),netif_running(net));
	dev=netdev_priv(net);
	return &dev->stats;
}

struct iw_statistics *u200_get_wireless_stats(struct net_device *net)
{
	struct usb_u200* dev;
	dev=netdev_priv(net);
//	printk("u200_get_wireless_stats\n");
	return &dev->iwstat;
}
static int u200_get_name(struct net_device *ndev, struct iw_request_info *info,
		                 char *cwrq, char *extra)
{
	const char* name="IEEE 802.16";
//	printk("u200:get_name\n");
	memcpy(cwrq,name,strlen(name)+1);
	return 0;
}
static int u200_get_freq(struct net_device *net, struct iw_request_info *info,
		                 struct iw_freq *fwrq, char *extra)
{
	struct usb_u200* dev;
//	printk("u200_get_freq\n");
	dev=netdev_priv(net);
	fwrq->m=dev->freq/1000;
	fwrq->e=6;
	fwrq->i=1;
	return 0;
}
static int
u200_get_range(struct net_device *net, struct iw_request_info *info,
		                  struct iw_point *dwrq, char *extra)
{
	struct usb_u200* dev;
	struct iw_range *range=(struct iw_range*)extra;
//	printk("u200_get_range\n");
	dev=netdev_priv(net);
	memset(range, 0, sizeof (struct iw_range));
	dwrq->length = sizeof (struct iw_range);
        range->we_version_source = SUPPORTED_WIRELESS_EXT;
	range->we_version_compiled = WIRELESS_EXT;
	
	range->min_retry = 0;
	range->max_retry = 255;

	range->sensitivity=127;//May be 92
	range->throughput = 10 * 1000 * 1000;

	range->max_qual.qual=25; // 92
	range->max_qual.level=-20; // -10
	range->max_qual.noise=0; // 0

	range->avg_qual.qual=15; // 50
	range->avg_qual.level=-60; // -50
	range->avg_qual.noise=0; // -10

	range->bitrate[0]=10*1024*1024;
	range->num_bitrates=1;

	range->num_frequency=1;
	range->num_channels=1;
	range->freq[0].m=2525;
	range->freq[0].e=6;
	range->freq[0].i=1;


	return 0;
}
static int u200_get_wap(struct net_device *net, struct iw_request_info *info, struct sockaddr *awrq, char *extra)
{
	struct usb_u200 *dev;
	//printk("u200_get_wap\n");
	dev=netdev_priv(net);
	awrq->sa_family = ARPHRD_ETHER;
	if(dev->net_found==0)
	{
		memset(awrq->sa_data,0,6);
	}
	else
	{
		memcpy(awrq->sa_data,dev->bsid,6);
	}
	return 0;
}

static int u200_get_scan(struct net_device *net, struct iw_request_info *info, union iwreq_data *dwrq, char *extra)
{
	struct usb_u200 *dev;
	char *ev=extra;
	char *stop;
	struct iw_event iw;
	//printk("u200_get_scan\n");
	dev=netdev_priv(net);
	
	stop=extra+dwrq->data.length;
	
	iw.cmd=SIOCGIWAP;
	iw.u.ap_addr.sa_family = ARPHRD_ETHER;
	memcpy(iw.u.ap_addr.sa_data, dev->bsid, ETH_ALEN);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
	ev=iwe_stream_add_event(info,ev,stop,&iw,IW_EV_ADDR_LEN);
#else
	ev=iwe_stream_add_event(ev,stop,&iw,IW_EV_ADDR_LEN);
#endif	
	iw.cmd = SIOCGIWESSID;
        iw.u.data.flags = 1;
	iw.u.data.length = sizeof(ESSID);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
	ev = iwe_stream_add_point(info, ev, stop,&iw, ESSID);
#else
	ev = iwe_stream_add_point(ev, stop,&iw, ESSID);
#endif
	iw.cmd = SIOCGIWFREQ;
	iw.u.freq.m = dev->freq;
	iw.u.freq.e = 3;
	iw.u.freq.i = 1;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
	ev = iwe_stream_add_event(info, ev, stop,&iw, IW_EV_FREQ_LEN);
#else
	ev = iwe_stream_add_event(ev, stop,&iw, IW_EV_FREQ_LEN);
#endif
	iw.u.qual=dev->iwstat.qual;
	iw.cmd = IWEVQUAL;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
	ev = iwe_stream_add_event(info, ev, stop,&iw, IW_EV_QUAL_LEN);
#else
	ev = iwe_stream_add_event(ev, stop,&iw, IW_EV_QUAL_LEN);
#endif
	dwrq->data.length=ev-extra;
	dwrq->data.flags=0;
	update_network(dev);
	return 0;
}
static int u200_set_scan(struct net_device *net, struct iw_request_info *info, struct iw_point *dwrq, char *extra)
{
	struct usb_u200 *dev;
	//printk("u200_set_scan\n");
	dev=netdev_priv(net);
	return update_network(dev);
}
static int u200_get_mode(struct net_device *net, struct iw_request_info *info,
		                 __u32 * uwrq, char *extra)
{
	struct usb_u200 * dev;
	//printk("u200_get_mode\n");
	dev=netdev_priv(net);
	*uwrq=2;
	return 0;
}
static int u200_set_mode(struct net_device *net, struct iw_request_info *info, __u32 * uwrq, char *extra)
{
	struct usb_u200 * dev;
	//printk("u200_set_mode\n");
	dev=netdev_priv(net);
	if((*uwrq)==0)
		dev->state=1;
//	printk("set mode 0x%x\n",*uwrq);
	return 0;

}
static int
u200_set_essid(struct net_device *ndev, struct iw_request_info *info,
		                  struct iw_point *dwrq, char *extra)
{
	//printk("u200_set_ssid\n");
	return 0;
}
static int u200_get_essid(struct net_device *ndev, struct iw_request_info *info, struct iw_point *dwrq, char *extra)
{
	char* essid=ESSID;
	//printk("u200_get_ssid\n");
	memcpy(extra,essid,strlen(essid));
	dwrq->flags=1;
	dwrq->length=strlen(essid);
	return 0;
}

static void u200_delete(struct kref *kref)
{
	struct usb_u200 *dev = to_u200_dev(kref);
	dprintk("U200: Delete\n");
	usb_put_dev(dev->udev);
	free_netdev(dev->net);
}

int u200_beginread(struct usb_u200*);
void u200_recv_workfunc(struct work_struct *ws)
{
	struct usb_u200 *dev;
	struct u200_buf_head *head;
	const unsigned  char* buf;
	size_t len;
	dev=container_of(ws,struct usb_u200,recv_work);
	head=(struct u200_buf_head*)dev->recv_buff;
	buf=dev->recv_buff;
	len=dev->recv_buff_len;
	switch (head->type) {
		case 0x43:
			process_C_response(dev, buf, len);
			break;
		case 0x44:
			process_D_response(dev, buf, len);
			break;
		case 0x45:
			process_E_response(dev, buf, len);
			break;
		case 0x50:
			process_P_response(dev, buf, len);
			break;
		default:
			dprintk("U200:u200_recv_workfunc:Bad response type: %02x\n", head->type);	
			break;
	}
	dev->recv_buff_len=0;
	dev->recv_buff_seek=0;
	u200_beginread(dev);
}
static void u200_read_bulk_callback(struct urb *urb)
{
	struct usb_u200 *dev;
	int rerun=1;
	dev = urb->context;
	//printk("U200:read_bulk_callback\n");
	/* sync/async unlink faults aren't errors */
	if (urb->status) {
		if(!(urb->status == -ENOENT ||
		    urb->status == -ECONNRESET ||
		    urb->status == -ESHUTDOWN))
			dprintk("U200:read_bulk_callback:%s - nonzero read bulk status received: %d:async read stoped",
			    __func__, urb->status);
		rerun=0;
	}

	if(!urb->status)
	{
		if(!dev->recv_buff)
		{
			dprintk("U200:read_bulk_callback:We have no recive buffer\n");
			goto out;
		}

		if(dev->recv_buff_seek+urb->actual_length > dev->recv_buff_size)
		{
			dprintk("U200:read_bulk_callback:Error:recv_buff_seek+len>recv_buff_size. Discarding buffer\n");
			dev->recv_buff_seek=0;
			goto out;
		}

		if(dev->recv_buff_seek==0)
		{
			struct u200_buf_head *head=(struct u200_buf_head*)urb->transfer_buffer;
			if(urb->actual_length <4)
			{
				dprintk("U200:read_bulk_callback:Shot read!\n");
				goto out;
			}
			if(head->head!=0x57)
			{
				dprintk("U200:read_bulk_callback:Invalid buffer\n");
				goto out;

			}
			dev->recv_buff_len=4+head->length;
			if(head->type == 0x43 || head->type == 0x44) {
				dev->recv_buff_len+= 2;
			}

			if(dev->recv_buff_len>dev->recv_buff_size)
			{
				dprintk("U200:read_bulk_callback:Error reciving data too big %i, maximum %i\n",dev->recv_buff_len,dev->recv_buff_size);
				goto out;
			}
		}

		memcpy(dev->recv_buff+dev->recv_buff_seek,urb->transfer_buffer,urb->actual_length);
		dev->recv_buff_seek+=urb->actual_length;

		if(dev->recv_buff_len==dev->recv_buff_seek)
		{
			queue_work(u200_wq,&dev->recv_work);
			rerun=0;
			goto out;
		}
		if(dev->recv_buff_seek>dev->recv_buff_len)
		{
			dprintk("U200:read_bulk_callback:Error:Buffer overrun seek=%i len=%i\n",dev->recv_buff_seek,dev->recv_buff_len);
			dev->recv_buff_seek=0;
			goto out;
		}
	}
	else
		dprintk("U200:read_bulk_callback:Error bulk status %i\n",urb->status);
	/* free up our allocated buffer */
out:
	usb_buffer_free(urb->dev, urb->transfer_buffer_length,
			urb->transfer_buffer, urb->transfer_dma);
	if(rerun==1)
		u200_beginread(dev);
}


int u200_beginread(struct usb_u200 *dev)
{
	struct urb* urb;
	int retval=0;
	char* buf;
	urb=usb_alloc_urb(0,GFP_KERNEL);
	if(!urb)
	{
		dprintk("U200:u200_beginread:Can`t alloc urb\n");
		retval=-ENOMEM;
		goto exit;
	}
	buf = usb_buffer_alloc(dev->udev, dev->bulk_in_size, GFP_KERNEL, &urb->transfer_dma);
	if (!buf) {
		retval = -ENOMEM;
		dprintk("U200:u200_beginread:Can`t allocate buffer\n");
		goto error;
	}

	/* initialize the urb properly */
	usb_fill_bulk_urb(urb, dev->udev,
			  usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
			  buf, dev->bulk_in_size, u200_read_bulk_callback, dev);
	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	usb_anchor_urb(urb, &dev->submitted);

	/* send the data out the bulk port */
	retval = usb_submit_urb(urb, GFP_KERNEL);
	if (retval) {
		dprintk("U200:u200_beginread:%s - failed submitting write urb, error %d", __func__, retval);
		goto error_unanchor;
	}

	/* release our reference to this urb, the USB core will eventually free it entirely */
	usb_free_urb(urb);

	return 0;

error_unanchor:
	usb_unanchor_urb(urb);
error:
	if (urb) {
		usb_buffer_free(dev->udev, dev->bulk_in_size, buf, urb->transfer_dma);
		usb_free_urb(urb);
	}

exit:
	return retval;
}
static void u200_write_bulk_callback(struct urb *urb)
{
	struct usb_u200 *dev;

	dev = urb->context;
	if(dev==0)
	{
		dprintk("U200:u200_write_bulk_callback:dev=0!\n");
		return;
	}
	//printk("U200:write_bulk_callback\n");
	/* sync/async unlink faults aren't errors */
	if (urb->status) {
		if(!(urb->status == -ENOENT ||
		    urb->status == -ECONNRESET ||
		    urb->status == -ESHUTDOWN))
			dprintk("U200:u200_write_bulk_callback:%s - nonzero write bulk status received: %d",
			    __func__, urb->status);
	}

	/* free up our allocated buffer */
	usb_buffer_free(urb->dev, urb->transfer_buffer_length,
			urb->transfer_buffer, urb->transfer_dma);
}

static ssize_t u200_write(struct usb_u200* dev, const char *inbuf, size_t count,int sync)
{
	int retval = 0;
	struct urb *urb = NULL;
	size_t writesize = min(count, (size_t)MAX_TRANSFER);
	char *buf;
	//printk("U200:write\n");

	/* verify that we actually have some data to write */
	if (count == 0)
		goto exit;

	/* create a urb, and a buffer for it, and copy the data to the urb */
	urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!urb) {
		retval = -ENOMEM;
		dprintk("U200:u200_write:Can`t allocate URB\n");
		goto error;
	}

	buf = usb_buffer_alloc(dev->udev, writesize, GFP_KERNEL, &urb->transfer_dma);
	if (!buf) {
		retval = -ENOMEM;
		dprintk("U200:u200_write:Can`t allocate buffer\n");
		goto error;
	}

	memcpy(buf, inbuf, writesize);
//		retval = -EFAULT;
//		printk("U200:u200_write: can`t copy to buf(size=%i)\n",writesize);
//		goto error;
//	}

	/* this lock makes sure we don't submit URBs to gone devices */
	if (!dev->interface) {		/* disconnect() was called */
		retval = -ENODEV;
		goto error;
	}

	/* initialize the urb properly */
	usb_fill_bulk_urb(urb, dev->udev,
			  usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
			  buf, writesize, u200_write_bulk_callback, dev);
	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	usb_anchor_urb(urb, &dev->submitted);

	/* send the data out the bulk port */
	retval = usb_submit_urb(urb, GFP_KERNEL);
	if (retval) {
		dprintk("U200:u200_write:%s - failed submitting write urb, error %d", __func__, retval);
		goto error_unanchor;
	}

	/* release our reference to this urb, the USB core will eventually free it entirely */
	usb_free_urb(urb);


	return writesize;

error_unanchor:
	usb_unanchor_urb(urb);
error:
	if (urb) {
		usb_buffer_free(dev->udev, writesize, buf, urb->transfer_dma);
		usb_free_urb(urb);
	}

exit:
	return retval;
}
/*
static const struct file_operations u200_fops = {
	.owner =	THIS_MODULE,
	.read =		u200_read,
	.write =	u200_write,
	.open =		u200_open,
	.release =	u200_release,
	.flush =	u200_flush,
};
*/
/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with the driver core
 **/
/*
static struct usb_class_driver u200_class = {
	.name =		"u200-%d",
	.fops =		&u200_fops,
	.minor_base =	USB_SKEL_MINOR_BASE,
};
*/
// Odem: adapt to the new kernel API
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,28)
static const struct net_device_ops u200NetDevOps =
{
    .ndo_open = u200_open,
    .ndo_stop = u200_close,
    .ndo_tx_timeout = u200_tx_timeout,
    .ndo_do_ioctl = u200_ioctl,
    .ndo_start_xmit = u200_start_xmit,
    .ndo_set_multicast_list = u200_set_multicast,
    .ndo_get_stats = u200_netdev_stats,
};
#endif

static int u200_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_u200 *dev;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	size_t buffer_size=0;
	char* writebuffer=0;
	size_t writebuffer_size=0x4000;
	struct net_device *net;
	int i,len;
	int retval = -ENOMEM;

	dprintk("U200: Probe\n");
	
	net=alloc_etherdev(sizeof(struct usb_u200));
	if(!net)
	{
		err("U200:probe:Can`t allocate device\n");
		return -ENOMEM;
	}

	dev=netdev_priv(net);
	dev->net=net;

	kref_init(&dev->kref);
	init_usb_anchor(&dev->submitted);
	dev->udev = usb_get_dev(interface_to_usbdev(interface));
	dev->interface = interface;
	INIT_WORK(&dev->recv_work,u200_recv_workfunc);
	spin_lock_init(&dev->recv_lock);
	dev->recv_buff_size=2048;
	dev->recv_buff=kmalloc(dev->recv_buff_size,GFP_KERNEL);
	dev->recv_buff_seek=0;
	/* set up the endpoint information */
	/* use only the first bulk-in and bulk-out endpoints */
	iface_desc = interface->cur_altsetting;
	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i].desc;

		if (!dev->bulk_in_endpointAddr &&
		    usb_endpoint_is_bulk_in(endpoint)&&
		    endpoint->bEndpointAddress==0x82) {
			/* we found a bulk in endpoint */
			buffer_size = le16_to_cpu(endpoint->wMaxPacketSize);
			dev->bulk_in_size = buffer_size;
			dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;		
		}

		if (!dev->bulk_out_endpointAddr &&
		    usb_endpoint_is_bulk_out(endpoint)&&
		    endpoint->bEndpointAddress==0x04) {
			/* we found a bulk out endpoint */
			dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
			dev->bulk_out_size=le16_to_cpu(endpoint->wMaxPacketSize);
		}
	}
	if (!(dev->bulk_in_endpointAddr && dev->bulk_out_endpointAddr)) {
		err("U200:probe:Could not find both bulk-in and bulk-out endpoints");
		goto error;
	}

	/* save our data pointer in this interface device */
	usb_set_intfdata(interface, dev);	
	SET_NETDEV_DEV(net, &interface->dev);
	strcpy(net->name, "wimax%d");
	net->watchdog_timeo = 10;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,28)
	net->netdev_ops = &u200NetDevOps;
#else
   	net->open = u200_open;
	net->stop = u200_close;
	net->tx_timeout = u200_tx_timeout;
	net->do_ioctl = u200_ioctl;
	net->hard_start_xmit = u200_start_xmit;
	net->set_multicast_list = u200_set_multicast;
	net->get_stats = u200_netdev_stats;
#endif
	net->mtu=1386;
	net->wireless_handlers=&u200_handler_def;
#ifdef FOR_ZYXEL
	wifi_link_monitor = 0;
#endif


	retval=u200_beginread(dev);
	if(retval!=0)
	{
		dprintk("U200:probe:Can`t start async read %i\n",retval);
		goto error;
	}

	
	writebuffer=kmalloc(writebuffer_size,GFP_KERNEL);
	if(!writebuffer)
	{
		dprintk("U200:probe:Can`t alloc wire buffer\n");
		retval=-ENOMEM;
		goto init_error;
	}

	retval=u200_write(dev,init_data1,sizeof(init_data1),1);
	if(retval!=sizeof(init_data1))
	{
		dprintk("U200:probe:Can`t initilize device!\n");
		goto init_error;
	}
	
	retval=u200_write(dev,init_data2,sizeof(init_data2),1);
	if(retval!=sizeof(init_data2))
	{
		dprintk("U200:probe:Can`t initilize device(2)!\n");
		goto init_error;
	}
	
	retval=u200_write(dev,init_data3,sizeof(init_data3),1);
	if(retval!=sizeof(init_data3))
	{
		dprintk("U200:probe:Can`t initilize device(3)!\n");
		goto init_error;
	}
	
	len=fill_string_info_req(writebuffer);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		dprintk("U200:probe:Can`t send info request\n");
		goto init_error;
	}

	len=fill_init1_req(writebuffer);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		err("U200:probe:Can`t send init1 request");
		goto init_error;
	}
	
	len=fill_mac_req(writebuffer);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		err("U200:probe:Can`t send mac request\n");
		goto init_error;
	}
	
	len=fill_string_info_req(writebuffer);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		err("U200:probe:Can`t send string info request\n");
		goto init_error;
	}
	
	len=fill_init2_req(writebuffer);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		err("U200:probe:Can`t send init2 request\n");
		goto init_error;
	}
	
	len=fill_init3_req(writebuffer);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		err("U200:probe:Can`t send init3 request\n");
		goto init_error;
	}
	

	len=fill_authorization_data_req(writebuffer, ESSID_AUTH);
	retval=u200_write(dev,writebuffer,len,1);
	if(retval<len)
	{
		err("U200:probe:Can`t send authorization_data request\n");
		goto init_error;
	}
	
	//info("probe complite\n");
	kfree(writebuffer);

	return 0;
init_error:
	if(writebuffer!=0)
	{
		kfree(writebuffer);
		writebuffer=0;
	}
	usb_set_intfdata(interface,NULL);
error:
	if (dev)
	{
		/* this frees allocated memory */
		kref_put(&dev->kref, u200_delete);
	}
	return retval;
}

static void u200_disconnect(struct usb_interface *interface)
{
	struct usb_u200 *dev;
	//int minor = interface->minor;
	printk("U200: Disconnect\n");
	dev = usb_get_intfdata(interface);
	unregister_netdev(dev->net);
	usb_set_intfdata(interface, NULL);

	/* prevent more I/O from starting */
	dev->interface = NULL;
	if(dev->recv_buff)
	{
		kfree(dev->recv_buff);
		dev->recv_buff=0;
	}
	usb_kill_anchored_urbs(&dev->submitted);
	/* decrement our usage count */
	kref_put(&dev->kref, u200_delete);
#ifdef FOR_ZYXEL
	wifi_link_monitor = 0;
#endif
//	info("USB U200 #%d now disconnected", minor);
}

static void u200_draw_down(struct usb_u200 *dev)
{
	int time;
	printk("U200: Draw_down\n");
	time = usb_wait_anchor_empty_timeout(&dev->submitted, 1000);
	if (!time)
		usb_kill_anchored_urbs(&dev->submitted);
}

static int u200_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct usb_u200 *dev = usb_get_intfdata(intf);

	if (!dev)
		return 0;
	printk("U200: Suspend\n");
	u200_draw_down(dev);
	return 0;
}

static int u200_resume (struct usb_interface *intf)
{
	printk("U200: Resume\n");
	return 0;
}

static int u200_pre_reset(struct usb_interface *intf)
{
	struct usb_u200 *dev = usb_get_intfdata(intf);
	printk("U200: Pre_reset\n");
	u200_draw_down(dev);

	return 0;
}

static int u200_post_reset(struct usb_interface *intf)
{
	//struct usb_u200 *dev = usb_get_intfdata(intf);

	/* we are sure no URBs are active - no locking needed */
	printk("U200: Post_reset\n");

	return 0;
}

static struct usb_driver u200_driver = {
	.name =		"u200",
	.probe =	u200_probe,
	.disconnect =	u200_disconnect,
	.suspend =	u200_suspend,
	.resume =	u200_resume,
	.pre_reset =	u200_pre_reset,
	.post_reset =	u200_post_reset,
	.id_table =	u200_table,
	.supports_autosuspend = 1,
};

static int __init usb_u200_init(void)
{
	int result;
	printk("U200: Init\n");
	/* register this driver with the USB subsystem */
	dprintk("U200:init:Creating workqueue\n");
	u200_wq=create_freezeable_workqueue("U200");
	if(!u200_wq)
		return -ENOMEM;
	result = usb_register(&u200_driver);
	if (result)
		err("U200:init:Usb_register failed. Error number %d", result);

	return result;
}

static void __exit usb_u200_exit(void)
{
	/* deregister this driver with the USB subsystem */
	printk("U200: Exit\n");
	destroy_workqueue(u200_wq);
	usb_deregister(&u200_driver);
}

module_init(usb_u200_init);
module_exit(usb_u200_exit);

MODULE_LICENSE("GPL");

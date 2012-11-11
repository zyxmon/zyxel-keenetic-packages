#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <ctype.h>

#include <sys/time.h>
#include <sys/ioctl.h>

#include <linux/types.h>
#include "videodev2.h"

#include "struct-dump.h"
#include "struct-v4l.h"
#include "struct-v4l2.h"

/* --------------------------------------------------------------------- */
/* v4l2                                                                  */

static int dump_v4l2(int fd, int tab)
{
	struct v4l2_capability  capability;
	struct v4l2_standard    standard;
	struct v4l2_input       input;
	struct v4l2_tuner       tuner;
	struct v4l2_fmtdesc     fmtdesc;
	struct v4l2_format      format;
	struct v4l2_framebuffer fbuf;
	struct v4l2_queryctrl   qctrl;
	int i;

	printf("general info\n");
	memset(&capability,0,sizeof(capability));
	if (-1 == ioctl(fd,VIDIOC_QUERYCAP,&capability))
		return -1;
	printf("    VIDIOC_QUERYCAP\n");
	print_struct(stdout,desc_v4l2_capability,&capability,"",tab);
	printf("\n");

	printf("standards\n");
	for (i = 0;; i++) {
		memset(&standard,0,sizeof(standard));
		standard.index = i;
		if (-1 == ioctl(fd,VIDIOC_ENUMSTD,&standard))
			break;
		printf("    VIDIOC_ENUMSTD(%d)\n",i);
		print_struct(stdout,desc_v4l2_standard,&standard,"",tab);
	}
	printf("\n");

	printf("inputs\n");
	for (i = 0;; i++) {
		memset(&input,0,sizeof(input));
		input.index = i;
		if (-1 == ioctl(fd,VIDIOC_ENUMINPUT,&input))
			break;
		printf("    VIDIOC_ENUMINPUT(%d)\n",i);
		print_struct(stdout,desc_v4l2_input,&input,"",tab);
	}
	printf("\n");

	if (capability.capabilities & V4L2_CAP_TUNER) {
		printf("tuners\n");
		for (i = 0;; i++) {
			memset(&tuner,0,sizeof(tuner));
			tuner.index = i;
			if (-1 == ioctl(fd,VIDIOC_G_TUNER,&tuner))
				break;
			printf("    VIDIOC_G_TUNER(%d)\n",i);
			print_struct(stdout,desc_v4l2_tuner,&tuner,"",tab);
		}
		printf("\n");
	}

	if (capability.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
		printf("video capture\n");
		for (i = 0;; i++) {
			memset(&fmtdesc,0,sizeof(fmtdesc));
			fmtdesc.index = i;
			fmtdesc.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (-1 == ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc))
				break;
			printf("    VIDIOC_ENUM_FMT(%d,VIDEO_CAPTURE)\n",i);
			print_struct(stdout,desc_v4l2_fmtdesc,&fmtdesc,"",tab);
		}
		memset(&format,0,sizeof(format));
		format.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == ioctl(fd,VIDIOC_G_FMT,&format)) {
			perror("VIDIOC_G_FMT(VIDEO_CAPTURE)");
		} else {
			printf("    VIDIOC_G_FMT(VIDEO_CAPTURE)\n");
			print_struct(stdout,desc_v4l2_format,&format,"",tab);
		}
		printf("\n");
	}

	if (capability.capabilities & V4L2_CAP_VIDEO_OVERLAY) {
		printf("video overlay\n");
		for (i = 0;; i++) {
			memset(&fmtdesc,0,sizeof(fmtdesc));
			fmtdesc.index = i;
			fmtdesc.type  = V4L2_BUF_TYPE_VIDEO_OVERLAY;
			if (-1 == ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc))
				break;
			printf("    VIDIOC_ENUM_FMT(%d,VIDEO_OVERLAY)\n",i);
			print_struct(stdout,desc_v4l2_fmtdesc,&fmtdesc,"",tab);
		}
		memset(&format,0,sizeof(format));
		format.type  = V4L2_BUF_TYPE_VIDEO_OVERLAY;
		if (-1 == ioctl(fd,VIDIOC_G_FMT,&format)) {
			perror("VIDIOC_G_FMT(VIDEO_OVERLAY)");
		} else {
			printf("    VIDIOC_G_FMT(VIDEO_OVERLAY)\n");
			print_struct(stdout,desc_v4l2_format,&format,"",tab);
		}
		memset(&fbuf,0,sizeof(fbuf));
		if (-1 == ioctl(fd,VIDIOC_G_FBUF,&fbuf)) {
			perror("VIDIOC_G_FBUF");
		} else {
			printf("    VIDIOC_G_FBUF\n");
			print_struct(stdout,desc_v4l2_framebuffer,&fbuf,"",tab);
		}
		printf("\n");
	}

	if (capability.capabilities & V4L2_CAP_VBI_CAPTURE) {
		printf("vbi capture\n");
		for (i = 0;; i++) {
			memset(&fmtdesc,0,sizeof(fmtdesc));
			fmtdesc.index = i;
			fmtdesc.type  = V4L2_BUF_TYPE_VBI_CAPTURE;
			if (-1 == ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc))
				break;
			printf("    VIDIOC_ENUM_FMT(%d,VBI_CAPTURE)\n",i);
			print_struct(stdout,desc_v4l2_fmtdesc,&fmtdesc,"",tab);
		}
		memset(&format,0,sizeof(format));
		format.type  = V4L2_BUF_TYPE_VBI_CAPTURE;
		if (-1 == ioctl(fd,VIDIOC_G_FMT,&format)) {
			perror("VIDIOC_G_FMT(VBI_CAPTURE)");
		} else {
			printf("    VIDIOC_G_FMT(VBI_CAPTURE)\n");
			print_struct(stdout,desc_v4l2_format,&format,"",tab);
		}
		printf("\n");
	}

	printf("controls\n");
	for (i = 0; i < V4L2_CID_LASTP1 - V4L2_CID_USER_BASE; i++) {
		memset(&qctrl,0,sizeof(qctrl));
		qctrl.id = V4L2_CID_BASE+i;
		if (-1 == ioctl(fd,VIDIOC_QUERYCTRL,&qctrl))
			continue;
		if (qctrl.flags & V4L2_CTRL_FLAG_DISABLED)
			continue;
		printf("    VIDIOC_QUERYCTRL(BASE+%d)\n",i);
		print_struct(stdout,desc_v4l2_queryctrl,&qctrl,"",tab);
	}

	for (i = 0;; i++) {
		memset(&qctrl,0,sizeof(qctrl));
		qctrl.id = V4L2_CID_PRIVATE_BASE+i;
		if (-1 == ioctl(fd,VIDIOC_QUERYCTRL,&qctrl))
			break;
		if (qctrl.flags & V4L2_CTRL_FLAG_DISABLED)
			continue;
		printf("    VIDIOC_QUERYCTRL(PRIVATE_BASE+%d)\n",i);
		print_struct(stdout,desc_v4l2_queryctrl,&qctrl,"",tab);
	}
	return 0;
}

/* --------------------------------------------------------------------- */
/* main                                                                  */

int main(int argc, char *argv[])
{
	char dummy[256];
	char *device = "/dev/video0";
	int tab = 1;
	int fd;

	if (argc > 1)
		device = argv[1];

	fd = open(device,O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr,"open %s: %s\n",device,strerror(errno));
		exit(1);
	};

	if (-1 != ioctl(fd,VIDIOC_QUERYCAP,dummy)) {
		printf("\n### v4l2 device info [%s] ###\n",device);
		dump_v4l2(fd,tab);
	} else {
		fprintf(stderr,"%s: not an video4linux device\n",device);
		exit(1);
	}
	return 0;
}
/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */

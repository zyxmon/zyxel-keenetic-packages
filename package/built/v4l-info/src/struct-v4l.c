#include <stdio.h>
#include <sys/ioctl.h>

#include "videodev.h"

#include "struct-dump.h"
#include "struct-v4l.h"

/* ---------------------------------------------------------------------- */

char *bits_vid_cap[32] = {
	"CAPTURE",
	"TUNER",
	"TELETEXT",
	"OVERLAY",
	"CHROMAKEY",
	"CLIPPING",
	"FRAMERAM",
	"SCALES",
	"MONOCHROME",
	"SUBCAPTURE",
	"MPEG_DECODER",
	"MPEG_ENCODER",
	"MJPEG_DECODER",
	"MJPEG_ENCODER",
};

char *bits_chan_flags[32] = {
	"TUNER",
	"AUDIO",
};

char *desc_chan_type[] = {
	[ VIDEO_TYPE_TV     ] = "TV",
	[ VIDEO_TYPE_CAMERA ] = "CAMERA",
};

char *bits_tuner_flags[32] = {
	"PAL",
	"NTSC",
	"SECAM",
	"LOW",
	"NORM",
	"?",
	"?",
	"STEREO_ON",
	"RDS_ON",
	"MBS_ON",
};

char *desc_tuner_mode[] = {
	[ VIDEO_MODE_PAL ]   = "PAL",
	[ VIDEO_MODE_NTSC ]  = "NTSC",
	[ VIDEO_MODE_SECAM ] = "SECAM",
	[ VIDEO_MODE_AUTO ]  = "AUTO",
};

char *desc_pict_palette[] = {
	[ VIDEO_PALETTE_GREY ]    = "GREY",
	[ VIDEO_PALETTE_HI240 ]   = "HI240",
	[ VIDEO_PALETTE_RGB565 ]  = "RGB565",
	[ VIDEO_PALETTE_RGB24 ]   = "RGB24",
	[ VIDEO_PALETTE_RGB32 ]   = "RGB32",
	[ VIDEO_PALETTE_RGB555 ]  = "RGB555",
	[ VIDEO_PALETTE_YUV422 ]  = "YUV422",
	[ VIDEO_PALETTE_YUYV ]    = "YUYV",
	[ VIDEO_PALETTE_UYVY ]    = "UYVY",
	[ VIDEO_PALETTE_YUV420 ]  = "YUV420",
	[ VIDEO_PALETTE_YUV411 ]  = "YUV411",
	[ VIDEO_PALETTE_RAW ]     = "RAW",
	[ VIDEO_PALETTE_YUV422P ] = "YUV422P",
	[ VIDEO_PALETTE_YUV411P ] = "YUV411P",
	[ VIDEO_PALETTE_YUV420P ] = "YUV420P",
	[ VIDEO_PALETTE_YUV410P ] = "YUV410P",
};

char *bits_audio_flags[32] = {
	"MUTE",
	"MUTABLE",
	"VOLUME",
	"BASS",
	"TREBLE",
	"BALANCE",
};

char *bits_audio_mode[32] = {
	"MONO",
	"STEREO",
	"LANG1",
	"LANG2",
};

/* ---------------------------------------------------------------------- */

struct struct_desc desc_video_capability[] = {{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = BITS32,
	.name   = "type",
	.bits   = bits_vid_cap,
},{
	.type   = SINT32,
	.name   = "channels",
},{
	.type   = SINT32,
	.name   = "audios",
},{
	.type   = SINT32,
	.name   = "maxwidth",
},{
	.type   = SINT32,
	.name   = "maxheight",
},{
	.type   = SINT32,
	.name   = "minwidth",
},{
	.type   = SINT32,
	.name   = "minheight",
},{
	/* end of list */
}};

struct struct_desc desc_video_channel[] = {{
	.type   = SINT32,
	.name   = "channel",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = SINT32,
	.name   = "tuners",
},{
	.type   = BITS32,
	.name   = "flags",
	.bits   = bits_chan_flags
},{
	.type   = ENUM16,
	.name   = "type",
	.enums  = desc_chan_type,
	.length = sizeof(desc_chan_type) / sizeof(char*),
},{
	.type   = UINT16,
	.name   = "norm",
},{
	/* end of list */
}};

struct struct_desc desc_video_tuner[] = {{
	.type   = SINT32,
	.name   = "tuner",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = UINT32,
	.name   = "rangelow",
},{
	.type   = UINT32,
	.name   = "rangehigh",
},{
	.type   = BITS32,
	.name   = "flags",
	.bits   = bits_tuner_flags,
},{
	.type   = ENUM16,
	.name   = "mode",
	.enums  = desc_tuner_mode,
	.length = sizeof(desc_tuner_mode) / sizeof(char*),
},{
	.type   = UINT16,
	.name   = "signal",
},{
	/* end of list */
}};

struct struct_desc desc_video_picture[] = {{
	.type   = UINT16,
	.name   = "brightness",
},{
	.type   = UINT16,
	.name   = "hue",
},{
	.type   = UINT16,
	.name   = "colour",
},{
	.type   = UINT16,
	.name   = "contrast",
},{
	.type   = UINT16,
	.name   = "whiteness",
},{
	.type   = UINT16,
	.name   = "depth",
},{
	.type   = ENUM16,
	.name   = "palette",
	.enums  = desc_pict_palette,
	.length = sizeof(desc_pict_palette) / sizeof(char*),
},{
	/* end of list */
}};

struct struct_desc desc_video_audio[] = {{
	.type   = SINT32,
	.name   = "audio",
},{
	.type   = UINT16,
	.name   = "volume",
},{
	.type   = UINT16,
	.name   = "bass",
},{
	.type   = UINT16,
	.name   = "treble",
},{
	.type   = PADDING,
	.length = 2,
},{
	.type   = BITS32,
	.name   = "flags",
	.bits   = bits_audio_flags,
},{
	.type   = STRING,
	.name   = "name",
	.length = 16,
},{
	.type   = BITS16,
	.name   = "mode",
	.bits   = bits_audio_mode,
},{
	.type   = UINT16,
	.name   = "balance",
},{
	.type   = UINT16,
	.name   = "step",
},{
	/* end of list */
}};

struct struct_desc desc_video_window[] = {{
	.type   = UINT32,
	.name   = "x",
},{
	.type   = UINT32,
	.name   = "y",
},{
	.type   = UINT32,
	.name   = "width",
},{
	.type   = UINT32,
	.name   = "height",
},{
	.type   = UINT32,
	.name   = "chromakey",
},{
	.type   = UINT32,
	.name   = "flags",
},{
	/* end of list */
}};

struct struct_desc desc_video_buffer[] = {{
	.type   = PTR,
	.name   = "base",
},{
	.type   = SINT32,
	.name   = "height",
},{
	.type   = SINT32,
	.name   = "width",
},{
	.type   = SINT32,
	.name   = "depth",
},{
	.type   = SINT32,
	.name   = "bytesperline",
},{
	/* end of list */
}};

struct struct_desc desc_video_mmap[] = {{
	.type   = UINT32,
	.name   = "frame",
},{
	.type   = SINT32,
	.name   = "height",
},{
	.type   = SINT32,
	.name   = "width",
},{
	.type   = UINT32,
	.name   = "format",
},{
	/* end of list */
}};

struct struct_desc desc_video_mbuf[] = {{
	.type   = SINT32,
	.name   = "size",
},{
	.type   = SINT32,
	.name   = "frames",
},{
	.type   = SINT32,
	.name   = "offsets",
	/* FIXME len=32 */
},{
	/* end of list */
}};

/* ---------------------------------------------------------------------- */

struct ioctl_desc ioctls_v4l1[256] = {
	[_IOC_NR(VIDIOCGCAP)] = {
		.name = "VIDIOCGCAP",
		.desc = desc_video_capability,
	},
	[_IOC_NR(VIDIOCGCHAN)] = {
		.name = "VIDIOCGCHAN",
		.desc = desc_video_channel,
	},
	[_IOC_NR(VIDIOCSCHAN)] = {
		.name = "VIDIOCSCHAN",
		.desc = desc_video_channel,
	},
	[_IOC_NR(VIDIOCGTUNER)] = {
		.name = "VIDIOCGTUNER",
		.desc = desc_video_tuner,
	},
	[_IOC_NR(VIDIOCSTUNER)] = {
		.name = "VIDIOCSTUNER",
		.desc = desc_video_tuner,
	},
	[_IOC_NR(VIDIOCGPICT)] = {
		.name = "VIDIOCGPICT",
		.desc = desc_video_picture,
	},
	[_IOC_NR(VIDIOCSPICT)] = {
		.name = "VIDIOCSPICT",
		.desc = desc_video_picture,
	},
	[_IOC_NR(VIDIOCCAPTURE)] = {
		.name = "VIDIOCCAPTURE",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOCGWIN)] = {
		.name = "VIDIOCGWIN",
		.desc = desc_video_window,
	},
	[_IOC_NR(VIDIOCSWIN)] = {
		.name = "VIDIOCSWIN",
		.desc = desc_video_window,
	},
	[_IOC_NR(VIDIOCGFBUF)] = {
		.name = "VIDIOCGFBUF",
		.desc = desc_video_buffer,
	},
	[_IOC_NR(VIDIOCSFBUF)] = {
		.name = "VIDIOCSFBUF",
		.desc = desc_video_buffer,
	},
	[_IOC_NR(VIDIOCKEY)] = {
		.name = "VIDIOCKEY",
//		.desc = desc_video_key,
	},
	[_IOC_NR(VIDIOCGFREQ)] = {
		.name = "VIDIOCGFREQ",
		.desc = desc_long,
	},
	[_IOC_NR(VIDIOCSFREQ)] = {
		.name = "VIDIOCSFREQ",
		.desc = desc_long,
	},
	[_IOC_NR(VIDIOCGAUDIO)] = {
		.name = "VIDIOCGAUDIO",
		.desc = desc_video_audio,
	},
	[_IOC_NR(VIDIOCSAUDIO)] = {
		.name = "VIDIOCSAUDIO",
		.desc = desc_video_audio,
	},
	[_IOC_NR(VIDIOCSYNC)] = {
		.name = "VIDIOCSYNC",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOCMCAPTURE)] = {
		.name = "VIDIOCMCAPTURE",
		.desc = desc_video_mmap,
	},
	[_IOC_NR(VIDIOCGMBUF)] = {
		.name = "VIDIOCGMBUF",
		.desc = desc_video_mbuf,
	},
	[_IOC_NR(VIDIOCGUNIT)] = {
		.name = "VIDIOCGUNIT",
//		.desc = desc_video_unit,
	},
	[_IOC_NR(VIDIOCGCAPTURE)] = {
		.name = "VIDIOCGCAPTURE",
//		.desc = desc_video_capture,
	},
	[_IOC_NR(VIDIOCSCAPTURE)] = {
		.name = "VIDIOCSCAPTURE",
//		.desc = desc_video_capture,
	},
	[_IOC_NR(VIDIOCSPLAYMODE)] = {
		.name = "VIDIOCSPLAYMODE",
//		.desc = desc_video_play_mode,
	},
	[_IOC_NR(VIDIOCSWRITEMODE)] = {
		.name = "VIDIOCSWRITEMODE",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOCGPLAYINFO)] = {
		.name = "VIDIOCGPLAYINFO",
//		.desc = desc_video_info,
	},
	[_IOC_NR(VIDIOCSMICROCODE)] = {
		.name = "VIDIOCSMICROCODE",
//		.desc = desc_video_code,
	},
	[_IOC_NR(VIDIOCGVBIFMT)] = {
		.name = "VIDIOCGVBIFMT",
//		.desc = desc_vbi_format,
	},
	[_IOC_NR(VIDIOCSVBIFMT)] = {
		.name = "VIDIOCSVBIFMT",
//		.desc = desc_vbi_format,
	},
};

/* ---------------------------------------------------------------------- */
/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */

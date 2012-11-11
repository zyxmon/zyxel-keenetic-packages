#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <asm/types.h>
#include "videodev2.h"

#include "struct-dump.h"
#include "struct-v4l2.h"

/* ---------------------------------------------------------------------- */

char *desc_v4l2_field[] = {
	[V4L2_FIELD_ANY]        = "ANY",
	[V4L2_FIELD_NONE]       = "NONE",
	[V4L2_FIELD_TOP]        = "TOP",
	[V4L2_FIELD_BOTTOM]     = "BOTTOM",
	[V4L2_FIELD_INTERLACED] = "INTERLACED",
	[V4L2_FIELD_SEQ_TB]     = "SEQ_TB",
	[V4L2_FIELD_SEQ_BT]     = "SEQ_BT",
	[V4L2_FIELD_ALTERNATE]  = "ALTERNATE",
};

char *desc_v4l2_buf_type[] = {
	[V4L2_BUF_TYPE_VIDEO_CAPTURE] = "VIDEO_CAPTURE",
	[V4L2_BUF_TYPE_VIDEO_OUTPUT]  = "VIDEO_OUTPUT",
	[V4L2_BUF_TYPE_VIDEO_OVERLAY] = "VIDEO_OVERLAY",
	[V4L2_BUF_TYPE_VBI_CAPTURE]   = "VBI_CAPTURE",
	[V4L2_BUF_TYPE_VBI_OUTPUT]    = "VBI_OUTPUT",
	[V4L2_BUF_TYPE_PRIVATE]       = "PRIVATE",
};

char *desc_v4l2_ctrl_type[] = {
	[V4L2_CTRL_TYPE_INTEGER] = "INTEGER",
	[V4L2_CTRL_TYPE_BOOLEAN] = "BOOLEAN",
	[V4L2_CTRL_TYPE_MENU]    = "MENU",
	[V4L2_CTRL_TYPE_BUTTON]  = "BUTTON",
};

char *desc_v4l2_tuner_type[] = {
	[V4L2_TUNER_RADIO]     = "RADIO",
	[V4L2_TUNER_ANALOG_TV] = "ANALOG_TV",
};

char *desc_v4l2_memory[] = {
	[V4L2_MEMORY_MMAP]    = "MMAP",
	[V4L2_MEMORY_USERPTR] = "USERPTR",
	[V4L2_MEMORY_OVERLAY] = "OVERLAY",
};

char *desc_v4l2_colorspace[] = {
	[V4L2_COLORSPACE_SMPTE170M]     = "SMPTE170M",
	[V4L2_COLORSPACE_SMPTE240M]     = "SMPTE240M",
	[V4L2_COLORSPACE_REC709]        = "REC709",
	[V4L2_COLORSPACE_BT878]         = "BT878",
	[V4L2_COLORSPACE_470_SYSTEM_M]  = "470_SYSTEM_M",
	[V4L2_COLORSPACE_470_SYSTEM_BG] = "470_SYSTEM_BG",
	[V4L2_COLORSPACE_JPEG]          = "JPEG",
	[V4L2_COLORSPACE_SRGB]          = "SRGB",
};

char *bits_capabilities[32] = {
	"VIDEO_CAPTURE", "VIDEO_OUTPUT", "VIDEO_OVERLAY", "",
	"VBI_CAPTURE", "VBI_OUTPUT",   "?","?",
	"RDS_CAPTURE", "?", "?", "?",
	"?", "?", "?", "?",
	"TUNER", "AUDIO", "?", "?",
	"?", "?", "?", "?",
	"READWRITE", "ASYNCIO", "STREAMING", "?",
};

char *bits_standard[64] = {
	"PAL_B",      "PAL_B1",      "PAL_G",   "PAL_H",
	"PAL_I",      "PAL_D",       "PAL_D1",  "PAL_K",
	"PAL_M",      "PAL_N",       "PAL_Nc",  "PAL_60",
	"NTSC_M",     "NTSC_M_JP",   "?", "?",
	"SECAM_B",    "SECAM_D",     "SECAM_G", "SECAM_H",
	"SECAM_K",    "SECAM_K1",    "SECAM_L", "?"
	"ATSC_8_VSB", "ATSC_16_VSB",
};

char *bits_buf_flags[32] = {
	"MAPPED",
	"QUEUED",
	"DONE",
	"KEYFRAME",
	"PFRAME",
	"BFRAME",
	"?", "?",
	"TIMECODE",
};

char *bits_fbuf_cap[32] = {
	"EXTERNOVERLAY",
	"CHROMAKEY",
	"LIST_CLIPPING",
	"BITMAP_CLIPPING",
};

char *bits_fbuf_flags[32] = {
	"PRIMARY",
	"OVERLAY",
	"CHROMAKEY",
};

char *desc_input_type[32] = {
	[ V4L2_INPUT_TYPE_TUNER ]  = "TUNER",
	[ V4L2_INPUT_TYPE_CAMERA ] = "CAMERA",
};

char *bits_input_status[32] = {
	"NO_POWER", "NO_SIGNAL", "NO_COLOR", "?",
	"?","?","?","?",
	"NO_H_LOCK", "COLOR_KILL", "?", "?",
	"?","?","?","?",
	"NO_SYNC", "NO_EQU", "NO_CARRIER", "?",
	"?","?","?","?",
	"MACROVISION", "NO_ACCESS", "VTR", "?",
	"?","?","?","?",
};

char *bits_tuner_cap[32] = {
	"LOW", "NORM", "?", "?",
	"STEREO", "LANG2", "LANG1", "?",
};

char *bits_tuner_rx[32] = {
	"MONO",
	"STEREO",
	"LANG2",
	"LANG1",
};

char *desc_tuner2_mode[] = {
	[ V4L2_TUNER_MODE_MONO ]   = "MONO",
	[ V4L2_TUNER_MODE_STEREO ] = "STEREO",
	[ V4L2_TUNER_MODE_LANG2 ]  = "LANG2",
	[ V4L2_TUNER_MODE_LANG1 ]  = "LANG1",
};

/* ---------------------------------------------------------------------- */

struct struct_desc desc_v4l2_rect[] = {{
	.type   = SINT32,
	.name   = "left",
},{
	.type   = SINT32,
	.name   = "top",
},{
	.type   = SINT32,
	.name   = "width",
},{
	.type   = SINT32,
	.name   = "height",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_fract[] = {{
	.type   = UINT32,
	.name   = "numerator",
},{
	.type   = UINT32,
	.name   = "denominator",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_capability[] = {{
	.type   = STRING,
	.name   = "driver",
	.length = 16,
},{
	.type   = STRING,
	.name   = "card",
	.length = 32,
},{
	.type   = STRING,
	.name   = "bus_info",
	.length = 32,
},{
	.type   = VER,
	.name   = "version",
},{
	.type   = BITS32,
	.name   = "capabilities",
	.bits   = bits_capabilities,
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_pix_format[] = {{
	.type   = UINT32,
	.name   = "width",
},{
	.type   = UINT32,
	.name   = "height",
},{
	.type   = FOURCC,
	.name   = "pixelformat",
},{
	.type   = ENUM32,
	.name   = "field",
	.enums  = desc_v4l2_field,
	.length = sizeof(desc_v4l2_field) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "bytesperline",
},{
	.type   = UINT32,
	.name   = "sizeimage",
},{
	.type   = ENUM32,
	.name   = "colorspace",
	.enums  = desc_v4l2_colorspace,
	.length = sizeof(desc_v4l2_colorspace) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "priv",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_fmtdesc[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "flags",
},{
	.type   = STRING,
	.name   = "description",
	.length = 32,
},{
	.type   = FOURCC,
	.name   = "pixelformat",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_timecode[] = {{
	.type   = UINT32,
	.name   = "type",
},{
	.type   = UINT32,
	.name   = "flags",
},{
	.type   = UINT8,
	.name   = "frames",
},{
	.type   = UINT8,
	.name   = "seconds",
},{
	.type   = UINT8,
	.name   = "minutes",
},{
	.type   = UINT8,
	.name   = "hours",
},{
	.type   = STRING,
	.name   = "userbits",
	.length = 4,
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_compression[] = {{
	.type   = UINT32,
	.name   = "quality",
},{
	.type   = UINT32,
	.name   = "keyframerate",
},{
	.type   = UINT32,
	.name   = "pframerate",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_jpegcompression[] = {{
	.type   = SINT32,
	.name   = "quality",
},{
	.type   = SINT32,
	.name   = "APPn",
},{
	.type   = SINT32,
	.name   = "APP_len",
},{
	.type   = STRING,
	.name   = "APP_data",
	.length = 60,
},{
	.type   = SINT32,
	.name   = "COM_len",
},{
	.type   = STRING,
	.name   = "COM_data",
	.length = 60,
},{
	.type   = UINT32,
	.name   = "jpeg_markers",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_requestbuffers[] = {{
	.type   = UINT32,
	.name   = "count",
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = ENUM32,
	.name   = "memory",
	.enums  = desc_v4l2_memory,
	.length = sizeof(desc_v4l2_memory) / sizeof(char*),
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_buffer[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "bytesused",
},{
	.type   = BITS32,
	.name   = "flags",
	.bits   = bits_buf_flags,
},{
	.type   = ENUM32,
	.name   = "field",
	.enums  = desc_v4l2_field,
	.length = sizeof(desc_v4l2_field) / sizeof(char*),
},{
	.type   = STRUCT,
	.name   = "timestamp",
	.desc   = desc_timeval,
	.length = sizeof(struct timeval),
},{
	.type   = STRUCT,
	.name   = "timecode",
	.desc   = desc_v4l2_timecode,
	.length = sizeof(struct v4l2_timecode),
},{
	.type   = UINT32,
	.name   = "sequence",
},{
	.type   = ENUM32,
	.name   = "memory",
	.enums  = desc_v4l2_memory,
	.length = sizeof(desc_v4l2_memory) / sizeof(char*),
},{
	/* FIXME ... */
	/* end of list */
}};

struct struct_desc desc_v4l2_framebuffer[] = {{
	.type   = BITS32,
	.name   = "capability",
	.bits   = bits_fbuf_cap,
},{
	.type   = BITS32,
	.name   = "flags",
	.bits   = bits_fbuf_flags,
},{
	.type   = PTR,
	.name   = "base",
},{
	.type   = STRUCT,
	.name   = "fmt",
	.desc   = desc_v4l2_pix_format,
	.length = sizeof(struct v4l2_pix_format),
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_clip[] = {{
	.type   = STRUCT,
	.name   = "c",
	.desc   = desc_v4l2_rect,
	.length = sizeof(struct v4l2_rect),
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_window[] = {{
	.type   = STRUCT,
	.name   = "w",
	.desc   = desc_v4l2_rect,
	.length = sizeof(struct v4l2_rect),
},{
	.type   = ENUM32,
	.name   = "field",
	.enums  = desc_v4l2_field,
	.length = sizeof(desc_v4l2_field) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "chromakey",
},{
	.type   = PTR,
	.name   = "clips",
},{
	.type   = UINT32,
	.name   = "clipcount",
},{
	.type   = PTR,
	.name   = "bitmap",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_captureparm[] = {{
	.type   = UINT32,
	.name   = "capability",
},{
	.type   = UINT32,
	.name   = "capturemode",
},{
	.type   = STRUCT,
	.name   = "timeperframe",
	.desc   = desc_v4l2_fract,
	.length = sizeof(struct v4l2_fract),
},{
	.type   = UINT32,
	.name   = "extendedmode",
},{
	.type   = UINT32,
	.name   = "readbuffers",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_outputparm[] = {{
	.type   = UINT32,
	.name   = "capability",
},{
	.type   = UINT32,
	.name   = "outputmode",
},{
	.type   = STRUCT,
	.name   = "timeperframe",
	.desc   = desc_v4l2_fract,
	.length = sizeof(struct v4l2_fract),
},{
	.type   = UINT32,
	.name   = "extendedmode",
},{
	.type   = UINT32,
	.name   = "writebuffers",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_cropcap[] = {{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = STRUCT,
	.name   = "bounds",
	.desc   = desc_v4l2_rect,
	.length = sizeof(struct v4l2_rect),
},{
	.type   = STRUCT,
	.name   = "defrect",
	.desc   = desc_v4l2_rect,
	.length = sizeof(struct v4l2_rect),
},{
	.type   = STRUCT,
	.name   = "pixelaspect",
	.desc   = desc_v4l2_fract,
	.length = sizeof(struct v4l2_fract),
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_crop[] = {{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = STRUCT,
	.name   = "c",
	.desc   = desc_v4l2_rect,
	.length = sizeof(struct v4l2_rect),
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_standard[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = BITS64,
	.name   = "id",
	.bits   = bits_standard,
},{
	.type   = STRING,
	.name   = "name",
	.length = 24,
},{
	.type   = STRUCT,
	.name   = "frameperiod",
	.desc   = desc_v4l2_fract,
	.length = sizeof(struct v4l2_fract),
},{
	.type   = UINT32,
	.name   = "framelines",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_input[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_input_type,
	.length = sizeof(desc_input_type) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "audioset",
},{
	.type   = UINT32,
	.name   = "tuner",
},{
	.type   = BITS64,
	.name   = "std",
	.bits   = bits_standard
},{
	.type   = BITS32,
	.name   = "status",
	.bits   = bits_input_status,
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_output[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = UINT32,
	.name   = "type",
},{
	.type   = UINT32,
	.name   = "audioset",
},{
	.type   = UINT32,
	.name   = "modulator",
},{
	.type   = BITS64,
	.name   = "std",
	.bits   = bits_standard
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_control[] = {{
	.type   = UINT32,
	.name   = "id",
},{
	.type   = SINT32,
	.name   = "value",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_queryctrl[] = {{
	.type   = UINT32,
	.name   = "id",
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_ctrl_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = SINT32,
	.name   = "minimum",
},{
	.type   = SINT32,
	.name   = "maximum",
},{
	.type   = SINT32,
	.name   = "step",
},{
	.type   = SINT32,
	.name   = "default_value",
},{
	.type   = UINT32,
	.name   = "flags",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_querymenu[] = {{
	.type   = UINT32,
	.name   = "id",
},{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = UINT32,
	.name   = "reserved",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_tuner[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_tuner_type,
	.length = sizeof(desc_v4l2_tuner_type) / sizeof(char*),
},{
	.type   = BITS32,
	.name   = "capability",
	.bits   = bits_tuner_cap,
},{
	.type   = UINT32,
	.name   = "rangelow",
},{
	.type   = UINT32,
	.name   = "rangehigh",
},{
	.type   = BITS32,
	.name   = "rxsubchans",
	.bits   = bits_tuner_rx,
},{
	.type   = ENUM32,
	.name   = "audmode",
	.enums  = desc_tuner2_mode,
	.length = sizeof(desc_tuner2_mode) / sizeof(char*),
},{
	.type   = SINT32,
	.name   = "signal",
},{
	.type   = SINT32,
	.name   = "afc",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_modulator[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = UINT32,
	.name   = "capability",
},{
	.type   = UINT32,
	.name   = "rangelow",
},{
	.type   = UINT32,
	.name   = "rangehigh",
},{
	.type   = UINT32,
	.name   = "txsubchans",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_frequency[] = {{
	.type   = UINT32,
	.name   = "tuner",
},{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_tuner_type,
	.length = sizeof(desc_v4l2_tuner_type) / sizeof(char*),
},{
	.type   = UINT32,
	.name   = "frequency",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_audio[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = UINT32,
	.name   = "capability",
},{
	.type   = UINT32,
	.name   = "mode",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_audioout[] = {{
	.type   = UINT32,
	.name   = "index",
},{
	.type   = STRING,
	.name   = "name",
	.length = 32,
},{
	.type   = UINT32,
	.name   = "capability",
},{
	.type   = UINT32,
	.name   = "mode",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_vbi_format[] = {{
	.type   = UINT32,
	.name   = "sampling_rate",
},{
	.type   = UINT32,
	.name   = "offset",
},{
	.type   = UINT32,
	.name   = "samples_per_line",
},{
	.type   = FOURCC,
	.name   = "sample_format",
},{
	.type   = UINT32,
	.name   = "start[0]",
},{
	.type   = UINT32,
	.name   = "start[1]",
},{
	.type   = UINT32,
	.name   = "count[0]",
},{
	.type   = UINT32,
	.name   = "count[1]",
},{
	.type   = UINT32,
	.name   = "flags",
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_format[] = {{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	.type   = UNION,
	.name   = "fmt",
	.u = {{
		.value = V4L2_BUF_TYPE_VIDEO_CAPTURE,
		.name  = "pix",
		.desc  = desc_v4l2_pix_format,
	},{
		.value = V4L2_BUF_TYPE_VIDEO_OVERLAY,
		.name  = "win",
		.desc  = desc_v4l2_window,
	},{
		.value = V4L2_BUF_TYPE_VBI_CAPTURE,
		.name  = "vbi",
		.desc  = desc_v4l2_vbi_format,
	},{
		/* end of list */
	}},
},{
	/* end of list */
}};

struct struct_desc desc_v4l2_streamparm[] = {{
	.type   = ENUM32,
	.name   = "type",
	.enums  = desc_v4l2_buf_type,
	.length = sizeof(desc_v4l2_buf_type) / sizeof(char*),
},{
	/* FIXME ... */
	/* end of list */
}};

struct struct_desc desc_v4l2_std_id[] = {{
	.type   = BITS64,
	.name   = "std",
	.bits   = bits_standard,
},{
	/* end of list */
}};

/* ---------------------------------------------------------------------- */

struct ioctl_desc ioctls_v4l2[256] = {
	[_IOC_NR(VIDIOC_QUERYCAP)] = {
		.name = "VIDIOC_QUERYCAP",
		.desc = desc_v4l2_capability,
	},
	[_IOC_NR(VIDIOC_ENUM_FMT)] = {
		.name = "VIDIOC_ENUM_FMT",
		.desc = desc_v4l2_fmtdesc,
	},
	[_IOC_NR(VIDIOC_G_FMT)] = {
		.name = "VIDIOC_G_FMT",
		.desc = desc_v4l2_format,
	},
	[_IOC_NR(VIDIOC_S_FMT)] = {
		.name = "VIDIOC_S_FMT",
		.desc = desc_v4l2_format,
	},
#if 0
	[_IOC_NR(VIDIOC_G_COMP)] = {
		.name = "VIDIOC_G_COMP",
		.desc = desc_v4l2_compression,
	},
	[_IOC_NR(VIDIOC_S_COMP)] = {
		.name = "VIDIOC_S_COMP",
		.desc = desc_v4l2_compression,
	},
#endif
	[_IOC_NR(VIDIOC_REQBUFS)] = {
		.name = "VIDIOC_REQBUFS",
		.desc = desc_v4l2_requestbuffers,
	},
	[_IOC_NR(VIDIOC_QUERYBUF)] = {
		.name = "VIDIOC_QUERYBUF",
		.desc = desc_v4l2_buffer,
	},
	[_IOC_NR(VIDIOC_G_FBUF)] = {
		.name = "VIDIOC_G_FBUF",
		.desc = desc_v4l2_framebuffer,
	},
	[_IOC_NR(VIDIOC_S_FBUF)] = {
		.name = "VIDIOC_S_FBUF",
		.desc = desc_v4l2_framebuffer,
	},
	[_IOC_NR(VIDIOC_OVERLAY)] = {
		.name = "VIDIOC_OVERLAY",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_QBUF)] = {
		.name = "VIDIOC_QBUF",
		.desc = desc_v4l2_buffer,
	},
	[_IOC_NR(VIDIOC_DQBUF)] = {
		.name = "VIDIOC_DQBUF",
		.desc = desc_v4l2_buffer,
	},
	[_IOC_NR(VIDIOC_STREAMON)] = {
		.name = "VIDIOC_STREAMON",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_STREAMOFF)] = {
		.name = "VIDIOC_STREAMOFF",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_G_PARM)] = {
		.name = "VIDIOC_G_PARM",
		.desc = desc_v4l2_streamparm,
	},
	[_IOC_NR(VIDIOC_S_PARM)] = {
		.name = "VIDIOC_S_PARM",
		.desc = desc_v4l2_streamparm,
	},
	[_IOC_NR(VIDIOC_G_STD)] = {
		.name = "VIDIOC_G_STD",
		.desc = desc_v4l2_std_id,
	},
	[_IOC_NR(VIDIOC_S_STD)] = {
		.name = "VIDIOC_S_STD",
		.desc = desc_v4l2_std_id,
	},
	[_IOC_NR(VIDIOC_ENUMSTD)] = {
		.name = "VIDIOC_ENUMSTD",
		.desc = desc_v4l2_standard,
	},
	[_IOC_NR(VIDIOC_ENUMINPUT)] = {
		.name = "VIDIOC_ENUMINPUT",
		.desc = desc_v4l2_input,
	},
	[_IOC_NR(VIDIOC_G_CTRL)] = {
		.name = "VIDIOC_G_CTRL",
		.desc = desc_v4l2_control,
	},
	[_IOC_NR(VIDIOC_S_CTRL)] = {
		.name = "VIDIOC_S_CTRL",
		.desc = desc_v4l2_control,
	},
	[_IOC_NR(VIDIOC_G_TUNER)] = {
		.name = "VIDIOC_G_TUNER",
		.desc = desc_v4l2_tuner,
	},
	[_IOC_NR(VIDIOC_S_TUNER)] = {
		.name = "VIDIOC_S_TUNER",
		.desc = desc_v4l2_tuner,
	},
	[_IOC_NR(VIDIOC_G_AUDIO)] = {
		.name = "VIDIOC_G_AUDIO",
		.desc = desc_v4l2_audio,
	},
	[_IOC_NR(VIDIOC_S_AUDIO)] = {
		.name = "VIDIOC_S_AUDIO",
		.desc = desc_v4l2_audio,
	},
	[_IOC_NR(VIDIOC_QUERYCTRL)] = {
		.name = "VIDIOC_QUERYCTRL",
		.desc = desc_v4l2_queryctrl,
	},
	[_IOC_NR(VIDIOC_QUERYMENU)] = {
		.name = "VIDIOC_QUERYMENU",
		.desc = desc_v4l2_querymenu,
	},
	[_IOC_NR(VIDIOC_G_INPUT)] = {
		.name = "VIDIOC_G_INPUT",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_S_INPUT)] = {
		.name = "VIDIOC_S_INPUT",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_G_OUTPUT)] = {
		.name = "VIDIOC_G_OUTPUT",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_S_OUTPUT)] = {
		.name = "VIDIOC_S_OUTPUT",
		.desc = desc_int,
	},
	[_IOC_NR(VIDIOC_ENUMOUTPUT)] = {
		.name = "VIDIOC_ENUMOUTPUT",
		.desc = desc_v4l2_output,
	},
	[_IOC_NR(VIDIOC_G_AUDOUT)] = {
		.name = "VIDIOC_G_AUDOUT",
		.desc = desc_v4l2_audioout,
	},
	[_IOC_NR(VIDIOC_S_AUDOUT)] = {
		.name = "VIDIOC_S_AUDOUT",
		.desc = desc_v4l2_audioout,
	},
	[_IOC_NR(VIDIOC_G_MODULATOR)] = {
		.name = "VIDIOC_G_MODULATOR",
		.desc = desc_v4l2_modulator,
	},
	[_IOC_NR(VIDIOC_S_MODULATOR)] = {
		.name = "VIDIOC_S_MODULATOR",
		.desc = desc_v4l2_modulator,
	},
	[_IOC_NR(VIDIOC_G_FREQUENCY)] = {
		.name = "VIDIOC_G_FREQUENCY",
		.desc = desc_v4l2_frequency,
	},
	[_IOC_NR(VIDIOC_S_FREQUENCY)] = {
		.name = "VIDIOC_S_FREQUENCY",
		.desc = desc_v4l2_frequency,
	},
	[_IOC_NR(VIDIOC_CROPCAP)] = {
		.name = "VIDIOC_CROPCAP",
		.desc = desc_v4l2_cropcap,
	},
	[_IOC_NR(VIDIOC_G_CROP)] = {
		.name = "VIDIOC_G_CROP",
		.desc = desc_v4l2_crop,
	},
	[_IOC_NR(VIDIOC_S_CROP)] = {
		.name = "VIDIOC_S_CROP",
		.desc = desc_v4l2_crop,
	},
	[_IOC_NR(VIDIOC_G_JPEGCOMP)] = {
		.name = "VIDIOC_G_JPEGCOMP",
		.desc = desc_v4l2_jpegcompression,
	},
	[_IOC_NR(VIDIOC_S_JPEGCOMP)] = {
		.name = "VIDIOC_S_JPEGCOMP",
		.desc = desc_v4l2_jpegcompression,
	},
	[_IOC_NR(VIDIOC_QUERYSTD)] = {
		.name = "VIDIOC_QUERYSTD",
		.desc = desc_v4l2_std_id,
	},
	[_IOC_NR(VIDIOC_TRY_FMT)] = {
		.name = "VIDIOC_TRY_FMT",
		.desc = desc_v4l2_format,
	},
};

/* ---------------------------------------------------------------------- */
/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */

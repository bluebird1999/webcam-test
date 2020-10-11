/*
 * config_video_interface.h
 *
 *  Created on: Sep 1, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_VIDEO_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_VIDEO_INTERFACE_H_

/*
 * header
 */
#include <rtscamkit.h>
#include <rtsavapi.h>
#include <rtsvideo.h>

#include "../../manager/global_interface.h"

/*
 * define
 */
#define		CONFIG_VIDEO_MODULE_NUM			7

#define		CONFIG_VIDEO_PROFILE			0
#define		CONFIG_VIDEO_ISP				1
#define		CONFIG_VIDEO_H264				2
#define		CONFIG_VIDEO_OSD				3
#define		CONFIG_VIDEO_3ACTRL				4
#define		CONFIG_VIDEO_JPG				5
#define		CONFIG_VIDEO_MD					6

#define		AE_AUTO_MODE_NONE				0
#define		AE_AUTO_MODE_TARGET_DELTA		1
#define		AE_AUTO_MODE_GAIN_MAX			2
#define		AE_AUTO_MODE_MIN_FPS			3
#define		AE_AUTO_MODE_WEIGHT				4
#define		AE_MANUAL_MODE_TOTAL_GAIN		5
#define		AE_MANUAL_MODE_GAIN				6
#define		AE_MANUAL_MODE_EXPOSURE_TIME 	7

#define		OSD_FONT_PATH					"/opt/qcy/font/"
#define		JPG_LIBRARY_PATH				"/mnt/media/"
#define		MP4_LIBRARY_PATH				"/mnt/media/"

/*
 * structure
 */
typedef struct isp_awb_para_t {
	int		awb_mode;
	int		awb_temperature;
	int		awb_rgain;
	int 	awb_bgain;
	int		awb_r;
	int		awb_g;
	int		awb_b;
} isp_awb_para_t;

typedef struct isp_af_para_t {
	int		af_window_size;
	int		af_window_num;
} isp_af_para_t;

typedef struct isp_ae_para_t {
	int		ae_mode;
	int 	ae_target_delta;
	int		ae_gain_max;
	int		ae_min_fps;
	int		ae_weight;
	int		ae_total_gain;
	int		ae_analog;
	int		ae_digital;
	int		ae_isp_digital;
	int		ae_exposure_time;
} isp_ae_para_t;

/*
 *
 */
typedef struct video_profile_config_t {
	int						run_mode;
	int						quality;
	struct rts_av_profile	profile[3];
} video_profile_config_t;

typedef struct video_isp_config_t {
	struct rts_isp_attr		isp_attr;
	int						awb_ctrl;
	int						af;
	int						exposure_mode;
	int						pan;
	int						tilt;
	int						mirror;
	int						flip;
	int						wdr_mode;
	int						wdr_level;
	int						ir_mode;
	int						smart_ir_mode;
	int						smart_ir_manual_level;
	int						ldc;
	int						noise_reduction;
	int						in_out_door_mode;
	int						detail_enhancement;
}video_isp_config_t;

typedef struct video_h264_config_t {
	struct rts_h264_attr		h264_attr;
	struct rts_video_h264_ctrl	h264_ctrl;
} video_h264_config_t;

typedef struct video_osd_config_t {
	int		enable;
	int		time_mode;
	int		time_rotate;
	char	time_font_face[MAX_SYSTEM_STRING_SIZE];
	int 	time_pixel_size;
	int		time_alpha;
	int		time_color;
	int		time_pos_mode;
	int		time_offset;
	int		time_flick;
	int		time_flick_on;
	int		time_flick_off;
	int		warning_mode;
	int		warning_rotate;
	char	warning_font_face[MAX_SYSTEM_STRING_SIZE];
	int 	warning_pixel_size;
	int		warning_alpha;
	int		warning_color;
	int		warning_pos_mode;
	int		warning_offset;
	int		warning_flick;
	int		warning_flick_on;
	int		warning_flick_off;
	int		label_mode;
	int		label_rotate;
	char	label_font_face[MAX_SYSTEM_STRING_SIZE];
	int 	label_pixel_size;
	int		label_alpha;
	int		label_color;
	int		label_pos_mode;
	int		label_offset;
	int		label_flick;
	int		label_flick_on;
	int		label_flick_off;
} video_osd_config_t;

typedef struct video_3actrl_config_t {
	isp_awb_para_t	awb_para;
	isp_af_para_t	af_para;
	isp_ae_para_t	ae_para;
} video_3actrl_config_t;

typedef struct video_jpg_config_t {
	int							enable;
	struct rts_jpgenc_attr		jpg_ctrl;
} video_jpg_config_t;

typedef struct video_md_config_t {
	int 	enable;
	int		polling;
	int		trig;
	int		cloud_report;
	int		alarm_interval;
	int		sensitivity;
	char	start[MAX_SYSTEM_STRING_SIZE];
	char	end[MAX_SYSTEM_STRING_SIZE];
} video_md_config_t;

typedef struct video_config_t {
	int							status;
	video_profile_config_t		profile;
	video_isp_config_t			isp;
	video_h264_config_t			h264;
	video_osd_config_t 			osd;
	video_3actrl_config_t		a3ctrl;
	video_jpg_config_t			jpg;
	video_md_config_t			md;
} video_config_t;

/*
 * function
 */

#endif /* SERVER_CONFIG_CONFIG_VIDEO_INTERFACE_H_ */

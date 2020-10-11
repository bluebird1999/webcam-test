/*
 * config_video.h
 *
 *  Created on: Sep 1, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_VIDEO_H_
#define SERVER_CONFIG_CONFIG_VIDEO_H_

/*
 * header
 */

/*
 * define
 */
#define 	CONFIG_VIDEO_PROFILE_PATH				"/opt/qcy/config/video_profile.config"
#define 	CONFIG_VIDEO_ISP_PATH					"/opt/qcy/config/video_isp.config"
#define 	CONFIG_VIDEO_H264_PATH					"/opt/qcy/config/video_h264.config"
#define 	CONFIG_VIDEO_OSD_PATH					"/opt/qcy/config/video_osd.config"
#define 	CONFIG_VIDEO_3ACTRL_PATH				"/opt/qcy/config/video_3actrl.config"
#define 	CONFIG_VIDEO_JPG_PATH					"/opt/qcy/config/video_jpg.config"
#define 	CONFIG_VIDEO_MD_PATH					"/opt/qcy/config/video_md.config"

/*
 * structure
 */

/*
 * function
 */
int config_video_read(void);
int config_video_get(void **t, int *size);
int config_video_set(int module, void *t);
int config_video_get_config_status(int module);

#endif /* SERVER_CONFIG_CONFIG_VIDEO_H_ */

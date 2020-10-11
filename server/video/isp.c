/*
 * isp.c
 *
 *  Created on: Sep 8, 2020
 *      Author: ning
 */

/*
 * header
 */
//system header
#include <stdio.h>
#include <stdlib.h>
#include <rtscamkit.h>
#include <rtsavapi.h>
#include <rtsvideo.h>
//program header
#include "../../tools/tools_interface.h"
//server header
#include "isp.h"


/*
 * static
 */
//variable

//function
static int isp_get_valid_value(int id, int value, struct rts_video_control *ctrl);

/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

static int isp_get_valid_value(int id, int value, struct rts_video_control *ctrl)
{
	int tvalue = value;
	if (value < ctrl->minimum)
		tvalue = ctrl->minimum;
	if (value > ctrl->maximum)
		tvalue = ctrl->maximum;
	if ((value - ctrl->minimum) % ctrl->step)
		tvalue = value - (value - ctrl->minimum) % ctrl->step;
	return tvalue;
}

/*
 * interface
 */
int isp_get_attr(unsigned int id)
{
	struct rts_video_control ctrl;
	int ret;
	ret = rts_av_get_isp_ctrl(id, &ctrl);
	if (ret) {
		log_err("get isp attr fail, ret = %d\n", ret);
		return ret;
	}
	log_info("%s min = %d, max = %d, step = %d, default = %d, cur = %d\n",
			 ctrl.name, ctrl.minimum, ctrl.maximum,
			 ctrl.step, ctrl.default_value, ctrl.current_value);
	return 0;
}

int isp_set_attr(unsigned int id, int value)
{
	struct rts_video_control ctrl;
	int ret;
	ret = rts_av_get_isp_ctrl(id, &ctrl);
	if (ret) {
		log_err("get isp attr fail, ret = %d\n", ret);
		return ret;
	}
	value = isp_get_valid_value(id, value, &ctrl);
	log_info("%s min = %d, max = %d, step = %d, default = %d, cur = %d\n",
			 ctrl.name, ctrl.minimum, ctrl.maximum,
			 ctrl.step, ctrl.default_value, ctrl.current_value);
	ctrl.current_value = value;
	ret = rts_av_set_isp_ctrl(id, &ctrl);
	if (ret) {
		log_err("set isp attr fail, ret = %d\n", ret);
		return ret;
	}
	ret = rts_av_get_isp_ctrl(id, &ctrl);
	if (ret) {
		log_err("get isp attr fail, ret = %d\n", ret);
		return ret;
	}
	log_info("%s min = %d, max = %d, step = %d, default = %d, cur = %d\n",
			 ctrl.name, ctrl.minimum, ctrl.maximum,
			 ctrl.step, ctrl.default_value, ctrl.current_value);
	return 0;
}

int isp_init(video_isp_config_t *ctrl)
{
	int ret = 0;
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_AWB_CTRL, ctrl->awb_ctrl);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_AF, ctrl->af);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_EXPOSURE_MODE, ctrl->exposure_mode);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_PAN, ctrl->pan);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_TILT, ctrl->tilt);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_MIRROR, ctrl->mirror);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_FLIP, ctrl->flip);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_WDR_MODE, ctrl->wdr_mode);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_WDR_LEVEL, ctrl->wdr_level);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_IR_MODE, ctrl->ir_mode);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_SMART_IR_MODE, ctrl->smart_ir_mode);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_SMART_IR_MANUAL_LEVEL, ctrl->smart_ir_manual_level);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_LDC, ctrl->ldc);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_NOISE_REDUCTION, ctrl->noise_reduction);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_IN_OUT_DOOR_MODE, ctrl->in_out_door_mode);
	ret |= isp_set_attr(RTS_VIDEO_CTRL_ID_DETAIL_ENHANCEMENT, ctrl->detail_enhancement);
	return ret;
}


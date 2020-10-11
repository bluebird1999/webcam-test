/*
 * focus.c
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
#include "focus.h"


/*
 * static
 */
//variable
static int last_frame;
struct rts_isp_af_ctrl *af;

//function
static int focus_set(int ws, int wn);

/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

static int focus_set(int ws, int wn)
{
	return 0;
}

/*
 * interface
 */
int focus_proc(isp_af_para_t *ctrl, int frame)
{
	int ret=0;
	struct rts_isp_awb_gain ct_gain;
	if( (frame - last_frame) > AF_FRAME_INTERVAL ) {
        ret = rts_av_query_isp_af(&af);
        if (ret) {
        	log_err("query isp af ctrl fail, ret = %d\n", ret);
        	RTS_SAFE_RELEASE(af, rts_av_release_isp_af);
        	return 0;
        }
		last_frame = frame;
	}
	return ret;
}

int focus_init(isp_af_para_t *ctrl)
{
	int ret=0;
    last_frame = 0;
    ret = rts_av_query_isp_af(&af);
    if (ret) {
    	log_err("query isp af ctrl fail, ret = %d\n", ret);
    	RTS_SAFE_RELEASE(af, rts_av_release_isp_af);
        return -1;
    }
	return ret;
}

int focus_release(void)
{
	int ret=0;
    last_frame = 0;
	RTS_SAFE_RELEASE(af, rts_av_release_isp_af);
	return ret;
}



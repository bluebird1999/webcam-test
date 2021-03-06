/*
 * miio_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_MIIO_MIIO_INTERFACE_H_
#define SERVER_MIIO_MIIO_INTERFACE_H_

/*
 * header
 */
#include "../../manager/manager_interface.h"

#define		MSG_DEVICE_BASE						(SERVER_DEVICE<<16)
#define		MSG_DEVICE_SIGINT					MSG_DEVICE_BASE | 0x0000
#define		MSG_DEVICE_SIGINT_ACK				MSG_DEVICE_BASE | 0x1000
#define		MSG_DEVICE_GET_PARA					MSG_DEVICE_BASE | 0x0010
#define		MSG_DEVICE_GET_PARA_ACK				MSG_DEVICE_BASE | 0x1010
#define		MSG_DEVICE_SET_PARA					MSG_DEVICE_BASE | 0x0011
#define		MSG_DEVICE_SET_PARA_ACK				MSG_DEVICE_BASE | 0x1011
#define		MSG_DEVICE_CTRL_DIRECT				MSG_DEVICE_BASE | 0x0012
#define		MSG_DEVICE_CTRL_DIRECT_ACK			MSG_DEVICE_BASE | 0x1012
#define		MSG_DEVICE_ACTION					MSG_DEVICE_BASE | 0x0020
#define		MSG_DEVICE_ACTION_ACK				MSG_DEVICE_BASE | 0x1020

#define		MSG_KERNEL_BASE						(SERVER_KERNEL<<16)
#define		MSG_KERNEL_SIGINT					MSG_KERNEL_BASE | 0x0000
#define		MSG_KERNEL_SIGINT_ACK				MSG_KERNEL_BASE | 0x1000
#define		MSG_KERNEL_GET_PARA					MSG_KERNEL_BASE | 0x0010
#define		MSG_KERNEL_GET_PARA_ACK				MSG_KERNEL_BASE | 0x1010
#define		MSG_KERNEL_SET_PARA					MSG_KERNEL_BASE | 0x0011
#define		MSG_KERNEL_SET_PARA_ACK				MSG_KERNEL_BASE | 0x1011
#define		MSG_KERNEL_CTRL_DIRECT				MSG_KERNEL_BASE | 0x0012
#define		MSG_KERNEL_CTRL_DIRECT_ACK			MSG_KERNEL_BASE | 0x1012
#define		MSG_KERNEL_ACTION					MSG_KERNEL_BASE | 0x0020
#define		MSG_KERNEL_ACTION_ACK				MSG_KERNEL_BASE | 0x1020
#define		MSG_KERNEL_OTA_DOWNLOAD				MSG_KERNEL_BASE | 0x0030
#define		MSG_KERNEL_OTA_DOWNLOAD_ACK			MSG_KERNEL_BASE | 0x1030
#define		MSG_KERNEL_OTA_REPORT				MSG_KERNEL_BASE | 0x0031
#define		MSG_KERNEL_OTA_REPORT_ACK			MSG_KERNEL_BASE | 0x1031
#define		MSG_KERNEL_OTA_REQUEST				MSG_KERNEL_BASE | 0x0032
#define		MSG_KERNEL_OTA_REQUEST_ACK			MSG_KERNEL_BASE | 0x1032

#define		MSG_MICLOUD_BASE					(SERVER_MICLOUD<<16)
#define		MSG_MICLOUD_SIGINT					MSG_MICLOUD_BASE | 0x0000
#define		MSG_MICLOUD_SIGINT_ACK				MSG_MICLOUD_BASE | 0x1000
#define		MSG_MICLOUD_GET_PARA				MSG_MICLOUD_BASE | 0x0010
#define		MSG_MICLOUD_GET_PARA_ACK			MSG_MICLOUD_BASE | 0x1010
#define		MSG_MICLOUD_SET_PARA				MSG_MICLOUD_BASE | 0x0011
#define		MSG_MICLOUD_SET_PARA_ACK			MSG_MICLOUD_BASE | 0x1011
#define		MSG_MICLOUD_CTRL_DIRECT				MSG_MICLOUD_BASE | 0x0012
#define		MSG_MICLOUD_CTRL_DIRECT_ACK			MSG_MICLOUD_BASE | 0x1012
#define		MSG_MICLOUD_VIDEO_DATA				MSG_MICLOUD_BASE | 0x0100
#define		MSG_MICLOUD_AUDIO_DATA				MSG_MICLOUD_BASE | 0x0101
#define		MSG_MICLOUD_WARNING_NOTICE			MSG_MICLOUD_BASE | 0x0102

#define		MSG_RECORDER_BASE					(SERVER_RECORDER<<16)
#define		MSG_RECORDER_SIGINT					MSG_RECORDER_BASE | 0x0000
#define		MSG_RECORDER_SIGINT_ACK				MSG_RECORDER_BASE | 0x1000
#define		MSG_RECORDER_GET_PARA				MSG_RECORDER_BASE | 0x0010
#define		MSG_RECORDER_GET_PARA_ACK			MSG_RECORDER_BASE | 0x1010
#define		MSG_RECORDER_SET_PARA				MSG_RECORDER_BASE | 0x0011
#define		MSG_RECORDER_SET_PARA_ACK			MSG_RECORDER_BASE | 0x1011
#define		MSG_RECORDER_CTRL_DIRECT			MSG_RECORDER_BASE | 0x0012
#define		MSG_RECORDER_CTRL_DIRECT_ACK		MSG_RECORDER_BASE | 0x1012
#define		MSG_RECORDER_VIDEO_DATA				MSG_RECORDER_BASE | 0x0100
#define		MSG_RECORDER_AUDIO_DATA				MSG_RECORDER_BASE | 0x0101

#define		MSG_SPEAKER_BASE					(SERVER_SPEAKER<<16)
#define		MSG_SPEAKER_SIGINT					MSG_SPEAKER_BASE | 0x0000
#define		MSG_SPEAKER_SIGINT_ACK				MSG_SPEAKER_BASE | 0x1000
#define		MSG_SPEAKER_AUDIO_DATA				MSG_SPEAKER_BASE | 0x0100

#define		DEVICE_CTRL_SD_STATUS					0x0000
#define 	DEVICE_CTRL_SD_SPACE					0x0001
#define 	DEVICE_CTRL_SD_FREE     	          	0x0002
#define 	DEVICE_CTRL_SD_USDED	    	       	0x0003
#define 	DEVICE_CTRL_IR_SWITCH	          		0x0010
#define 	DEVICE_CTRL_IR_MODE	          			0x0011
#define 	DEVICE_CTRL_NET_STATUS		 			0x0020
#define		DEVICE_CTRL_NET_MAC						0x0021
#define		DEVICE_CTRL_NET_IP						0x0022
#define		DEVICE_CTRL_NET_NETMASK					0x0023
#define 	DEVICE_CTRL_NET_GATEWAY	    			0x0024
#define 	DEVICE_CTRL_WIFI_STATUS		 			0x0030
#define		DEVICE_CTRL_WIFI_NAME					0x0031
#define		DEVICE_CTRL_WIFI_RSSI					0x0032
#define		DEVICE_CTRL_INDICATOR_SWITCH			0x0040
#define 	DEVICE_CTRL_MOTOR_HOR_STATUS          	0x0050
#define 	DEVICE_CTRL_MOTOR_HOR_POS	          	0x0051
#define 	DEVICE_CTRL_MOTOR_HOR_SPEED          	0x0052
#define 	DEVICE_CTRL_MOTOR_VER_STATUS		    0x0053
#define 	DEVICE_CTRL_MOTOR_VER_POS	          	0x0054
#define 	DEVICE_CTRL_MOTOR_VER_SPEED          	0x0055
#define 	DEVICE_CTRL_SPEAKER_SWITCH	         	0x0060

#define 	DEVICE_ACTION_SD_FORMAT	    	       	0x0000
#define 	DEVICE_ACTION_SD_POPUP	    	       	0x0001

#define		KERNEL_CTRL_TIMEZONE					0x0000
#define		KERNEL_ACTION_REBOOT					0x0000

#define		MICLOUD_CTRL_UPLOAD_SWITCH				0x0000
#define 	MICLOUD_CTRL_WARNING_PUSH_SWITCH		0x0001

enum{
    NOT_SD_CARD = 0,
    SD_CARD_OK,
    SD_CARD_READONLY,
    SD_CARD_POPUP,
    SD_CARD_FORMATING,
};

/*
 * define
 */
#define		SERVER_MIIO_VERSION_STRING			"alpha-3.1"

#define		MSG_MIIO_BASE						(SERVER_MIIO<<16)
#define		MSG_MIIO_SIGINT						MSG_MIIO_BASE | 0x0000
#define		MSG_MIIO_SIGINT_ACK					MSG_MIIO_BASE | 0x0000
#define		MSG_MIIO_CLOUD_TRYING				MSG_MIIO_BASE | 0x0010
#define		MSG_MIIO_CLOUD_CONNECTED			MSG_MIIO_BASE | 0x0011
#define		MSG_MIIO_MISSRPC_ERROR				MSG_MIIO_BASE | 0x0012
#define		MSG_MIIO_RPC_SEND					MSG_MIIO_BASE | 0x0020
#define		MSG_MIIO_RPC_SEND_ACK				MSG_MIIO_BASE | 0x1020
#define		MSG_MIIO_RPC_REPORT_SEND			MSG_MIIO_BASE | 0x0021
#define		MSG_MIIO_RPC_REPORT_SEND_ACK		MSG_MIIO_BASE | 0x1021
#define		MSG_MIIO_SOCKET_SEND				MSG_MIIO_BASE | 0x0022
#define		MSG_MIIO_SOCKET_SEND_ACK			MSG_MIIO_BASE | 0x1022

#define		OTA_TYPE_UBOOT							0x00
#define		OTA_TYPE_KERNEL							0x01
#define		OTA_TYPE_ROOTFS							0x02
#define		OTA_TYPE_APPLICATION					0x03
#define		OTA_TYPE_MIIO_CLIENT					0x04
#define		OTA_TYPE_CONFIG							0x05
#define		OTA_TYPE_LIB							0x06
/*
 * structure
 */
typedef enum
{
	OTA_PROC_DNLD = 1,
	OTA_PROC_INSTALL,
	OTA_PROC_DNLD_INSTALL,
}OTA_PROC;

typedef enum
{
	OTA_MODE_SILENT = 1,
	OTA_MODE_NORMAL,
}OTA_MODE;

typedef enum
{
	OTA_STATE_IDLE = 0,
	OTA_STATE_DOWNLOADING,
	OTA_STATE_DOWNLOADED,
	OTA_STATE_INSTALLING,
	OTA_STATE_WAIT_INSTALL,
	OTA_STATE_INSTALLED,
	OTA_STATE_FAILED,
	OTA_STATE_BUSY,
}OTA_STATE;

typedef enum
{
	OTA_ERR_NONE = 0,       //无错误
	OTA_ERR_DOWN_ERR,       //下载失败，设备侧无法提供失败的原因，一般是网络连接失败
	OTA_ERR_DNS_ERR,        //dns解析失败
	OTA_ERR_CONNECT_ERR,    //连接下载服务器失败
	OTA_ERR_DICONNECT,      //下载过程中连接中断
	OTA_ERR_INSTALL_ERR,    //安装错误，下载已经完成，但是安装的时候报错
	OTA_ERR_CANCEL,         //设备侧取消下载
	OTA_ERR_LOW_ENERGY,     //电量低，终止下载
	OTA_ERR_UNKNOWN,        //未知原因
};

/*
 * function
 */
int server_miio_start(void);
int server_miio_message(message_t *msg);


#endif /* SERVER_MIIO_MIIO_INTERFACE_H_ */

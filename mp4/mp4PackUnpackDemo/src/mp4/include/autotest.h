
#ifndef _AUTOTEST_H_
#define _AUTOTEST_H_

#include "public_def.h"
#include "td_types.h"

#define DEV_USB_RATH "/dev/usb"
#define GUI_CONFIG_PATH	"/dvr/gui_config/guide.cnf"

#define TESTSUCCESS 0
#define TESTFAILED	1
#define TESTNOTSUPPORT 	2
#define TESTTIMEOUT		3

#define DVR_TEST_SIGN	"/dvr/testsign.txt"


//设置工作模式
td_s32 AutoTest_SetWorkMode(td_s32 _s32WorkeMode);

td_s32 AutoTest_Start();

td_s32 AutoTest_SATAFormat();

td_s32 AutoTest_USBDisk();

td_s32 AutoTest_USBMouse();

td_s32 AutoTest_USBCount();

td_s32 AutoTest_SataCheck(td_s32 _s32SATANum);

td_s32 AutoTest_SdCardCheck(td_s32 _s32SdCardNum);

td_s32 AutoTest_I2cCheck();

td_s32 AutoTest_HDMICheck();

td_s32 AutoTest_VgaCheck();

td_s32 AutoTest_BackUp();

td_s32 AutoTest_ClearLog();

td_s32 AutoTest_ChangeGuide();

td_s32 AutoTest_AlarmIn(td_s32 _s32AlarmInPort);

td_s32 AutoTest_AlarmOut(td_s32 _s32AlarmOutPort, td_s32 _s32OutStat);

td_s32 AutoTest_Lan(td_s32 _s32LanNum);

td_s32 AutoTest_ReadIfconfig(td_s32 _s32LanNum, td_s32 *_s32RXReadNum, td_s32 *_s32TXReadNum);










#endif


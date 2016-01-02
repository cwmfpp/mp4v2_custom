#ifndef _LOG_TEMPLATE_H_
#define _LOG_TEMPLATE_H_

//system
#define WRITE_LOG_SysStart(socket) log_Write(socket,LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 1, NULL)
#define WRITE_LOG_SysRebootByGui(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 2, NULL)
#define WRITE_LOG_SysRebootByReset(socket) log_Write(socket,LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 3, NULL)
#define WRITE_LOG_RebootByRevSignal(socket,i) log_Write(socket,LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 4, (td_u8 *)"%d", i)
#define WRITE_LOG_RemoteReboot(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 5, NULL)
#define WRITE_LOG_RemoteLoad(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 6, NULL)
#define WRITE_LOG_RemoteLoadProWebPage(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 7, NULL)
#define WRITE_LOG_TCPInitFail(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 8, NULL)
#define WRITE_LOG_UpdataAppSucc(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 9, NULL)
#define WRITE_LOG_LostHeartBeatFromGuiRestart(socket, s) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 10, (td_u8 *)"%s", s)
#define WRITE_LOG_ProxyConnSetSockoptErr(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 11, NULL)
#define WRITE_LOG_ActiveClientHeartBeatErr(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 12, NULL)
#define WRITE_LOG_PthreadTcpClientRecvConntsThisIsNULL(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 13, NULL)
#define WRITE_LOG_TcpServer_CopyDataToSocketBuftsThisIsNULL(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 14, NULL)
#define WRITE_LOG_StartCheckDiskPartition(socket,i,j) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 15, (td_u8 *)"%d\t%d", i, j)
#define WRITE_LOG_FinishCheckDiskPartition(socket,i,j) log_Write(socket,LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 16, (td_u8 *)"%d\t%d", i, j)
#define WRITE_LOG_StartedFixPartition(socket,s) log_Write(socket,LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 17, (td_u8 *)"%s", s)
#define WRITE_LOG_FinishFixPartition(socket,s) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 18, (td_u8 *)"%s", s)
#define WRITE_LOG_SlaveRestart(socket,i) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 19, (td_u8 *)"%d", i)
#define WRITE_LOG_RebootForTooHighLoad(socket,i) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 20, (td_u8 *)"%d", i)
#define WRITE_LOG_ShutDownLocally(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 21, NULL)
#define WRITE_LOG_RebootForAudioEncErr(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 22, NULL)
#define WRITE_LOG_RebootForVideoEncErr(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 23, NULL)
#define WRITE_LOG_RebootForSlaveDown(socket,i) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 24, (td_u8 *)"%d", i)
#define WRITE_LOG_RemoteRebootChangeIP(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 25, NULL)
#define WRITE_LOG_RemoteRebootChangeMac(socket) log_Write(socket,LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 26, NULL)
#define WRITE_LOG_RemoteRebootChangePort(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 27, NULL)
#define WRITE_LOG_RemoteRebootManageClient(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 28, NULL)
#define WRITE_LOG_ModifyVideoEncrypt(socket) log_Write(socket, LOG_LEVEL_SYSTEM,CHANNEL_INVALID,29,NULL)
#define WRITE_LOG_ModifyLanguage(socket) log_Write(socket, LOG_LEVEL_SYSTEM,CHANNEL_INVALID,30, NULL)
#define WRITE_LOG_DiskSleep(socket,i) log_Write(socket, LOG_LEVEL_SYSTEM,CHANNEL_INVALID,31, (td_u8*)"%d", i)
#define WRITE_LOG_DiskWakeUp(socket,i) log_Write(socket,LOG_LEVEL_SYSTEM,CHANNEL_INVALID,32, (td_u8*)"%d", i)
#define WRITE_LOG_DiskErrReboot(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 33, NULL)
#define WRITE_LOG_RemoteShutDown(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 34, NULL)
#define WRITE_LOG_CriticalUnusualPowerVoltage(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 35, NULL)
#define WRITE_LOG_ChnHeartLost(socket, s) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 36, (td_u8 *)"%s", s)
#define WRITE_LOG_SysAutoMaintain(socket) log_Write(socket, LOG_LEVEL_SYSTEM, CHANNEL_INVALID, 37, NULL)

//warning
#define WRITE_LOG_RemoteLogonErr(socket,s) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1001, (td_u8 *)"%s", s)
#define WRITE_LOG_NotFindUsableDisk(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1002, NULL)
#define WRITE_LOG_NotRebundanceDisk(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1011, NULL)
#define WRITE_LOG_WriteDiskErr(socket,d,s) log_Write(socket,LOG_LEVEL_WARNING, CHANNEL_INVALID, 1003, (td_u8 *)"%d\t%s", d, s)
#define WRITE_LOG_DiskFullStopRecord(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1004, NULL)
//#define WRITE_LOG_ConflictIPAddress(s) log_Write(LOG_LEVEL_WARNING, CHANNEL_INVALID, 1005, "%s", s)
#define WRITE_LOG_ConflictIPAddress(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1005, NULL)
#define WRITE_LOG_EthernetIsDown(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1006, NULL)
#define WRITE_LOG_EthernetIsUp(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1007, NULL)
#define WRITE_LOG_SndConflictIPAddress(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1008, NULL)
#define WRITE_LOG_SndEthernetIsDown(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1009, NULL)
#define WRITE_LOG_SndEthernetIsUp(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1010, NULL)
#define WRITE_LOG_UnusualPowerVoltage(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1012, NULL)
#define WRITE_LOG_BackUsualPowerVoltage(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID, 1013, NULL)
#define WRITE_LOG_ConflictMACAddress(socket,d,s) log_Write(socket,LOG_LEVEL_WARNING, CHANNEL_INVALID, 1014, (td_u8 *)"%d\t%s", d, s)

//alarm
#define WRITE_LOG_VideoLostAlarmHapp(socket, i) log_Write(socket,LOG_LEVEL_ALARM, i, 2001, NULL)
#define WRITE_LOG_VideoLostAlarmNoHapp(socket, i) log_Write(socket,LOG_LEVEL_ALARM, i, 2002, NULL)
#define WRITE_LOG_MotionDetectAlarmHapp(socket, i) log_Write(socket,LOG_LEVEL_ALARM, i, 2003, NULL)
#define WRITE_LOG_MotionDetectAlarmNoHapp(socket, i) log_Write(socket,LOG_LEVEL_ALARM, i, 2004, NULL)
#define WRITE_LOG_PortAlarmHapp(socket,i) log_Write(socket,LOG_LEVEL_ALARM, i, 2005, NULL)
#define WRITE_LOG_PortAlarmNoHapp(socket,i) log_Write(socket,LOG_LEVEL_ALARM, i, 2006, NULL)

//zhangweiwei add 智能分析报警
//-----------------------------------------------------------------------
#if 0
#define WRITE_LOG_TripWireAlarmHapp(i,j)  log_Write(LOG_LEVEL_ALARM, i, 2007, "%d", j)
#define WRITE_LOG_TripWireAlarmNoHapp(i,j)  log_Write(LOG_LEVEL_ALARM, i, 2008, "%d", j)

#define WRITE_LOG_PeriMeterAlarmHapp(i,j) log_Write(LOG_LEVEL_ALARM, i, 2009, "%d", j)
#define WRITE_LOG_PeriMeterAlarmNoHapp(i,j) log_Write(LOG_LEVEL_ALARM, i, 2010, "%d", j)

#define WRITE_LOG_FaceAlarmHapp(i,j)      log_Write(LOG_LEVEL_ALARM, i, 2011, "%d", j)
#define WRITE_LOG_FaceAlarmNoHapp(i,j)      log_Write(LOG_LEVEL_ALARM, i, 2012, "%d", j)

#define WRITE_LOG_DiagnoseAlarmHapp(i, j)   log_Write(LOG_LEVEL_ALARM, i, 2013, "%d", j)
#define WRITE_LOG_DiagnoseAlarmNoHapp(i, j)   log_Write(LOG_LEVEL_ALARM, i, 2014, "%d", j)
#define WRITE_LOG_AbandumAlarmHapp(i, j)   log_Write(LOG_LEVEL_ALARM, i, 2015, "%d", j)
#define WRITE_LOG_AbandumAlarmNoHapp(i, j)   log_Write(LOG_LEVEL_ALARM, i, 2016, "%d", j)


#else
#define WRITE_LOG_TripWireAlarmHapp(socket,i,j)  log_Write(socket, LOG_LEVEL_ALARM, i, 2007, NULL)
#define WRITE_LOG_TripWireAlarmNoHapp(socket,i,j)  log_Write(socket, LOG_LEVEL_ALARM, i, 2008, NULL)

#define WRITE_LOG_PeriMeterAlarmHapp(socket,i,j) log_Write(socket, LOG_LEVEL_ALARM, i, 2009, NULL)
#define WRITE_LOG_PeriMeterAlarmNoHapp(socket,i,j) log_Write(socket, LOG_LEVEL_ALARM, i, 2010, NULL)

#define WRITE_LOG_FaceAlarmHapp(socket,i,j)      log_Write(socket, LOG_LEVEL_ALARM, i, 2011, NULL)
#define WRITE_LOG_FaceAlarmNoHapp(socket,i,j)      log_Write(socket, LOG_LEVEL_ALARM, i, 2012, NULL)

#define WRITE_LOG_DiagnoseAlarmHapp(socket,i, j)   log_Write(SYSTEMSTAT, LOG_LEVEL_ALARM, i, 2013, NULL)
#define WRITE_LOG_DiagnoseAlarmNoHapp(socket,i, j)   log_Write(socket, LOG_LEVEL_ALARM, i, 2014, NULL)

#define WRITE_LOG_AbandumAlarmHapp(socket,i, j)   log_Write(socket, LOG_LEVEL_ALARM, i, 2015, NULL)
#define WRITE_LOG_AbandumAlarmNoHapp(socket,i, j)   log_Write(socket, LOG_LEVEL_ALARM, i, 2016, NULL)

#define WRITE_LOG_CapAlarmHapp(socket,i, j)   log_Write(socket,LOG_LEVEL_ALARM, i, 2021, NULL)
#define WRITE_LOG_CapAlarmNoHapp(socket,i, j)   log_Write(socket,LOG_LEVEL_ALARM, i, 2022, NULL)

#define WRITE_LOG_CddAlarmHapp(socket,i, j)   log_Write(socket,LOG_LEVEL_ALARM, i, 2023, NULL)
#define WRITE_LOG_CddAlarmNoHapp(socket,i, j)   log_Write(socket,LOG_LEVEL_ALARM, i, 2024, NULL)
#define WRITE_LOG_SvdAlarmHapp(socket,i, j) log_Write(socket,LOG_LEVEL_ALARM, i, 2025, NULL)
#define WRITE_LOG_SvdAlarmNoHapp(socket,i, j) log_Write(socket,LOG_LEVEL_ALARM, i, 2026, NULL)


#endif

//20121225 SUN Wei add for Debug alarm clear exceptio
#define WRITE_LOG_NETAlarmClear(socket,i,j) //log_Write(socket, LOG_LEVEL_ALARM, i, 2017, (td_u8 *)"%d", j)
#define WRITE_LOG_NETAlarmAllClear(socket,i) //log_Write(socket, LOG_LEVEL_ALARM, CHANNEL_INVALID, 2018, (td_u8 *)"%d", i)
#define WRITE_LOG_UIAlarmClear(socket,i,j) //log_Write(socket, LOG_LEVEL_ALARM, i, 2019, (td_u8 *)"%d", j)
#define WRITE_LOG_UIAlarmAllClear(socket,i) //log_Write(socket, LOG_LEVEL_ALARM, CHANNEL_INVALID, 2020, (td_u8 *)"%d", i)
//20121225 SUN Wei add for Debug alarm clear exception end
//------------------------------------------------------------------------
//add by gyn 20120903
//起始从2101开始，音频丢失报警日志
#define WRITE_LOG_AudioLostAlarmHapp(socket,i) log_Write(socket, LOG_LEVEL_ALARM, i, 2101, NULL)
#define WRITE_LOG_AudioLostAlarmNoHapp(socket,i) log_Write(socket, LOG_LEVEL_ALARM, i, 2102, NULL)
//add end

//20130305 SUN Wei add for OD alarm
#define WRITE_LOG_VideoCoverAlarmHapp(socket,i) log_Write(socket, LOG_LEVEL_ALARM, i, 2201, NULL);
#define WRITE_LOG_VideoCoverAlarmNoHapp(socket,i) log_Write(socket, LOG_LEVEL_ALARM, i, 2202, NULL);
//20130305 SUN Wei add for OD alarm end
//add by zhb 20130904 添加3g或wifi日志
#define WRITE_LOG_NotLoad3gMod(socket,s) log_Write(socket, LOG_LEVEL_ALARM, CHANNEL_INVALID,2301 ,(td_u8 *)"%s", s);
#define WRITE_LOG_NotFoundSIM(socket,s) log_Write(socket, LOG_LEVEL_ALARM, CHANNEL_INVALID,2302 ,(td_u8 *)"%s", s);
#define WRITE_LOG_SIMException(socket,s) log_Write(socket, LOG_LEVEL_ALARM, CHANNEL_INVALID,2303 ,(td_u8 *)"%s", s);
#define WRITE_LOG_NotLoadWifiMod(socket,s) log_Write(socket, LOG_LEVEL_ALARM, CHANNEL_INVALID,2304 ,(td_u8 *)"%s", s);
#define WRITE_LOG_DatabaseErr(socket) log_Write(socket, LOG_LEVEL_WARNING, CHANNEL_INVALID,2305 ,NULL);//数据库操作失败

//add end
//operation
#define WRITE_LOG_DownloadLogFile(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3001, NULL)
#define WRITE_LOG_RebuildIndexRemoteSucc(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3002, NULL)
#define WRITE_LOG_RebuildIndexRemoteFail(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3003, NULL)
#define WRITE_LOG_ModifyChnAudioEncode(socket,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, atoi(s1), 3004, (td_u8 *)"%s", s2)
#define WRITE_LOG_SetValueFrom(socket,s1,s2,s3) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3005, (td_u8 *)"%s\t%s\t%s", s1, s2, s3)
#define WRITE_LOG_ChangeIP(socket,s1,s2,s3,s4) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3006, (td_u8 *)"%s\t%s\t%s\t%s", s1, s2, s3, s4)
#define WRITE_LOG_ChangeMac(socket,x1,x2,x3,x4,x5,x6) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3007, (td_u8 *)"%2X\t%2X\t%2X\t%2X\t%2X\t%2X", x1, x2, x3, x4, x5, x6)
#define WRITE_LOG_ChangePort(socket,d1,d2,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3008, (td_u8 *)"%d\t%d\t%s\t%s", d1, d2, s1, s2)
#define WRITE_LOG_RemoteRebootFromManagerClient(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3009, NULL)
#define WRITE_LOG_NewIPMask(socket,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3010, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_SetNetConfigLocally(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3011, NULL)
#define WRITE_LOG_NewGatewayDNS(socket,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3012, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_SetBurnPolicy(socket,s1,d1,d2,d3) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3013, (td_u8 *)"%s\t%d\t%d\t%d", s1,d1,d2,d3)
#define WRITE_LOG_StartBurn(socket,d1,s1) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3014, (td_u8 *)"%d\t%s", d1,s1)
#define WRITE_LOG_StopBurn(socket,d1,s1) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3015, (td_u8 *)"%d\t%s", d1,s1)
#define WRITE_LOG_StartOneKeyBurn(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3016, NULL)
#define WRITE_LOG_StopOneKeyBurn(socket) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3017, NULL)
#define WRITE_LOG_SetTimeZone(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3018, (td_u8 *)"%s", s1)
#define WRITE_LOG_SetTimeOfSystem(socket,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3019, (td_u8 *)"%s\t%s",s1,s2)
#define WRITE_LOG_GPSModifyTime(socket) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,3020, NULL);
#define WRITE_LOG_NtpSetTimeOfSystem(socket,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3027, (td_u8 *)"%s\t%s",s1,s2)
#define WRITE_LOG_DeleteVideoFile(socket,s1,s2) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID,3028,(td_u8 *)"%s\t%s",s1,s2)
#define WRITE_LOG_ExportConfigFile(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 3063, (td_u8 *)"%s",s1)


//user
#define WRITE_LOG_ModifyUserPassWordSucc(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4001, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_ModifyUserPassWordFail(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4002, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_AddUserFromSucc(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4003, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_AddUserFromFail(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4004, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_DeleteUserFromSucc(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4005, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_DeleteUserFromFail(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4006, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_UserLogonLocalSucc(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4007, (td_u8 *)"%s", s1)
#define WRITE_LOG_UserLogonLocalFail(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4008, (td_u8 *)"%s", s1)
#define WRITE_LOG_UserLogoffLocalSucc(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4009, (td_u8 *)"%s", s1)
#define WRITE_LOG_UserLogoffLocalFail(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4010, (td_u8 *)"%s", s1)
#define WRITE_LOG_AddUserLocalSucc(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4011, (td_u8 *)"%s", s1)
#define WRITE_LOG_AddUserLocalFail(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4012, (td_u8 *)"%s", s1)
#define WRITE_LOG_ModifyUserLocalSucc(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4013, (td_u8 *)"%s", s1)
#define WRITE_LOG_ModifyUserLocalFail(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4014, (td_u8 *)"%s", s1)
#define WRITE_LOG_DeleteUserLocalSucc(socket,s1,d1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4015, (td_u8 *)"%s", s1)
#define WRITE_LOG_DeleteUserLocalFail(socket,s1,d1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4016, (td_u8 *)"%s", s1)
#define WRITE_LOG_UserLogonFromSucc(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4017,(td_u8 *) "%s\t%s", s1, s2)
#define WRITE_LOG_UserLogoffFromSucc(socket,s1,s2) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4018, (td_u8 *)"%s\t%s", s1, s2)
#define WRITE_LOG_ModifyUserSetRightLocalSucc(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4019, (td_u8 *)"%s", s1)
#define WRITE_LOG_ModifyUserSetRightLocalFail(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4020, (td_u8 *)"%s", s1)
#define WRITE_LOG_RemoteModifyUserSetRightSucc(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4021, (td_u8 *)"%s", s1)
#define WRITE_LOG_RemoteModifyUserSetRightFail(socket,s1) log_Write(socket, LOG_LEVEL_USER, CHANNEL_INVALID, 4022, (td_u8 *)"%s", s1)



//other
#define WRITE_LOG_TalkSetSockoptErr(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5001, NULL)
#define WRITE_LOG_SetNetServiceConfig(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5002, NULL)
#define WRITE_LOG_SetRegisterCenterConfig(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5003, NULL)
#define WRITE_LOG_SetDiskConfig(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5004, NULL)
#define WRITE_LOG_SetEncodeParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5005, NULL)
#define WRITE_LOG_SetOsdParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5006, NULL)
#define WRITE_LOG_SetVideoinParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5007, NULL)
#define WRITE_LOG_ChangeVideoParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5008, NULL)
#define WRITE_LOG_SetVideoParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5009, NULL)
#define WRITE_LOG_SetRecordStrategy(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5010, NULL)
#define WRITE_LOG_SetSerialParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5011, NULL)
#define WRITE_LOG_SetPTZParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5012, NULL)
#define WRITE_LOG_SetPortAlarmParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5013, NULL)
#define WRITE_LOG_SetAlarmOutPortParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5014, NULL)
#define WRITE_LOG_SetMotionDetectAlarmParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5015, NULL)

#define WRITE_LOG_SetODAlarmParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5025, NULL)//20130305 SUN Wei add for OD alarm end

#define WRITE_LOG_SetVideoLostAlarmParameter(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5016, NULL)
#define WRITE_LOG_ConnectChnModeFrom(socket,d1,d2,s1) log_Write(socket, LOG_LEVEL_OTHER, d1, 5017, (td_u8 *)"%d\t%s", d2, s1)
#define WRITE_LOG_DisConnectChnModeFrom(socket,d1,d2,s1) log_Write(socket, LOG_LEVEL_OTHER, d1, 5018, (td_u8 *)"%d\t%s", d2, s1)
#define WRITE_LOG_GetDatabasePathFail(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5019, NULL)
#define WRITE_LOG_CurStorPathIsFull(socket,s1) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5020, (td_u8 *)"%s", s1)
#define WRITE_LOG_OverwritingRecordAndImageFilesStart(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5021, NULL)
#define WRITE_LOG_OverwritingRecordAndImageFilesFinish(socket) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5022, NULL)
#define WRITE_LOG_ConnectChnModeFromSubStream(socket,d1,d2,s1) log_Write(socket, LOG_LEVEL_OTHER, d1, 5023, (td_u8 *)"%d\t%s", d2, s1)
#define WRITE_LOG_DisConnectChnModeFromSubStream(socket,d1,d2,s1) log_Write(socket, LOG_LEVEL_OTHER, d1, 5024, (td_u8 *)"%d\t%s", d2, s1)
//#define WRITE_LOG_SetRecordTemplate() log_Write(LOG_LEVEL_OTHER, CHANNEL_INVALID, 5025, NULL)
#define WRITE_LOG_ConnectChnModeFromAudioStream(socket,d1,d2,s1) log_Write(socket, LOG_LEVEL_OTHER, d1, 5025, (td_u8 *)"%d\t%s", d2, s1)
#define WRITE_LOG_DisConnectChnModeFromAudioStream(socket,d1,d2,s1) log_Write(socket, LOG_LEVEL_OTHER, d1, 5026, (td_u8 *)"%d\t%s", d2, s1)

#define WRITE_LOG_String(socket,s)	log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 6001, (td_u8*)"%s", s)

//add by zhb 20130904 3gandwifi
#define WRITE_LOG_3gBeginDial(socket,s) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 5301, (td_u8*)"%s", s)
#define WRITE_LOG_3gOnLine(socket,s) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 5302, (td_u8*)"%s", s)
#define WRITE_LOG_3gOffLine(socket,s) log_Write(socket, LOG_LEVEL_OPERATION, CHANNEL_INVALID, 5303, (td_u8*)"%s", s)
#define WRITE_LOG_3gNoDataRstMod(socket,s) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5304, (td_u8*)"%s", s)
#define WRITE_LOG_3gTenDailFailed(socket,s) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5305, (td_u8*)"%s", s)
#define WRITE_LOG_3gSendSmsFailed(socket,s) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5306, (td_u8*)"%s", s)
#define WRITE_LOG_3gSmsFormatError(socket,s) log_Write(socket, LOG_LEVEL_OTHER, CHANNEL_INVALID, 5307, (td_u8*)"%s", s)
//add end
//debug
#define LOG_DEFINE_LIMIT 	6
#define WRITE_LOG_DebugInfo(level,s) \
	{\
		if(level>=LOG_DEFINE_LIMIT)\
			log_Write(SYSTEMSTAT, LOG_LEVEL_DEBUG, CHANNEL_INVALID, 6001, (td_u8 *)"%s", s);\
		else\
			td_printf(0, "DebugInfo: level = %d, msg = %s\n",level, s);\
	}

// for decoder
#define WRITE_LOG_SetNetPara(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100001,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetDNSPara(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100002,(td_u8 *)"%s",s1)
#define WRITE_LOG_Set485(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100003,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetAlarmOut(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100004,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetChannelInfo(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100005,(td_u8 *)"%s",s1)
#define WRITE_LOG_AddLoopItem(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100006,(td_u8 *)"%s",s1)
#define WRITE_LOG_DelLoopItem(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100007,(td_u8 *)"%s",s1)
#define WRITE_LOG_StartLoop(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100008,(td_u8 *)"%s",s1)
#define WRITE_LOG_StopLoop(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100009,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetLogo(socket,s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100010,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetRes(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100011,(td_u8 *)"%s",s1)
#define WRITE_LOG_StartView(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100012,(td_u8 *)"%s",s1)
#define WRITE_LOG_StopView(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100013,(td_u8 *)"%s",s1)
#define WRITE_LOG_StartPush(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100014,(td_u8 *)"%s",s1)
#define WRITE_LOG_StopPush(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100015,(td_u8 *)"%s",s1)
#define WRITE_LOG_SoundCtl(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100016,(td_u8 *)"%s",s1)
#define WRITE_LOG_TalkCtl(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100017,(td_u8 *)"%s",s1)
#define WRITE_LOG_UpdateProgram(socket) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100018,NULL)
#define WRITE_LOG_UpdateWeb(socket) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100019,NULL)
#define WRITE_LOG_UpdateLogo(socket) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100020,NULL)
#define WRITE_LOG_UpdateProtocol(socket) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100021,NULL)
#define WRITE_LOG_DelProtocol(socket) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100022,NULL)

//for decoder platform
#define WRITE_LOG_SetManageInfo(socket, s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100101,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetCommonInfo(socket, s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100102,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetCommonInfo_Channel(socket, s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100103,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetSIPConfig(socket, s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100104,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetSIPChnPara(socket, s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100105,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetVolume(socket,s1) log_Write(socket,LOG_LEVEL_OPERATION,CHANNEL_INVALID,100107,(td_u8 *)"%s",s1)
#define WRITE_LOG_SetAppname(socket, s1) log_Write(socket, LOG_LEVEL_OPERATION,CHANNEL_INVALID,100108,(td_u8 *)"%s",s1)

#endif


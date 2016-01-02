/**************************************************
文件名:net_detect.h
公司：Tianjin Tiandy Tech.
　　创建者:Tiandy shihc
　　创建日期:2013-10-24
　　简要描述:网络部分对延时、丢包  、抓包、流量统计等的实现
修改记录:
日期        修改者    修改原因 
2013-10-24  Tiandy    Create this File
******************************************************************/ 

td_s32 Net_Detect_Pcap(td_s32 _ieth, td_u8* _cPath, td_s32 _iPackSize, td_s32 _iTmDelay);
td_s32 Net_Detect_Ping(td_s32 _iEth,td_char* _cIpDomain,td_s32 *_iReach,td_s32 *_iDealy,td_s32 *_iLost);



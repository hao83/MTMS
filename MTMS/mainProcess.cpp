//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "FrameBase.h"
#include "MTCPConnection.h"
#include "mtcp_module.h"

#include <thread>
#include <queue>
#include <mutex>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"  
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "CallDll.h"

#include <WinSock2.h>
#include "CrashDump.h"

#include <atlstr.h>
#include <atltime.h>
#include <boost\regex.hpp>
#include <ctype.h>

#pragma comment(lib, "CrashDump.lib")
#pragma comment(lib, "ws2_32.lib")

//pub/sub  req/rep
#include "ZmqSubscriber.h"
#include "ZmqReplyer.h"
#include "TMHandler.h"

#include "JsonHelper.h"

using namespace std;

/*********** program log **************/
#include "CLog.h"
#define SLOG1(x)			CLog::Instance()->WriteLog(x);
#define SLOG2(x, y)			CLog::Instance()->WriteJoinLog(x, y);	
#define LOG1(x)				CLog::Instance()->WriteLog(__FILE__, __LINE__, x)
#define LOG2(x, y)			CLog::Instance()->WriteJoinLog(__FILE__, __LINE__, x, y)
/***********************************/

// queue for TM_to MTCP, storage Json message 
// std::queue<char* > TM_to_MTCP;

// for thread 
// mutex mtx;


const int pre_Port = 61806;
const int post_Port = 61807;

static int  choose_Port = 0;
static char peer_address[128];
static int  peer_port = pre_Port;

void * g_context = NULL;

// all protocol objects
TSCR tscr; TSED tsed; SEQU sequ; INIT init; VTST vtst; DPCK dpck; DPWR dpwr;
DPWC dpwc; VCAL vcal; VDCR vdcr; CTSS ctss; EMCT emct; BMPF bmpf; FLDP fldp;
CTES ctes; ALPR alpr; ALPH alph; POST post; FLDI fldi;
YGTR ygtr; YGTS ygts; YGWC ygwc; YGWR ygwr;
YGTSPOST ygtspost;VTSTPOST vtstpost;



// 填充数据结构
int fill_protocol_objects(char * data, int length)
{
	char type[128];
	char ID[128];
	char Val[2048];
	int event = -1;

	//log string
	char logdata[1024];
	memset(logdata, 0, 1024);

	memset(type, 0, 128);
	memset(ID, 0, 128);
	memset(Val, 0, 2048);

	event = pub_json_to_mtcp(data, type, ID, Val);

	if (101 == event)
	{
		Sleep(0);
		return 0;
	}
		
	if (0 == strcmp(type, "TSCR"))
	{
		// start = GetCurrentTime();
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		tscr.setvalue(ID, Val);
		//if (1 == event){
		//	tMTCP_payload_TSCR_RSP ret = tscr.SendTSCR();
		//	sprintf(logdata, "receive response message from napa....tscr_ret.HOST_SW_VERSION:%d", ret.HOST_SW_VERSION);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "TSED")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	ret = tsed.SendTSED();
		//	temp = GetCurrentTime() - start;
		//	sprintf(logdata, "receive response message from napa...tsed_ret.T_ERRC:%d,tsed_ret.T_ERRS:%s,cost time : %d"
		//		, ret.T_ERRC, ret.T_ERRS,temp);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "SEQU")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_SEQU_RSP ret = sequ.SendSEQU();
		//	sprintf(logdata, "receive response message from napa...sequ_ret.TEST_ITEM_CNT:%d,sequ_ret.TEST_CTRL_CODE:%s,sequ_ret:CHAMBER_CNT:%d,sequ_ret:CHAMBER_TO_RUN:%s"
		//		, ret.TEST_ITEM_CNT, ret.TEST_CTRL_CODE, ret.CHAMBER_CNT
		//		, sequ.CHAMBER_TO_RUN());
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "INIT")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		init.setvalue(ID, Val);
		//if (1 == event){
		//	ret = init.SendINIT();
		//	sprintf(logdata, "receive response message from napa...init_ret.T_ERRC:%d,init_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "VTST")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		if (choose_Port == pre_Port){
			if (0 == strcmp(ID, "VTST_RAWDATA")){
				vtst.readFloatArr_fromFile(Val);
			}
			else{
				vtst.setvalue(ID, Val);
			}
		}
		else if (choose_Port == post_Port){
			if (0 == strcmp(ID, "VTST_RAWDATA")){
				vtstpost.readFloatArr_fromFile(Val);
			}
			else{
				vtstpost.setvalue(ID, Val);
			}
		}
		//if (1 == event){
		//	ret = vtst.SendVTST();
		//	sprintf(logdata, "receive response message from napa...vtst_ret.T_ERRC:%d,vtst_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "DPCK")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		dpck.setvalue(ID, Val);
		//if (1 == event){
		//	ret = dpck.SendDPCK();
		//	sprintf(logdata, "receive response message from napa...dpck_ret.T_ERRC:%d,dpck_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "DPWR")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_DPWR_RSP dpwr_ret = dpwr.SendDPWR();
		//	memset(&dpwr_ret, 0, sizeof(tMTCP_payload_DPWR_RSP));
		//	//dpwr_ret = dpwr.GetData();	
		//	sprintf(logdata, "receive response message from napa...dpwr_ret.CNT:%d,dpwr.ADDR:%d,dpwr.DATA:%d"
		//		, dpwr_ret.CNT, dpwr_ret.ADDR, dpwr_ret.DATA);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "DPWC")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		dpwc.setvalue(ID, Val);
		//if (1 == event){
		//	ret = dpwc.SendDPWC();
		//	sprintf(logdata, "receive response message from napa...T_ERRC:%d,T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "VCAL")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		if (0 == strcmp(ID, "TTDP")){
			vcal.setTTDP(Val);// val files path
		}
		else if (0 == strcmp(ID, "ADIP")){
			vcal.setADIP(Val);// val; files path

		}
		else{
			vcal.setvalue(ID, Val);

		}
		//if (1 == event){
		//	sprintf(logdata, "received message:%s", data);
		//	SLOG1(logdata);
		//	vcal.setAllRawdata();
		//	ret = vcal.SendVCAL();
		//	sprintf(logdata, "receive response message from napa...vcal_ret.T_ERRC:%d,vcal_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "VDCR")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_VDCR_RSP vdcr_ret = vdcr.SendVDCR();
		//	sprintf(logdata, "receive response message from napa...vdcr_ret.I_DR:%f,vdcr_ret.T_SETTLE:%f"
		//		, vdcr_ret.I_DR, vdcr_ret.T_SETTLE);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "CTSS")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (0 == strcmp(ID, "CTSS_RAWDATA")){
		//	char* c1ss_rawdata = nullptr;
		//	ctss.readFloatArr_fromFile(Val);
		//	//if (1 == event){
		//	//	ret = ctss.SendC1SS();
		//	//	sprintf(logdata, "receive response message from napa...c1ss_ret.T_ERRC:%d,c1ss_ret.T_ERRS:%s"
		//	//		, ret.T_ERRC, ret.T_ERRS);
		//	//	SLOG1(logdata);
		//	//}
		//}
		//else
		{
			ctss.setvalue(ID, Val);
			//if (1 == event){
			//	ret = ctss.SendC1SS();
			//	sprintf(logdata, "receive response message from napa...c1ss_ret.T_ERRC:%d,c1ss_ret.T_ERRS:%s"
			//		, ret.T_ERRC, ret.T_ERRS);
			//	SLOG1(logdata);
			//}
		}
	}
		
	else if (0 == strcmp(type, "EMCT")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//cout << "EMCT....ID:" << ID << endl;
		if (0 == strcmp(ID, "IMAGE_DATA")){
			int rawret = emct.setRawdata(Val);
			// cout << "-------------------rawret :" << endl;
		}
		else{
			emct.setvalue(ID, Val);
		}
		//if (1 == event){
		//	ret = emct.SendEMCT();
		//	sprintf(logdata, "receive response message from napa...emct_ret.T_ERRC:%d,emct_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "BMPF")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		if (0 == strcmp(ID, "IMAGE_DATA")){
			int rawret = bmpf.setRawdata(Val);
			// cout << "setRawdata..." << rawret << endl;
		}
		else
		{
			bmpf.setvalue(ID, Val);
		}
		//if (1 == event){
		//	ret = bmpf.SendBMPF();
		//	cout << "send bmpf success......." << endl;
		//	sprintf(logdata, "receive response message from napa...bmpf_ret.T_ERRC:%d,bmpf_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "FLDP")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		fldp.setvalue(ID, Val);
		//if (1 == event){
		//	ret = fldp.SendFLDP();
		//	sprintf(logdata, "receive response message from napa...fldp_ret.T_ERRC:%d,fldp_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "CTES")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		ctes.setvalue(ID, Val);
		//if (1 == event){
		//	ret = ctes.SendC1ES();
		//	sprintf(logdata, "receive response message from napa...c1es_ret.T_ERRC:%d,c1es_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "ALPR")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_ALPR_RSP alpr_ret = alpr.SendALPR();
		//	sprintf(logdata, "receive response message from napa...alpr_ret.HOT_SPOT_PHI:%f,alpr_ret.HOT_SPOT_THETA:%f"
		//		, alpr_ret.HOT_SPOT_PHI, alpr_ret.HOT_SPOT_THETA);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "ALPH")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		if (0 == strcmp(ID, "ALPH_IMAGES")){
			// cout << "ALPH_Val:....." << Val << endl;

			/*****************************/
			char** filename_arr = new char *[64];
			memset(filename_arr, 0, 64);
			for (int i = 0; i < 16; i++)
			{
				filename_arr[i] = new char[128];
				memset(filename_arr[i], 0, 128);
			}
			/*****************************/
			int file_count = ALPHCount(Val, filename_arr);
			if (file_count < 1)
			{
				// continue;
			}
			else
			{
				// cout << "-------------1:" << filename_arr[0] << endl;
				// cout << "-------------2:" << filename_arr[1] << endl;
				// cout << "-------------3:" << filename_arr[2] << endl;
				alph.setRawdata(file_count, filename_arr);
			}
		}
		else
		{
			alph.setvalue(ID, Val);
		}
		//if (1 == event)
		//{
		//	ret = alph.SendALPH();
		//	sprintf(logdata, "receive response message from napa...alpr_ret.T_ERRC:%d,alpr_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "POST")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_POST_RSP post_ret = post.SendPOST();
		//	sprintf(logdata, "receive response message from napa... post_ret.T_ERRC:%d,post_ret.T_ERRS:%s,post_ret.T_BIN:%d"
		//		, post_ret.T_ERRC, post_ret.T_ERRS, post_ret.T_BIN);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "FLDI")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		if (0 == strcmp(ID, "IMAGE_DATA")){
			fldi.setImageRawdata(Val);
		}
		else if (0 == strcmp(ID, "FLAT_DATA")){
			fldi.setFloatRawdata(Val);
		}
		else
		{
			fldi.setvalue(ID, Val);
		}
		//if (1 == event){
		//	fldi.setAllRawdata();
		//	ret = fldi.SendFLDI();
		//	sprintf(logdata, "receive response message from napa...fldi_ret.T_ERRC:%d,fldi_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "YGTS")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		if (choose_Port == pre_Port){
			ygts.setvalue(ID, Val);
		}
		else if (choose_Port == post_Port){
			ygtspost.setvalue(ID, Val);
		}
		//if (1 == event){
		//	ret = ygts.SendYGTS();
		//	sprintf(logdata, "receive response message from napa...ygts_ret.T_ERRC:%d,ygts_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "YGWC")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		ygwc.setvalue(ID, Val);
		//if (1 == event){
		//	ret = ygwc.SendYGWC();
		//	sprintf(logdata, "receive response message from napa...ygwc_ret.T_ERRC:%d,ygwc_ret.T_ERRS:%s"
		//		, ret.T_ERRC, ret.T_ERRS);
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "YGTR")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_YGTR_RSP ygtr_ret = ygtr.SendYGTR();
		//	SLOG1(logdata);
		//}
	}

	else if (0 == strcmp(type, "YGWR")){
		sprintf(logdata, "received message:%s", data);
		SLOG1(logdata);
		//if (1 == event){
		//	tMTCP_payload_YGWR_RSP ygwr_ret = ygwr.SendYGWR();
		//	sprintf(logdata, "receive response message from napa...ygwr_ret.CNT:%s,ygwr_ret.CNT:%s,ygwr_ret.CNT:%s"
		//		, ygwr_ret.CNT, ygwr_ret.ADDR, ygwr_ret.DATA);
		//	SLOG1(logdata);
		//}
	}

	return 0;
}



//REQ: send && rep
int dispatch_request_proc(char * data, int length
	, ZmqReplyer * TM2, const char * peer_address, int port, int timeout)
{
	tMTCP_payload_TEST_RSP ret;

	// char dpwr_ret[2048];

	char logdata[32 + 4096 + 1];
	int i = 1;

	char type[128];
	char ID[128];
	char result[1024];
	char sendresult[1024];
	
	// int _errc = 0;
	// char _errs[1024];

	memset(type, 0, 128);
	memset(ID, 0, 128);
	//memset(req_msg, 0, 4096);
	memset(result, 0, 1024);
	memset(sendresult, 0, 1024);
	// memset(_errs, 0, 1024);
	// memset(dpwr_ret, 0, 2048);

	sprintf(logdata, "received request from TM[%d]:%s", length, data);
	SLOG1(logdata);
	printf("%s\n", logdata);

	req_json_to_mtcp(data, type, ID);

	if (0 == strcmp(type, "ALPR"))
	{
		tMTCP_payload_ALPR_RSP alpr_ret = alpr.SendALPR();

		sprintf(logdata, "receive response message from napa...alpr_ret.HOT_SPOT_PHI:%f,alpr_ret.HOT_SPOT_THETA:%f"
			, alpr_ret.HOT_SPOT_PHI, alpr_ret.HOT_SPOT_THETA);
		SLOG1(logdata);

		if (0 == strcmp(ID, "HOT_SPOT_THETA"))
		{
			sprintf(result, "%f", alpr_ret.HOT_SPOT_THETA);
			mtcp_to_json(result, sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "HOT_SPOT_PHI"))
		{
			sprintf(result, "%f", alpr_ret.HOT_SPOT_PHI);
			mtcp_to_json(result, sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else
		{
			//char buf[] = "Message type error!";
			//mtcp_to_json(buf, sendresult);
			//TM2->send(sendresult);
			//sprintf(logdata, "send message to TM...error_str:%s", buf);
			//SLOG1(logdata);
			JsonWirter j;
			j.StartArray();
			j.WriteJson("HOT_SPOT_THETA", alpr_ret.HOT_SPOT_THETA);
			j.WriteJson("HOT_SPOT_PHI", alpr_ret.HOT_SPOT_PHI);
			j.EndArray();
			TM2->send_n(j.GetString(), j.GetLength());
		}
	}
	else if (0 == strcmp(type, "SEQU"))
	{
		tMTCP_payload_SEQU_RSP ret = sequ.SendSEQU();

		const char * temp = sequ.CHAMBER_TO_RUN();
		sprintf(logdata, "receive response message from napa...sequ_ret.TEST_ITEM_CNT:%d,sequ_ret.TEST_CTRL_CODE:%s,sequ_ret:CHAMBER_CNT:%d,sequ_ret:CHAMBER_TO_RUN:%s"
			, ret.TEST_ITEM_CNT, ret.TEST_CTRL_CODE, ret.CHAMBER_CNT
			, temp);
		SLOG1(logdata);

		if (0 == strcmp(ID, "TEST_ITEM_CNT"))
		{
			_ultoa(ret.TEST_ITEM_CNT, result, 10);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "TEST_CTRL_CODE"))
		{
			sprintf(result, "%s", ret.TEST_CTRL_CODE);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "CHAMBER_CNT"))
		{
			_ultoa(ret.CHAMBER_CNT, result, 10);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "CHAMBER_TO_RUN"))
		{
			sprintf(result, "%s", temp);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else
		{
			//char buf[] = "Message type error!";
			//mtcp_to_json(buf, sendresult);
			//TM2->send(sendresult);
			//sprintf(logdata, "send message to TM...error_str:%s", buf);
			//SLOG1(logdata);
			JsonWirter jw;
			jw.StartArray();
			jw.WriteJson("TEST_ITEM_CNT", ret.TEST_ITEM_CNT);
			jw.WriteJson("TEST_CTRL_CODE", ret.TEST_CTRL_CODE);
			jw.WriteJson("CHAMBER_CNT", ret.CHAMBER_CNT);
			jw.WriteJson("CHAMBER_TO_RUN", ret.CHAMBER_TO_RUN);
			jw.EndArray();
			TM2->send_n(jw.GetString(), jw.GetLength());
		}
	}
	else if (0 == strcmp(type, "POST"))
	{
		tMTCP_payload_POST_RSP post_ret = post.SendPOST();
		sprintf(logdata, "receive response message from napa... post_ret.T_ERRC:%d,post_ret.T_ERRS:%s,post_ret.T_BIN:%d"
			, post_ret.T_ERRC, post_ret.T_ERRS, post_ret.T_BIN);
		SLOG1(logdata);
		
		if (0 == strcmp(ID, "T_ERRC"))
		{
			_ultoa(post_ret.T_ERRC, result, 10);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "T_ERRS"))
		{
			sprintf(result, "%s", post_ret.T_ERRS);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "T_BIN"))
		{
			_ultoa(post_ret.T_BIN, result, 10);
			mtcp_to_json(result, sendresult);
			// TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else
		{
			JsonWirter jw;
			jw.StartArray();
			jw.WriteJson("T_BIN", post_ret.T_BIN);
			jw.WriteJson("T_ERRS", post_ret.T_ERRS);
			jw.WriteJson("T_ERRC", post_ret.T_ERRC);
			jw.EndArray();
			TM2->send_n(jw.GetString(), jw.GetLength());
		}
	}
	else if (0 == strcmp(type, "TSCR"))
	{
		Close();
		Open(peer_address, port, timeout);

		tMTCP_payload_TSCR_RSP ret = tscr.SendTSCR();
		sprintf(logdata, "receive response message from napa....tscr_ret.HOST_SW_VERSION:%s", ret.HOST_SW_VERSION);
		SLOG1(logdata);

		JsonWirter jwriter;

		jwriter.StartArray();
		jwriter.WriteJson("result", 0);
		jwriter.WriteJson("HOST_SW_VERSION", ret.HOST_SW_VERSION);
		jwriter.EndArray();

		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}
	else  if (0 == strcmp(type, "VDCR"))
	{
		long start = GetCurrentTime();
		tMTCP_payload_VDCR_RSP vdcr_ret = vdcr.SendVDCR();
		long end = GetCurrentTime() - start;
		sprintf(logdata, "receive response message from napa...vdcr_ret.I_DR:%f,vdcr_ret.T_SETTLE:%f,cost time : %d"
			, vdcr_ret.I_DR, vdcr_ret.T_SETTLE, end);
		SLOG1(logdata);

		if (0 == strcmp(ID, "I_DR"))
		{
			sprintf(result, "%f", vdcr_ret.I_DR);
			mtcp_to_json(result, sendresult);
			//start = GetCurrentTime();
			//int res = TM2->send(sendresult);
			int res = TM2->send_n(sendresult, strlen(sendresult));
			long end1 = GetCurrentTime() - start;
			sprintf(logdata, "receive response message cost time 1: %d,result : %d,sendresult:%s"
				, end1, res, sendresult);
			SLOG1(logdata);
		}
		else if (0 == strcmp(ID, "T_SETTLE"))
		{
			sprintf(result, "%f", vdcr_ret.T_SETTLE);
			mtcp_to_json(result, sendresult);
			//start = GetCurrentTime();
			//int res1 = TM2->send(sendresult);
			int res = TM2->send_n(sendresult, strlen(sendresult));
			long end2 = GetCurrentTime() - start;
			sprintf(logdata, "receive response message cost time 1: %d,result : %d,sendresult:%s"
				, end2, res, sendresult);
			SLOG1(logdata);
		}
		else
		{
			//char buf[] = "Message type error!";
			//mtcp_to_json(buf, sendresult);
			//// TM2->send(sendresult);
			//sprintf(logdata, "send message to TM...error_str:%s", buf);
			//SLOG1(logdata);
			
			JsonWirter jwriter;

			jwriter.StartArray();
			jwriter.WriteJson("result", 0);
			jwriter.WriteJson("I_DR", vdcr_ret.I_DR);
			jwriter.WriteJson("T_SETTLE", vdcr_ret.T_SETTLE);
			jwriter.EndArray();

			TM2->send_n(jwriter.GetString(), jwriter.GetLength());
		}
	}
	else if (0 == strcmp(type, "TSED"))
	{
		Sleep(100);

		ret = tsed.SendTSED();

		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());

		Sleep(500);

		Close();
	}
	else if (0 == strcmp(type, "INIT"))
	{
		ret = init.SendINIT();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);

		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "VTST"))
	{
		printf("###start to upload VTST to MTCP Server!\r\n");
		if (choose_Port == pre_Port){
			printf("###Upload VTST with pre MODE!\r\n");
			vtst.MAGIC(0);
			ret = vtst.SendVTST();
		}
		else if (choose_Port == post_Port){
			printf("###Upload VTST with post MODE!\r\n");
			vtstpost.MAGIC(1);
			ret = vtstpost.SendVTSTPOST();
		}
		
		Sleep(100);

		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);

		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "DPCK"))
	{
		ret = dpck.SendDPCK();

		sprintf(logdata, "Send to MTCP port : %d,with return : %d", choose_Port, ret);
		SLOG1(logdata);

		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);

		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "DPWR"))
	{
		tMTCP_payload_DPWR_RSP ret = dpwr.SendDPWR();
		sprintf(logdata, "receive response message from napa...dpwr_ret.CNT:%d,dpwr_ret.ADDR:%d,dpwr_ret.DATA:%d"
			, ret.CNT, ret.ADDR, ret.DATA);
		SLOG1(logdata);

		printf("DPWR Dump out :\r\n");
		for (size_t i = 0; i < ret.CNT; i++)
		{
			printf("ADDR[0x%x] :  0x%x\r\n", ret.ADDR[i], ret.DATA[i]);
		}
		printf("DPWR Dump finished!\r\n");


		if (0 == strcmp(ID, "CNT"))
		{
			sprintf(result, "%d", ret.CNT);
			mtcp_to_json(result, sendresult);
			// TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "ADDR"))
		{
			//sprintf(result, "%d", ret.ADDR);
			memset(result, 0, 1024);
			for (size_t i = 0; i < ret.CNT; i++)
			{
				char tmp[256];
				char * fmt = (i == ret.CNT - 1) ? "%d" : "%d,";
				sprintf(tmp, fmt, ret.ADDR[i]);
				strcat(result, tmp);
			}
			mtcp_to_json(result, sendresult);


			printf("DPWR Address: %s\r\n", sendresult);
			// TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "DATA"))
		{
			//sprintf(result, "%d", ret.DATA);
			memset(result, 0, 1024);
			for (size_t i = 0; i < ret.CNT; i++)
			{
				char tmp[256];
				char * fmt = (i == ret.CNT - 1) ? "%d" : "%d,";
				sprintf(tmp, fmt, ret.DATA[i]);
				strcat(result, tmp);
			}

			mtcp_to_json(result, sendresult);

			printf("DPWR Data : %s\r\n", sendresult);
			// TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else
		{
			JsonWirter jwriter;

			jwriter.StartArray();
			jwriter.WriteJson("ADDR", ret.ADDR);
			jwriter.WriteJson("CNT", ret.CNT);
			jwriter.WriteJson("DATA", ret.DATA);
			jwriter.EndArray();

			TM2->send_n(jwriter.GetString(), jwriter.GetLength());
		}
	}

	else if (0 == strcmp(type, "DPWC"))
	{
		ret = dpwc.SendDPWC();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "VCAL"))
	{
		ret = vcal.SendVCAL();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "CTSS"))
	{
		ret = ctss.SendCTSS();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "EMCT"))
	{
		ret = emct.SendEMCT();

		Sleep(100);

		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "BMPF"))
	{
		ret = bmpf.SendBMPF();

		Sleep(100);

		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "FLDP"))
	{
		ret = fldp.SendFLDP();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "CTES"))
	{
		ret = ctes.SendCTES();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "ALPH"))
	{
		ret = alph.SendALPH();
		Sleep(500);
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "FLDI"))
	{
		ret = fldi.SendFLDI();
		Sleep(500);
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);
		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "YGTS"))
	{
		if (choose_Port == pre_Port){
			ret = ygts.SendYGTS();
		}
		else if (choose_Port == post_Port){
			ret = ygtspost.SendYGTSPOST();
		}
		
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);

		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "YGWC"))
	{
		ret = ygwc.SendYGWC();
		sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
			, ret.T_ERRC, ret.T_ERRS);
		SLOG1(logdata);

		//_errc = ret.T_ERRC;
		//sprintf(_errs, "%s", ret.T_ERRS);
		//mtcp_to_json(_errc, _errs, sendresult);
		//TM2->send(sendresult);
		JsonWirter jwriter;
		jwriter.StartArray();
		if (0 == ret.T_ERRC)
		{
			jwriter.WriteJson("result", 0);
		}
		else
		{
			jwriter.WriteJson("T_ERRC", ret.T_ERRC);
			jwriter.WriteJson("T_ERRS", ret.T_ERRS);
		}
		jwriter.EndArray();
		TM2->send_n(jwriter.GetString(), jwriter.GetLength());
	}

	else if (0 == strcmp(type, "YGTR"))
	{
		tMTCP_payload_YGTR_RSP ygtr_ret = ygtr.SendYGTR();
		sprintf(logdata, "receive response message from napa...ygtr_ret.I_PULSEMAX_B1:%f,ygtr_ret.I_PULSEMAX_B2:%f,ygtr_ret.I_PULSEMAX_B3:%f"
			, ygtr_ret.I_PULSEMAX_B1, ygtr_ret.I_PULSEMAX_B2, ygtr_ret.I_PULSEMAX_B3);
		SLOG1(logdata);
		if (0 == strcmp(ID, "I_PULSEMAX_B1"))
		{
			sprintf(result, "%f", ygtr_ret.I_PULSEMAX_B1);
			mtcp_to_json(result, sendresult);
			// TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "I_PULSEMAX_B2"))
		{
			sprintf(result, "%f", ygtr_ret.I_PULSEMAX_B2);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "I_PULSEMAX_B3"))
		{
			sprintf(result, "%f", ygtr_ret.I_PULSEMAX_B3);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else
		{
			JsonWirter jw;
			jw.StartArray();
			jw.WriteJson("I_PULSEMAX_B1", ygtr_ret.I_PULSEMAX_B1);
			jw.WriteJson("I_PULSEMAX_B2", ygtr_ret.I_PULSEMAX_B2);
			jw.WriteJson("I_PULSEMAX_B3", ygtr_ret.I_PULSEMAX_B3);
			jw.EndArray();
			TM2->send_n(jw.GetString(), jw.GetLength());
		}
	}
	else if (0 == strcmp(type, "YGWR"))
	{
		tMTCP_payload_YGWR_RSP ygwr_ret = ygwr.SendYGWR();
		sprintf(logdata, "receive response message from napa...ygwr_ret.CNT:%s,ygwr_ret.ADDR:%s,ygwr_ret.DATA:%s"
			, ygwr_ret.CNT, ygwr_ret.ADDR, ygwr_ret.DATA);
		SLOG1(logdata);
		if (0 == strcmp(ID, "CNT"))
		{
			sprintf(result, "%d", ygwr_ret.CNT);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "ADDR"))
		{
			sprintf(result, "%s", ygwr_ret.ADDR);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else if (0 == strcmp(ID, "DATA"))
		{
			sprintf(result, "%s", ygwr_ret.DATA);
			mtcp_to_json(result, sendresult);
			//TM2->send(sendresult);
			TM2->send_n(sendresult, strlen(sendresult));
		}
		else
		{
			JsonWirter jw;
			jw.StartArray();
			jw.WriteJson("CNT", ygwr_ret.CNT);
			jw.WriteJson("ADDR", ygwr_ret.ADDR);
			jw.WriteJson("DATA", ygwr_ret.DATA);
			jw.EndArray();
			TM2->send_n(jw.GetString(), jw.GetLength());
		}
	}
	else
	{
		char buf[] = "Message type error!";
		mtcp_to_json(buf, sendresult);
		//TM2->send(sendresult);
		TM2->send_n(sendresult, strlen(sendresult));
		sprintf(logdata, "send message to TM...error_str:%s", buf);
		SLOG1(logdata);
	}

	printf("%s\n", logdata);

	return 0;
}

// sub: recv data
int sub_thread_proc()
{
	char address[64];
	int port = 0;

	memset(address, 0, 64);

	ZmqSubscriber * TM1 = new ZmqSubscriber(g_context);

	Getaddress("pub", address, port);
	TM1->Connect(address, port);

	const int BUFF_SIZE =  4 * 1024 * 1024 - 1;
	char * data = new char[BUFF_SIZE + 1];
	data[BUFF_SIZE] = 0;

	while (true)
	{
		int rcv_cnt = TM1->recv_n(data, BUFF_SIZE);
		if (rcv_cnt < 0)
		{
			TM1->Close();
			TM1->Connect(address, port);
			continue;
		}
		else if (rcv_cnt == 0)
		{
			Sleep(500);
			continue;
		}

		data[rcv_cnt] = 0;
		//cout << "received data from pub :" << data << endl;
		fill_protocol_objects(data, rcv_cnt);
		
	}

	delete TM1;

	delete[] data;

	return 0;
}

//REQ: send && rep
int req_thread_proc()
{
	int timeout = 5000;

	const int BUFFER_SIZE = 4 * 1024 * 1024 - 1;
	char * buffer = new char[BUFFER_SIZE + 1];
	buffer[BUFFER_SIZE] = 0;

	ZmqReplyer * TM2 = new ZmqReplyer(g_context);
	
	char address[128];
	int port = 0;
	memset(address, 0, sizeof(address));

	Getaddress("req", address, port); 

	TM2->Bind(address, port);

	while (true)
	{
		int rcv_cnt = TM2->recv_n(buffer, BUFFER_SIZE);
		if (rcv_cnt < 0)
		{
			SLOG1("server error!!!!\n");
			printf("server error!!!!\n");
			TM2->Close();
			TM2->Bind(address, port);
			continue;
		}
		else if (0 == rcv_cnt)
		{
			Sleep(500);
			continue;
		}
		buffer[rcv_cnt] = 0;

		dispatch_request_proc(buffer, rcv_cnt, TM2, peer_address, peer_port, timeout);
	};

	delete[] buffer;

	return 0;
}

string GetVersionName(){
	FILE * fp = fopen("MTCP_Rosaline.h", "r");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	char* m_data = new char[1024];
	fseek(fp, 0, SEEK_SET);
	fread(m_data, length, 1, fp);
	fclose(fp);

	boost::regex pattern1("(MTCP_Rosaline_v)(.*)(h\")");
	boost::regex pattern(".*(MTCP_Rosaline_v.*.h).*");

	std::string str = m_data;

	char* match = new char[1024];
	boost::smatch m;
	bool ac1 = regex_match(str, m, pattern);
	return m[1];
	
}

void GetVersionTime(){
	char logdata[32 + 4096 + 1];
	TCHAR exeFullPath[300];
	memset(exeFullPath, 0, 300);
	GetModuleFileName(NULL, exeFullPath, 300);
	WIN32_FIND_DATA filestruct;
	HANDLE hf;
	hf = FindFirstFile(exeFullPath, &filestruct);
	filestruct.ftCreationTime;
	filestruct.ftLastAccessTime;
	FILETIME ft = filestruct.ftLastWriteTime;
	CString tmp;
	CTime time(ft);
	tmp.Format(L"%d-%d-%d,%d:%d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	sprintf(logdata, "Version : %s\r\n", GetVersionName().c_str());
	printf(logdata);
	SLOG1(logdata);
	sprintf(logdata, "Last edit time : %S\r\n", (char*)tmp.GetBuffer());
	printf(logdata);
	SLOG1(logdata);
}

unsigned short GetCrc16(unsigned char* InStr, unsigned int len){
	//生成Crc16的查询表     
	unsigned short Crc16Table[256];
	unsigned int i, j;
	unsigned short Crc;
	for (i = 0; i < 256; i++)
	{
		Crc = i;
		for (j = 0; j < 8; j++)
		{
			if (Crc & 0x1)
				Crc = (Crc >> 1) ^ 0x9b09;
				//Crc = (Crc >> 1) ^ 0xA001;
				//Crc = (Crc >> 1) ^ 0x0000;
			else
				Crc >>= 1;
		}
		Crc16Table[i] = Crc;
	}

	//开始计算CRC16校验值     
	//Crc = 0xffff;
	Crc = 0x0000;
	for (i = 0; i<len; i++){
		Crc = (Crc >> 8) ^ Crc16Table[(Crc & 0xFF) ^ InStr[i]];

	}
	//Crc ^= 0x0000;       
	return Crc;
}
int main(int argc, char * argv[])
{
	unsigned char cc[] = {0x00,0xff,0x77,0xc7,0xa3,0xd0,0x23,0x0f,0xda,0x82,0x8b,0x00,0x53,0x62,0xa7,0x80};
	unsigned char cc1[] = { 0xda, 0x82, 0x8b, 0x00, 0x53, 0x62, 0xa7, 0x80, 0x00, 0xff, 0x77, 0xc7, 0xa3, 0xd0, 0x23, 0x0f };
	unsigned char cc2[] = { 0x8b, 0x00, 0x53, 0x62, 0xa7, 0x80, 0x00, 0xff, 0x77, 0xc7, 0xa3, 0xd0, 0x23, 0x0f, 0xda, 0x82};
	unsigned char cc3[] = { 0x23, 0x0f, 0xda, 0x82, 0x8b, 0x00, 0x53, 0x62, 0x80 };
	unsigned char cc4[] = { 0x00, 0xff, 0x77, 0xc7, 0xa3, 0xd0, 0x23, 0x0f, 0xda, 0x82, 0x8b, 0x00, 0x53, 0x80 };


	unsigned short crc1 = GetCrc16(cc4, 14);
	unsigned short crc = GetCrc16(cc, 16);
	unsigned short crc3 = GetCrc16(cc3, 8);


	memset((void*)peer_address, 0, sizeof(peer_address));
	
	GetVersionTime();
	
	if (3 == argc)
	{
		sprintf_s(peer_address, "%s", argv[1]);
		peer_port = atoi(argv[2]);
	}
	else
	{
		Getaddress("req_napa_port7", peer_address, peer_port);
	}
	choose_Port = peer_port;

	SetGlobalCrashDump();

	g_context = zmq_ctx_new();
	if (NULL == g_context)
	{
		printf("create ZMQ Context failed!\n");
		return -1;
	}

	std::thread  sub_mt(sub_thread_proc);	
	std::thread	 req_mt(req_thread_proc);

	mp_service_run();

	sub_mt.join();
	req_mt.join();

	Close();

	DelGlobalCrashDump();
	
	zmq_ctx_destroy(g_context);

	return 0;
}







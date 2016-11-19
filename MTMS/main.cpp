#if 0
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

//pub/sub  req/rep
#include "ZmqSubscriber.h"
#include "ZmqReplyer.h"
#include "TMHandler.h"
using namespace std;

/*********** program log **************/
#include "CLog.h"
#define SLOG1(x)			CLog::Instance()->WriteLog(x);
#define SLOG2(x, y)			CLog::Instance()->WriteJoinLog(x, y);	
#define LOG1(x)				CLog::Instance()->WriteLog(__FILE__, __LINE__, x)
#define LOG2(x, y)			CLog::Instance()->WriteJoinLog(__FILE__, __LINE__, x, y)
/***********************************/
//queue for TM_to MTCP, storage Json message 
std::queue<char* > TM_to_MTCP;
//for thread 
mutex mtx;
//char* rspmsg_to_TM = new char[4096];

//mtcp thread
int create_mtcp_client()
{
	char address[32];
	int port = 61806;
	int timeout = 5000;
	long start = 0;
	long end = 0;
	long temp = 0;
	//char* AddressAdapter = new char;

	//read ip,port from load.xml

	Getaddress("req_napa_port7", address, port);
	Open(address, port, timeout);

	//receive datajson message from TM
	char* data = NULL; // new char[2048];
	char *type = new char[128];
	char *ID = new char[128];
	char *Val = new char[2048];
	int event = -1;
	tMTCP_payload_TEST_RSP ret;

	int req_flag = 0;//have req message, 1:yes 0:no  
	// protocol object
	TSCR tscr; TSED tsed; SEQU sequ; INIT init; VTST vtst; DPCK dpck; DPWR dpwr;
	DPWC dpwc; VCAL vcal; VDCR vdcr; C1SS c1ss; EMCT emct; BMPF bmpf; FLDP fldp;
	C1ES c1es; ALPR alpr; ALPH alph; POST post; FLDI fldi;
	YGTR ygtr; YGTS ygts; YGWC ygwc; YGWR ygwr;
	//log string
	char logdata[1024];
	memset(logdata, 0, 1024);


	//dpwr object return
	char* dpwr_ret = new char[2048];
	sprintf(logdata, "MTCP_PROTOCOL thread init success......");
	cout << logdata << endl;
	LOG1(logdata);
	while (true){
		
		// memset(data, 0, 2048);

		delete[] data;
		data = NULL;

		memset(type, 0, 128);
		memset(ID, 0, 128);
		memset(Val, 0, 2048);
		memset(&ret, 0, sizeof(tMTCP_payload_TEST_RSP));
		req_flag = 0;

		if (TM_to_MTCP.empty()){
			continue;
		}

		mtx.lock();
		data = TM_to_MTCP.front();
		// strcpy(data, TM_to_MTCP.front());
		TM_to_MTCP.pop();
		mtx.unlock();

		event = pub_json_to_mtcp(data, type, ID, Val);
		
		cout << "-------------**event: "<<event << endl;

		/*strcpy(AddressAdapter, PortAdapter("PortAdapter", type));
		Getaddress(AddressAdapter, address, port);
		Open(address, port, timeout);*/

		if (0 == strcmp(type, "TSCR")){
			start = GetCurrentTime();
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			tscr.setvalue(ID, Val);
			if (1 == event){
				tMTCP_payload_TSCR_RSP ret = tscr.SendTSCR();
				sprintf(logdata, "receive response message from napa....tscr_ret.HOST_SW_VERSION:%d", ret.HOST_SW_VERSION);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "TSED")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				ret = tsed.SendTSED();
				temp = GetCurrentTime() - start;
				sprintf(logdata, "receive response message from napa...tsed_ret.T_ERRC:%d,tsed_ret.T_ERRS:%s,cost time : %d"
					, ret.T_ERRC, ret.T_ERRS,temp);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "SEQU")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_SEQU_RSP ret = sequ.SendSEQU();
				sprintf(logdata, "receive response message from napa...sequ_ret.TEST_ITEM_CNT:%d,sequ_ret.TEST_CTRL_CODE:%s,sequ_ret:CHAMBER_CNT:%d,sequ_ret:CHAMBER_TO_RUN:%s"
					, ret.TEST_ITEM_CNT, ret.TEST_CTRL_CODE, ret.CHAMBER_CNT
					, sequ.CHAMBER_TO_RUN());
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "INIT")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			init.setvalue(ID, Val);
			if (1 == event){
				ret = init.SendINIT();
				sprintf(logdata, "receive response message from napa...init_ret.T_ERRC:%d,init_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "VTST")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (0 == strcmp(ID, "VTST_RAWDATA")){
				vtst.readFloatArr_fromFile(Val);
			}
			else{
				vtst.setvalue(ID, Val);
			}
			if (1 == event){
				ret = vtst.SendVTST();
				sprintf(logdata, "receive response message from napa...vtst_ret.T_ERRC:%d,vtst_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "DPCK")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			dpck.setvalue(ID, Val);
			if (1 == event){
				ret = dpck.SendDPCK();
				sprintf(logdata, "receive response message from napa...dpck_ret.T_ERRC:%d,dpck_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "DPWR")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_DPWR_RSP dpwr_ret = dpwr.SendDPWR();
				memset(&dpwr_ret, 0, sizeof(tMTCP_payload_DPWR_RSP));
				//dpwr_ret = dpwr.GetData();	
				sprintf(logdata, "receive response message from napa...dpwr_ret.CNT:%d,dpwr.ADDR:%d,dpwr.DATA:%d"
					, dpwr_ret.CNT, dpwr_ret.ADDR, dpwr_ret.DATA);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "DPWC")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			dpwc.setvalue(ID, Val);
			if (1 == event){
				ret = dpwc.SendDPWC();
				sprintf(logdata, "receive response message from napa...T_ERRC:%d,T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "VCAL")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (0 == strcmp(ID, "VCAL_TTDP")){
				vcal.setTTDP(Val);// val files path
			}
			else if (0 == strcmp(ID, "VCAL_ADIP")){
				vcal.setADIP(Val);// val; files path

			}
			else{
				vcal.setvalue(ID, Val);

			}
			if (1 == event){
				sprintf(logdata, "received message:%s", data);
				SLOG1(logdata);
				vcal.setAllRawdata();
				ret = vcal.SendVCAL();
				sprintf(logdata, "receive response message from napa...vcal_ret.T_ERRC:%d,vcal_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "VDCR")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_VDCR_RSP vdcr_ret = vdcr.SendVDCR();
				sprintf(logdata, "receive response message from napa...vdcr_ret.I_DR:%f,vdcr_ret.T_SETTLE:%f"
					, vdcr_ret.I_DR, vdcr_ret.T_SETTLE);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "C1SS")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (0 == strcmp(ID, "C1SS_RAWDATA")){
				//char* c1ss_rawdata = nullptr;
				c1ss.readFloatArr_fromFile(Val);

				if (1 == event){
					ret = c1ss.SendC1SS();
					sprintf(logdata, "receive response message from napa...c1ss_ret.T_ERRC:%d,c1ss_ret.T_ERRS:%s"
						, ret.T_ERRC, ret.T_ERRS);
					SLOG1(logdata);
				}

				continue;
			}
			else{
				c1ss.setvalue(ID, Val);
				if (1 == event){
					ret = c1ss.SendC1SS();
					sprintf(logdata, "receive response message from napa...c1ss_ret.T_ERRC:%d,c1ss_ret.T_ERRS:%s"
						, ret.T_ERRC, ret.T_ERRS);
					SLOG1(logdata);

				}
				continue;
			}
		}
		if (0 == strcmp(type, "EMCT")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			cout << "EMCT....ID:" << ID << endl;
			if (0 == strcmp(ID, "IMAGE_DATA")){
				int rawret = emct.setRawdata(Val);
				cout << "-------------------rawret :" << endl;
			}
			else{
				emct.setvalue(ID, Val);
			}
			if (1 == event){
				ret = emct.SendEMCT();
				sprintf(logdata, "receive response message from napa...emct_ret.T_ERRC:%d,emct_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "BMPF")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (0 == strcmp(ID, "IMAGE_DATA")){
				int rawret = bmpf.setRawdata(Val);
				cout << "setRawdata..." << rawret << endl;
			}
			else{
				bmpf.setvalue(ID, Val);
			}
			if (1 == event){
				ret = bmpf.SendBMPF();
				cout << "send bmpf success......." << endl;
				sprintf(logdata, "receive response message from napa...bmpf_ret.T_ERRC:%d,bmpf_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);

			}
			continue;
		}
		if (0 == strcmp(type, "FLDP")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			fldp.setvalue(ID, Val);
			if (1 == event){
				ret = fldp.SendFLDP();
				sprintf(logdata, "receive response message from napa...fldp_ret.T_ERRC:%d,fldp_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);

			}
			continue;
		}
		if (0 == strcmp(type, "C1ES")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			c1es.setvalue(ID, Val);
			if (1 == event){
				/*mtx.lock();
				Close();
				Getaddress("req_napa_port2", address, port);
				Open(address, port, timeout);
				mtx.unlock();*/
				ret = c1es.SendC1ES();
				sprintf(logdata, "receive response message from napa...c1es_ret.T_ERRC:%d,c1es_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "ALPR")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_ALPR_RSP alpr_ret = alpr.SendALPR();
				//tMTCP_payload_ALPR_RSP alpr_ret = alpr.Get_ALPR_RSP_MSG();
				sprintf(logdata, "receive response message from napa...alpr_ret.HOT_SPOT_PHI:%f,alpr_ret.HOT_SPOT_THETA:%f"
					, alpr_ret.HOT_SPOT_PHI, alpr_ret.HOT_SPOT_THETA);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "ALPH")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (0 == strcmp(ID, "ALPH_IMAGES")){
				cout << "ALPH_Val:....." << Val << endl;

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
				if (file_count < 1){
					continue;
				}
				cout << "-------------1:" << filename_arr[0] << endl;
				cout << "-------------2:" << filename_arr[1] << endl;
				cout << "-------------3:" << filename_arr[2] << endl;
				alph.setRawdata(file_count, filename_arr);
			}
			else{
				alph.setvalue(ID, Val);
				if (1 == event){
					ret = alph.SendALPH();
					sprintf(logdata, "receive response message from napa...alpr_ret.T_ERRC:%d,alpr_ret.T_ERRS:%s"
						, ret.T_ERRC, ret.T_ERRS);
					SLOG1(logdata);
				}
			}
			continue;
		}
		if (0 == strcmp(type, "POST")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_POST_RSP post_ret = post.SendPOST();
				sprintf(logdata, "receive response message from napa... post_ret.T_ERRC:%d,post_ret.T_ERRS:%s,post_ret.T_BIN:%d"
					, post_ret.T_ERRC, post_ret.T_ERRS, post_ret.T_BIN);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "FLDI")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (0 == strcmp(ID, "IMAGE_DATA")){
				fldi.setImageRawdata(Val);
			}
			else if (0 == strcmp(ID, "FLAT_DATA")){
				fldi.setFloatRawdata(Val);

			}
			else{
				fldi.setvalue(ID, Val);
				if (1 == event){
					fldi.setAllRawdata();
					ret = fldi.SendFLDI();
					sprintf(logdata, "receive response message from napa...fldi_ret.T_ERRC:%d,fldi_ret.T_ERRS:%s"
						, ret.T_ERRC, ret.T_ERRS);
					SLOG1(logdata);
				}
			}
			continue;
		}
		if (0 == strcmp(type, "YGTS")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			ygts.setvalue(ID, Val);
			if (1 == event){
				ret = ygts.SendYGTS();
				sprintf(logdata, "receive response message from napa...ygts_ret.T_ERRC:%d,ygts_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "YGWC")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			ygwc.setvalue(ID, Val);
			if (1 == event){
				ret = ygwc.SendYGWC();
				sprintf(logdata, "receive response message from napa...ygwc_ret.T_ERRC:%d,ygwc_ret.T_ERRS:%s"
					, ret.T_ERRC, ret.T_ERRS);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "YGTR")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_YGTR_RSP ygtr_ret = ygtr.SendYGTR();
				sprintf(logdata, "receive response message from napa...ygtr_ret.I_PULSEMAX:%f"
					, ygtr_ret.I_PULSEMAX);
				SLOG1(logdata);
			}
			continue;
		}
		if (0 == strcmp(type, "YGWR")){
			sprintf(logdata, "received message:%s", data);
			SLOG1(logdata);
			if (1 == event){
				tMTCP_payload_YGWR_RSP ygwr_ret = ygwr.SendYGWR();
				sprintf(logdata, "receive response message from napa...ygwr_ret.CNT:%s,ygwr_ret.CNT:%s,ygwr_ret.CNT:%s"
					, ygwr_ret.CNT, ygwr_ret.ADDR, ygwr_ret.DATA);
				SLOG1(logdata);
			}
			continue;
		}
		

	}
	Close();
	return 0;
}

int sub_thread()
{
	char *address = new char[64];
	memset(address, 0, 64);
	int port = 0;
	ZmqSubscriber *TM1 = new ZmqSubscriber;
	//mtx.lock();
	Getaddress("pub", address, port);
	TM1->Connect(address, port);
	//mtx.unlock();
	char *data = NULL;// new char[4096];
	
	// char logdata[1050];
	int length = 0;

	while (1)
	{
		data = new char[2048];
		memset(data, 0, 2048);

		// memset(logdata, 0, 1050);
		//cout << "dd" << endl;
		
		TM1->resv(data, length);

		mtx.lock();
		TM_to_MTCP.push(data);
		mtx.unlock();

		std::cout << "receive message from TM PUB : " << data << std::endl;
	}
	return 0;
}

//req/rep thread
int req_thread()
{
	TSCR tscr; TSED tsed; SEQU sequ; INIT init; VTST vtst; DPCK dpck; DPWR dpwr;
	DPWC dpwc; VCAL vcal; VDCR vdcr; C1SS c1ss; EMCT emct; BMPF bmpf; FLDP fldp;
	C1ES c1es; ALPR alpr; ALPH alph; POST post; FLDI fldi;
	YGTS ygts; YGWC ygwc; YGTR ygtr; YGWR ygwr;

	char* req_msg = new char[4096];
	tMTCP_payload_TEST_RSP ret;
	char *address = new char[128];
	memset(address, 0, 64);
	int port = 0;
	char* dpwr_ret = new char[2048];
	ZmqReplyer *TM2 = new ZmqReplyer;
	//mtx.lock();
	Getaddress("req", address, port);
	TM2->Bind(address, port);
	//mtx.unlock();
	char *logdata = new char[32 + strlen(req_msg) + 1];
	int length = 0;
	int i = 1;
	char *type = new char[128];
	char *ID = new char[128];
	char *result = new char[1024];
	char *sendresult = new char[1024];
	int _errc = 0;
	char *_errs = new char[1024];
	/*********reconect napa*********/

	/******************/
	while (1){
		memset(type, 0, 128);
		memset(ID, 0, 128);
		memset(req_msg, 0, 4096);
		memset(result, 0, 1024);
		memset(sendresult, 0, 1024);
		memset(_errs, 0, 1024);
		memset(dpwr_ret, 0, 2048);

		TM2->resv(req_msg);

		/***********  log  **************/
		if (strlen(req_msg) == 0){
			Sleep(100);
			continue;
		}
		std::cout << "req_thread...receive message:" << req_msg << std::endl;
		memset(logdata, 0, 32 + strlen(req_msg) + 1);
		sprintf(logdata, "received message from TM_req:%s", req_msg);
		SLOG1(logdata);
		/********************************/

		//mtx.lock();
		req_json_to_mtcp(req_msg, type, ID);
		//mtx.unlock();
		//cout << type << endl;
		//system("pause");
		if (0 == strcmp(type, "ALPR"))
		{
			tMTCP_payload_ALPR_RSP alpr_ret = alpr.SendALPR();
			//tMTCP_payload_ALPR_RSP alpr_ret = alpr.Get_ALPR_RSP_MSG();
			sprintf(logdata, "receive response message from napa...alpr_ret.HOT_SPOT_PHI:%f,alpr_ret.HOT_SPOT_THETA:%f"
				, alpr_ret.HOT_SPOT_PHI, alpr_ret.HOT_SPOT_THETA);
			SLOG1(logdata);
			if (0 == strcmp(ID, "HOT_SPOT_THETA"))
			{
				sprintf(result, "%f", alpr_ret.HOT_SPOT_THETA);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "HOT_SPOT_PHI"))
			{
				sprintf(result, "%f", alpr_ret.HOT_SPOT_PHI);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
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
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "TEST_CTRL_CODE"))
			{
				sprintf(result, "%s", ret.TEST_CTRL_CODE);
				//_ultoa(ret.TEST_CTRL_CODE, result, 10);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "CHAMBER_CNT"))
			{
				_ultoa(ret.CHAMBER_CNT, result, 10);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "CHAMBER_TO_RUN"))
			{
				//result = (char*)sequ.CHAMBER_TO_RUN();
				sprintf(result, "%s", temp);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
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
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "T_ERRS"))
			{
				//result = (char *)post_ret.T_ERRS;
				sprintf(result, "%s", post_ret.T_ERRS);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "T_BIN"))
			{
				_ultoa(post_ret.T_BIN, result, 10);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}


		}
		else if (0 == strcmp(type, "TSCR"))
		{
			//Open(address, port, timeout);
			tMTCP_payload_TSCR_RSP ret = tscr.SendTSCR();
			sprintf(logdata, "receive response message from napa....tscr_ret.HOST_SW_VERSION:%s", ret.HOST_SW_VERSION);
			SLOG1(logdata);
			if (0 == strcmp(ID, "HOST_SW_VERSION"))
			{
				//result = (char *)ret.HOST_SW_VERSION;
				sprintf(result, "%s", ret.HOST_SW_VERSION);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}


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
				int res = TM2->send(sendresult);
				long end1 = GetCurrentTime() - start;
				sprintf(logdata, "receive response message cost time 1: %d,result : %d,sendresult:%s"
					, end1, res, sendresult);
			}
			else if (0 == strcmp(ID, "T_SETTLE"))
			{
				sprintf(result, "%f", vdcr_ret.T_SETTLE);
				mtcp_to_json(result, sendresult);
				//start = GetCurrentTime();
				int res1 = TM2->send(sendresult);
				long end2 = GetCurrentTime() - start;
				sprintf(logdata, "receive response message cost time 1: %d,result : %d,sendresult:%s"
					, end2, res1, sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}

		}
		else if (0 == strcmp(type, "TSED")){
			ret = tsed.SendTSED();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			//_errs = (char*)ret.T_ERRS;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "INIT")){
			ret = init.SendINIT();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "VTST")){
			ret = vtst.SendVTST();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "DPCK")){
			ret = dpck.SendDPCK();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "DPWR"))
		{
			tMTCP_payload_DPWR_RSP ret = dpwr.SendDPWR();
			memset(dpwr_ret, 0, 2048);
			dpwr_ret = dpwr.GetData();
			sprintf(logdata, "receive response message from napa...dpwr_ret.CNT:%d,dpwr_ret.ADDR:%d,dpwr_ret.DATA:%d"
				, ret.CNT, ret.ADDR, ret.DATA);
			SLOG1(logdata);
			if (0 == strcmp(ID, "CNT"))
			{
				sprintf(result, "%d", ret.CNT);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "ADDR"))
			{
				sprintf(result, "%d", ret.ADDR);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "DATA"))
			{
				sprintf(result, "%d", ret.DATA);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}

		}
		else if (0 == strcmp(type, "DPWC")){
			ret = dpwc.SendDPWC();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "VCAL")){
			ret = vcal.SendVCAL();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "C1SS")){
			ret = vtst.SendVTST();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "EMCT")){
			ret = emct.SendEMCT();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			//_errs = (char*)ret.T_ERRS;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "BMPF")){
			ret = bmpf.SendBMPF();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "FLDP")){
			ret = fldp.SendFLDP();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "C1ES")){
			ret = c1es.SendC1ES();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "ALPH")){
			ret = alph.SendALPH();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "FLDI")){
			ret = fldi.SendFLDI();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "YGTS")){
			ret = ygts.SendYGTS();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);

		}
		else if (0 == strcmp(type, "YGWC")){
			ret = ygwc.SendYGWC();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "YGTR"))
		{
			tMTCP_payload_YGTR_RSP ygtr_ret = ygtr.SendYGTR();
			sprintf(logdata, "receive response message from napa...ygtr_ret.I_PULSEMAX:%f"
				, ygtr_ret.I_PULSEMAX);
			SLOG1(logdata);
			if (0 == strcmp(ID, "I_PULSEMAX"))
			{
				sprintf(result, "%f", ygtr_ret.I_PULSEMAX);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
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
				sprintf(result, "%s", ygwr_ret.CNT);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "ADDR"))
			{
				sprintf(result, "%s", ygwr_ret.ADDR);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "DATA"))
			{
				sprintf(result, "%s", ygwr_ret.DATA);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}
		}
		else
		{
			char buf[] = "Message type error!";
			mtcp_to_json(buf, sendresult);
			TM2->send(sendresult);
			sprintf(logdata, "send message to TM...error_str:%s", buf);
			SLOG1(logdata);
			continue;
		}

		//YGTS ygts; YGWC ygwc; YGTR ygtr; YGWR ygwr;
		//string buf = "{\"result\":\"ok\"}";
		//TM2->send(buf.c_str(), length);

		/***********  log  **************/
		memset(logdata, 0, (32 + strlen(sendresult) + 1));
		sprintf(logdata, "send message to TM_req:%s", sendresult);
		SLOG1(logdata);
		/********************************/
	}
	delete[] req_msg;
	req_msg = nullptr;
	delete[] address;
	address = nullptr;;
	delete[] dpwr_ret;
	dpwr_ret = nullptr;
	delete[] type;
	type = nullptr;
	delete[] ID;
	ID = nullptr;
	delete[] result;
	result = nullptr;
	delete[] sendresult;
	sendresult = nullptr;
	delete[] _errs;
	_errs = nullptr;
	return 0;
}



int req_thread_proc()
{
	TSCR tscr; TSED tsed; SEQU sequ; INIT init; VTST vtst; DPCK dpck; DPWR dpwr;
	DPWC dpwc; VCAL vcal; VDCR vdcr; C1SS c1ss; EMCT emct; BMPF bmpf; FLDP fldp;
	C1ES c1es; ALPR alpr; ALPH alph; POST post; FLDI fldi;
	YGTS ygts; YGWC ygwc; YGTR ygtr; YGWR ygwr;

	char req_msg[4096];
	tMTCP_payload_TEST_RSP ret;

	char address[128];
	memset(address, 0, sizeof(address));
	int port = 0;

	char dpwr_ret[2048];
	
	ZmqReplyer * TM2 = new ZmqReplyer;
	
	//mtx.lock();
	Getaddress("req", address, port);
	TM2->Bind(address, port);
	//mtx.unlock();

	char logdata[32 + 4096 + 1];
	int length = 0;
	int i = 1;

	char type[128];
	char ID[128];
	char result[1024];
	char sendresult[1024];
	int _errc = 0;
	char _errs[1024];
	/*********reconect napa*********/

	/******************/
	while (1){
		memset(type, 0, 128);
		memset(ID, 0, 128);
		memset(req_msg, 0, 4096);
		memset(result, 0, 1024);
		memset(sendresult, 0, 1024);
		memset(_errs, 0, 1024);
		memset(dpwr_ret, 0, 2048);

		int rcv_cnt = TM2->recv_n(req_msg, 4096);
		if (rcv_cnt < 0)
		{
			SLOG1("server error!!!!\n");
			printf("server error!!!!\n");
			exit(0);
		}

		else if (0 == rcv_cnt)
		{
			Sleep(500);
			continue;
		}

		/***********  log  **************/
		//if (strlen(req_msg) == 0){
		//	Sleep(100);
		//	continue;
		//}

		std::cout << "req_thread...receive message:" << req_msg << std::endl;
		//memset(logdata, 0, 32 + strlen(req_msg) + 1);
		sprintf(logdata, "received message from TM_req:%s", req_msg);
		SLOG1(logdata);
		/********************************/


		printf("====================================================================rcv[%d]:%s===================================\n",rcv_cnt,req_msg);

		//mtx.lock();
		req_json_to_mtcp(req_msg, type, ID);
		//mtx.unlock();
		//cout << type << endl;
		//system("pause");
		if (0 == strcmp(type, "ALPR"))
		{
			tMTCP_payload_ALPR_RSP alpr_ret = alpr.SendALPR();
			//tMTCP_payload_ALPR_RSP alpr_ret = alpr.Get_ALPR_RSP_MSG();
			sprintf(logdata, "receive response message from napa...alpr_ret.HOT_SPOT_PHI:%f,alpr_ret.HOT_SPOT_THETA:%f"
				, alpr_ret.HOT_SPOT_PHI, alpr_ret.HOT_SPOT_THETA);
			SLOG1(logdata);
			if (0 == strcmp(ID, "HOT_SPOT_THETA"))
			{
				sprintf(result, "%f", alpr_ret.HOT_SPOT_THETA);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "HOT_SPOT_PHI"))
			{
				sprintf(result, "%f", alpr_ret.HOT_SPOT_PHI);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
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
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "TEST_CTRL_CODE"))
			{
				sprintf(result, "%s", ret.TEST_CTRL_CODE);
				//_ultoa(ret.TEST_CTRL_CODE, result, 10);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "CHAMBER_CNT"))
			{
				_ultoa(ret.CHAMBER_CNT, result, 10);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "CHAMBER_TO_RUN"))
			{
				//result = (char*)sequ.CHAMBER_TO_RUN();
				sprintf(result, "%s", temp);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
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
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "T_ERRS"))
			{
				//result = (char *)post_ret.T_ERRS;
				sprintf(result, "%s", post_ret.T_ERRS);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "T_BIN"))
			{
				_ultoa(post_ret.T_BIN, result, 10);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}


		}
		else if (0 == strcmp(type, "TSCR"))
		{
			//Open(address, port, timeout);
			tMTCP_payload_TSCR_RSP ret = tscr.SendTSCR();
			sprintf(logdata, "receive response message from napa....tscr_ret.HOST_SW_VERSION:%s", ret.HOST_SW_VERSION);
			SLOG1(logdata);
			if (0 == strcmp(ID, "HOST_SW_VERSION"))
			{
				//result = (char *)ret.HOST_SW_VERSION;
				sprintf(result, "%s", ret.HOST_SW_VERSION);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}


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
				int res = TM2->send(sendresult);
				long end1 = GetCurrentTime() - start;
				sprintf(logdata, "receive response message cost time 1: %d,result : %d,sendresult:%s"
					, end1, res, sendresult);
			}
			else if (0 == strcmp(ID, "T_SETTLE"))
			{
				sprintf(result, "%f", vdcr_ret.T_SETTLE);
				mtcp_to_json(result, sendresult);
				//start = GetCurrentTime();
				int res1 = TM2->send(sendresult);
				long end2 = GetCurrentTime() - start;
				sprintf(logdata, "receive response message cost time 1: %d,result : %d,sendresult:%s"
					, end2, res1, sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}

		}
		else if (0 == strcmp(type, "TSED")){
			ret = tsed.SendTSED();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			//_errs = (char*)ret.T_ERRS;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "INIT")){
			ret = init.SendINIT();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "VTST")){
			ret = vtst.SendVTST();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "DPCK")){
			ret = dpck.SendDPCK();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "DPWR"))
		{
			tMTCP_payload_DPWR_RSP ret = dpwr.SendDPWR();
			// memset(dpwr_ret, 0, 2048);
			// dpwr_ret = dpwr.GetData();
			sprintf(logdata, "receive response message from napa...dpwr_ret.CNT:%d,dpwr_ret.ADDR:%d,dpwr_ret.DATA:%d"
				, ret.CNT, ret.ADDR, ret.DATA);
			SLOG1(logdata);
			if (0 == strcmp(ID, "CNT"))
			{
				sprintf(result, "%d", ret.CNT);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "ADDR"))
			{
				sprintf(result, "%d", ret.ADDR);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "DATA"))
			{
				sprintf(result, "%d", ret.DATA);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}

		}
		else if (0 == strcmp(type, "DPWC")){
			ret = dpwc.SendDPWC();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "VCAL")){
			ret = vcal.SendVCAL();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "C1SS")){
			ret = vtst.SendVTST();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "EMCT")){
			ret = emct.SendEMCT();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			//_errs = (char*)ret.T_ERRS;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "BMPF")){
			ret = bmpf.SendBMPF();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "FLDP")){
			ret = fldp.SendFLDP();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "C1ES")){
			ret = c1es.SendC1ES();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "ALPH")){
			ret = alph.SendALPH();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "FLDI")){
			ret = fldi.SendFLDI();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "YGTS")){
			ret = ygts.SendYGTS();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);

		}
		else if (0 == strcmp(type, "YGWC")){
			ret = ygwc.SendYGWC();
			sprintf(logdata, "receive response message from napa...error_code:%d,error_str:%s"
				, ret.T_ERRC, ret.T_ERRS);
			SLOG1(logdata);
			_errc = ret.T_ERRC;
			sprintf(_errs, "%s", ret.T_ERRS);
			mtcp_to_json(_errc, _errs, sendresult);
			TM2->send(sendresult);
		}
		else if (0 == strcmp(type, "YGTR"))
		{
			tMTCP_payload_YGTR_RSP ygtr_ret = ygtr.SendYGTR();
			sprintf(logdata, "receive response message from napa...ygtr_ret.I_PULSEMAX:%f"
				, ygtr_ret.I_PULSEMAX);
			SLOG1(logdata);
			if (0 == strcmp(ID, "I_PULSEMAX"))
			{
				sprintf(result, "%f", ygtr_ret.I_PULSEMAX);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
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
				sprintf(result, "%s", ygwr_ret.CNT);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "ADDR"))
			{
				sprintf(result, "%s", ygwr_ret.ADDR);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else if (0 == strcmp(ID, "DATA"))
			{
				sprintf(result, "%s", ygwr_ret.DATA);
				mtcp_to_json(result, sendresult);
				TM2->send(sendresult);
			}
			else
			{
				char buf[] = "Message type error!";
				mtcp_to_json(buf, sendresult);
				TM2->send(sendresult);
				sprintf(logdata, "send message to TM...error_str:%s", buf);
				SLOG1(logdata);
				continue;
			}
		}
		else
		{
			char buf[] = "Message type error!";
			mtcp_to_json(buf, sendresult);
			TM2->send(sendresult);
			sprintf(logdata, "send message to TM...error_str:%s", buf);
			SLOG1(logdata);
			continue;
		}

		//YGTS ygts; YGWC ygwc; YGTR ygtr; YGWR ygwr;
		//string buf = "{\"result\":\"ok\"}";
		//TM2->send(buf.c_str(), length);

		/***********  log  **************/
		memset(logdata, 0, (32 + strlen(sendresult) + 1));
		sprintf(logdata, "send message to TM_req:%s", sendresult);
		SLOG1(logdata);
		/********************************/
	}
	//delete[] req_msg;
	//req_msg = nullptr;
	//delete[] address;
	//address = nullptr;;
	//delete[] dpwr_ret;
	//dpwr_ret = nullptr;
	//delete[] type;
	//type = nullptr;
	//delete[] ID;
	//ID = nullptr;
	//delete[] result;
	//result = nullptr;
	//delete[] sendresult;
	//sendresult = nullptr;
	//delete[] _errs;
	//_errs = nullptr;
	return 0;
}


int old_main()
{

	std::thread mt(create_mtcp_client);
	
	std::thread tm1(sub_thread);


	 // std::thread tm2(req_thread);
	 // std::thread tm2(req_thread_proc);

	mt.join();
	
	tm1.join();
	
	//tm2.join();

	return 0;
}

#endif // 0

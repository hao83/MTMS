#ifndef	__MTCP_MODULE__H__
#define __MTCP_MODULE__H__
#pragma once

#include "FrameBase.h"
#include "TMHandler.h"
#include "MTCPConnection.h"



//socket
extern int Open(const char*  address = "127.0.0.1", int port = MTCP_PORT_DEFAULT, int timeout = 5000);
extern void Close();

extern const char*  getVersion();

class CTSS :public CFrameBase
{
public:
	CTSS();
	~CTSS();

	int NTC_STEADY_STATE(float v);
	//int RSVD(const char* v);
	//int I_DR(float v);
	int I_FORWARD(const char*  v);
	int V_FORWARD(const char*  v);
	int PEAK_OPTICAL_POWER(const char* v);
	//int PEAK_WAVELENGTH(float v);
	//int FWHM_WAVELENGTH(float v);
	//int WL_DATA_LEN(float v);

	//int readFloatArr_fromFile(char *filename);
	//int pushData(const float d1[], const float d2[]);
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendCTSS();
	//int fillRawdata(char* filename);

private:
	tMTCP_payload_CTSS_REQ m_CTSS_REQ;
	//tMTCP_data_TTDP * m_pdata;//rawdata
	//long rawdata_len;
	//std::string _filepath;
};


class DPCK :public CFrameBase
{
public:
	DPCK();
	~DPCK();
	int C_I_IDLE_LOW(float v);
	int C_NVM_DATA(const char*  d);	//1,2,3,4,5
	int Y_DEVICE_ID_H(uint8_t d);
	int Y_DEVICE_ID_L(uint8_t d);
	int Y_REVISION_MAJOR(uint8_t d);
	int Y_REVISION_MINOR(uint8_t d);
	int Y_TRACE_ID(const char* d);
	int Y_I_ACTIVE(float v);

	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendDPCK();

private:
	tMTCP_payload_DPCK_REQ m_DPCK_REQ;
};


class DPWC :CFrameBase
{
public:
	DPWC();
	~DPWC();
	int NVM_DATA(const char*  data);

	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendDPWC();

private:
	tMTCP_payload_DPWC_REQ m_DPWC_REQ;
};



class DPWR :public CFrameBase
{
public:
	DPWR();
	~DPWR();

	char*  GetData();
	tMTCP_payload_DPWR_RSP SendDPWR();
private:
	tMTCP_payload_DPWR_RSP m_Rep;
	char  m_buffer[2048];
};

class TSCR :public CFrameBase
{
public:
	TSCR();
	~TSCR();

	int setDUTSN(const char*  sn);
	int setSocketSN(const char* sn);
	int setMode(int mode);
	int setSlotID(int slot);
	tMTCP_payload_TSCR_RSP SendTSCR();
	int setStationID(int station);
	int setSWVersion(const char* sw = "V1.0");

	int setvalue(char*  what, char*  value);
	int setSOCKETDCR(float dcr);

private:
	tMTCP_payload_TSCR_REQ m_TSCR_REQ;
};


class VCAL :public CFrameBase
{
public:
	VCAL();
	~VCAL();


	int NTC_STEADY_STATE(float v);
	int T_STEADY_STATE(float v);
	int PEAK_OPTICAL_POWER_FINAL(float v);
	int I_PULSE_FINAL(float v);
	int V_PULSE_FINAL(float v);
	int PEAK_WAVELENGTH(float v);
	int FWHM_WAVELENGTH(float v);
	int WL_DATA_LEN(int v);
	int ITER_CNT(int v);

	int pushData(float I, float v, float power, const char*  v_pulse_raw, float power_raw);

	int setTTDP(char* filename);// wavelength file,measure file
	int setADIP(char * filename);
	int setAllRawdata();
	int real_setTTDP(char* filename); //return -1 read fail,return 0 read success
	int real_setADIP(char* filename);
	int fill_ALL_Rawdata();

	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendVCAL();
private:
	tMTCP_payload_VCAL_REQ m_VCAL_REQ;
	tMTCP_data_TTDP * m_TTDPData;
	tMTCP_data_ADIP * m_ADIPData;
	char * allRawdata;
	long TTDP_len;
	long ADIP_len;
	long all_len; // 0 or length of rawdata
	std::string ttdp_Path;
	std::string adip_Path;

};

class VDCR :public CFrameBase
{
public:
	VDCR();
	~VDCR();

	float I_DR();
	float T_SETTLE();


	tMTCP_payload_VDCR_RSP SendVDCR();
private:
	tMTCP_payload_VDCR_RSP m_Rep;
};

class VTSTPOST :public CFrameBase
{
public:
	VTSTPOST();
	~VTSTPOST();

	int MAGIC(uint8_t v);
	int SOCKET_DCR(float v);
	int PEAK_OPTICAL_POWER(const char*  data);
	int PEAK_OPTICAL_POWER_0(float data);
	int FORWARD_V(const char*  data);
	int FORWARD_I(const char*  data);
	int CENTROID_WAVELENGTH(const char*  data);
	int FWHM_WAVELENGTH(const char*  data);
	int CENTROID_WAVELENGTH_0(float v);
	int FWHM_WAVELENGTH_0(float v);
	int NTC_STEADY_STATE(float v);
	int WL_DATA_CNT(uint8_t v);
	int WL_LEN(int v);

	//int setPayload(tMTCP_payload_VTST_REQ payload);
	//int pushData(const char*  d1, const char*  d2);
	int pushData(const float d1[], const float d2[]);

	int setRawdata(char* filename);
	int fillRawdata(char * filename);


	int pushWaveLength(const char*  wave);
	int readFloatArr_fromFile(char *filename);
	int setvalue(char*  what, char*  value);

	tMTCP_payload_TEST_RSP SendVTSTPOST();

private:
	tMTCP_payload_VTST_POSTDA_REQ m_VTST_POSTDA_REQ;
	tMTCP_data_TTDP * m_pdata;
	long rawdata_len; // 0 or length of m_data
	char * m_data;
	std::string _filepath;
};

class VTST :public CFrameBase
{
public:
	VTST();
	virtual ~VTST();

	int MAGIC(uint8_t v);
	int NTC_INIT(float v);
	int SOCKET_DCR(float v);
	int PEAK_OPTICAL_POWER(const char*  data);
	int FORWARD_V(const char*  data);
	int FORWARD_I(const char*  data);
	int CENTROID_WAVELENGTH(const char*  data);
	int FWHM_WAVELENGTH(const char*  data);
	int WL_DATA_CNT(uint8_t v);
	int WL_LEN(uint16_t v);
	//int setPayload(tMTCP_payload_VTST_REQ payload);
	//int pushData(const char*  d1, const char*  d2);
	int pushData(const float d1[], const float d2[]);

	int pushWaveLength(const char*  wave);
	int readFloatArr_fromFile(char *filename);
	int setvalue(char*  what, char*  value);

	int VTST::setRawdata(char* filename);
	int VTST::fillRawdata(char * filename);


	tMTCP_payload_TEST_RSP SendVTST();

private:
	tMTCP_payload_VTST_PREDA_REQ m_VTST_PREDA_REQ;
	tMTCP_data_TTDP * m_pdata;
	long rawdata_len; // 0 or length of m_data
	char* m_data;
	std::string _filepath;
};


class TSED :public CFrameBase
{
public:
	TSED();
	virtual ~TSED();

	tMTCP_payload_TEST_RSP SendTSED();
private:

};

///////////////////////////////////////////////////////////////////
//Chamber2
//////////////////////////////////////////////////////////////////
class EMCT :public CFrameBase
{
	enum{ BUFFER_SIZE = 50 * 1024 * 1024 - 1 };

public:
	EMCT();
	virtual ~EMCT();

public:
	int IMG_WIDTH(unsigned short v);
	int IMG_HEIGHT(unsigned short v);
	int IMG_SIZE(unsigned int v);
	int CLUSTER_THRESHOLD(float v);
	int VALID_EMITTER_CNT(unsigned char  v);
	int EMITTER_CENTER_X(const char*  data);
	int EMITTER_CENTER_Y(const char*  data);
	int EMITTER_SIZE(const char*  data);
	int UNIF_MAX(float  data);
	int UNIF_MIN(float  data);
	int UNIF_STD(float  data);
	int SOCKET_DCR(float  data);
	int FORWARD_V(const char* v);
	int FORWARD_I(const char* v);

	int setRawdata(char* filename);
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendEMCT();

protected:
	int real_setRawdata();

private:
	tMTCP_payload_EMCT_REQ m_EMCT_REQ;
	char * m_data;
	int		m_size;
	int		m_length;

	std::string m_file;
};


//////////////////////////////////////////////////
//Chamber3
/////////////////////////////////////////////////
class BMPF :public CFrameBase
{
	enum{ BUFFER_SIZE = 50 * 1024 * 1024 - 1};

public:
	BMPF();
	virtual ~BMPF();

public:
	int IMG_WIDTH(unsigned short v);
	int IMG_HEIGHT(unsigned short v);
	int IMG_SIZE(unsigned short v);
	int TOTAL_OPTICAL_POWER(float v);
	int BEAM_SIZE(float v);
	int HOT_SPOT_POWER(float v);
	int HOT_SPOT_LOC_X(unsigned short v);
	int HOT_SPOT_LOC_Y(unsigned short v);
	int SOCKET_DCR(float v);
	int FORWARD_V(const char* v);
	int FORWARD_I(const char* v);

	int setRawdata(char* filename); //return -1 read fail,return 0 read success
	int setvalue(char*  what, char*  value);

	tMTCP_payload_TEST_RSP SendBMPF();

protected:
	int real_setImgData();
	int real_setRawData();

private:
	tMTCP_payload_BMPF_REQ m_BMPF_REQ;
	
	std::string		m_file;

	char *			m_data;
	int				m_size;
	int				m_length;;
};


//////////////////////////////////////////////////
// POST
/////////////////////////////////////////////////
class POST : public CFrameBase
{
public:
	POST();
	virtual ~POST();

	int T_BIN()
	{
		return _rsp.T_BIN;
	}

	int T_ERRC()
	{
		return _rsp.T_ERRC;
	}

	const char*  T_ERRS()
	{
		return (const char*)_rsp.T_ERRS;
	}

	tMTCP_payload_POST_RSP SendPOST();
private:
	tMTCP_payload_POST_RSP	_rsp;
};



//////////////////////////////////////////////////
//void set_IMG_CNT(uint8_t IMG_CNT){ _req.IMG_CNT = IMG_CNT; }
//////////////////////////////////////////////////
//#define SET_PRO(type, name) void set_##name(type name){ _req.##name = name; }
#define SET_PRO(type, name) void name(type name){ _req.##name = name; }
#define SET_ARRAY_PRO(type, name, size) void set_##name(type name[size]){for (int i = 0; i < size; i++)_req.##name[i] = name[i];}

//////////////////////////////////////////////////
// CTES
/////////////////////////////////////////////////
class CTES : public CFrameBase
{
public:
	CTES();
	~CTES();


public:
	SET_PRO(int, IMG_CNT);
	//SET_PRO(float, NTC_STEADY_STATE);
	SET_PRO(float, SCREEN_REFLECTANCE);
	SET_PRO(float, HOT_SPOT_THETA);
	SET_PRO(float, HOT_SPOT_PHI);
	SET_PRO(float, AE_POWER);
	SET_PRO(float, OPTICAL_POWER);

public:
	int get_payload_CTES();
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendCTES();

private:
	tMTCP_payload_CTES_REQ		_req;
};


//////////////////////////////////////////////////
// FLDP
/////////////////////////////////////////////////
class FLDP : public CFrameBase
{
public:
	FLDP();
	~FLDP();

public:
	SET_PRO(int, IMG_CNT);
	//SET_PRO(float, NTC_STEADY_STATE);
	SET_PRO(float, SCREEN_REFLECTANCE);
	SET_PRO(float, POWER_IN_FOV);
	SET_PRO(float, POWER_EFFICIENCY);
	SET_PRO(float, HFNU);
	SET_PRO(float, SOCKET_DCR);
	//SET_PRO(float, I_DR);
	//SET_PRO(float, V_FOR);


	int LFNU_P_C(const char*  data)
	{
		char*  p = new char[strlen(data) + 1];
		strcpy(p, data);

		char*  dd;
		char*  k = strtok(p, ",");
		int index = 0;
		while (k)
		{
			_req.LFNU_P_C[index++] = atof(k);
			if (index > 9) break;
			k = strtok(NULL, ",");
		}
		delete[] p;
		return 0;
	}

	int OPTICAL_POWER_MAX(const char* data)	// Optical power max for each region
	{
		char*  p = new char[strlen(data) + 1];
		strcpy(p, data);

		char*  dd;
		char*  k = strtok(p, ",");
		int index = 0;
		while (k)
		{
			_req.OPTICAL_POWER_MAX[index++] = atof(k);
			if (index > 9) break;
			k = strtok(NULL, ",");
		}
		delete[] p;
		return 0;
	}

	int OPTICAL_POWER_MIN(const char* data)	// optical power min for each region
	{
		char*  p = new char[strlen(data) + 1];
		strcpy(p, data);

		char*  dd;
		char*  k = strtok(p, ",");
		int index = 0;
		while (k)
		{
			_req.OPTICAL_POWER_MIN[index++] = atof(k);
			if (index > 9) break;
			k = strtok(NULL, ",");
		}
		delete[] p;
		return 0;
	}

	int OPTICAL_POWER_AVG(const char* data)
	{
		char*  p = new char[strlen(data) + 1];
		strcpy(p, data);

		char*  dd;
		char*  k = strtok(p, ",");
		int index = 0;
		while (k)
		{
			_req.OPTICAL_POWER_AVG[index++] = atof(k);
			if (index > 9) break;
			k = strtok(NULL, ",");
		}
		delete[] p;
		return 0;
	}

	int FORWARD_V(const char* data)
	{
		char*  p = new char[strlen(data) + 1];
		strcpy(p, data);

		char*  dd;
		char*  k = strtok(p, ",");
		int index = 0;
		while (k)
		{
			_req.FORWARD_V[index++] = atof(k);
			if (index > 10) break;
			k = strtok(NULL, ",");
		}
		delete[] p;
		return 0;
	}

	int FORWARD_I(const char* data)
	{
		char*  p = new char[strlen(data) + 1];
		strcpy(p, data);

		char*  dd;
		char*  k = strtok(p, ",");
		int index = 0;
		while (k)
		{
			_req.FORWARD_I[index++] = atof(k);
			if (index > 10) break;
			k = strtok(NULL, ",");
		}
		delete[] p;
		return 0;
	}

public:
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendFLDP();

private:
	tMTCP_payload_FLDP_REQ		_req;
};



//////////////////////////////////////////////////
// FLDI
/////////////////////////////////////////////////
class FLDI : public CFrameBase
{
	enum MyEnum
	{
		BUFFER_SIZE = 64 * 1024 * 1024
	};
public:
	FLDI();
	~FLDI();

public:
	SET_PRO(int, IMG_INDEX);
	SET_PRO(int, IMG_WIDTH);
	SET_PRO(int, IMG_HEIGHT);
	SET_PRO(int, IMG_SIZE);
	SET_PRO(int, POS_X);
	SET_PRO(int, POS_Y);
	SET_PRO(float, POWER_TO_PIXEL_RATIO);

public:
	int setImageRawdata(char* filename); //return -1 read fail,return 0 read success
	int setFloatRawdata(char* filename);
	int setAllRawdata();

	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendFLDI();

private:
	int real_setImageRawdata(char* filename); //return -1 read fail,return 0 read success
	int real_setFloatRawdata(char* filename);
	int fill_FLoat_Raw_Data();

private:
	std::string				m_ffd_filepath;
	std::string				m_img_filepath;
	char *					m_data;
	int						m_size;
	int						m_length;

	tMTCP_payload_FLDI_REQ	_req;

	char * flat_rawdata; // float array
	char * image_rawdata; // one image 

	char * rawdata;		  // fldi raw data

	long flat_len;		  // length of flat_rawdata 
	long image_len;		  // 0 or length of image_rawdata 

	long rawdata_len;	  // 0 or length of rawdata 
};



//////////////////////////////////////////////////
// INIT
/////////////////////////////////////////////////
class INIT : public CFrameBase
{
public:
	INIT();
	~INIT();

public:
	int TSTC_NAME(char* value);
	int TSTC_ID(float v);
	int TSTC_SW_VER(char* value);
	int LOT_NAME(char* value);
	int LOT_SIZE(uint32_t value);
	int DIFFUSER_LOT_ID(char* value);
	int OPERATOR(char* value);

	int setvalue(char* what, char* value);
public:
	tMTCP_payload_TEST_RSP SendINIT();

private:

	tMTCP_payload_INIT_REQ	_req;
};


//////////////////////////////////////////////////
// SEQU
/////////////////////////////////////////////////
class SEQU : public CFrameBase
{
public:
	SEQU();
	~SEQU();

public:
	tMTCP_payload_SEQU_RSP SendSEQU();

	int GetItemCount()
	{
		return _rsp.TEST_ITEM_CNT;
	}
	int GetChamber()
	{
		return _rsp.CHAMBER_CNT;
	}

	const char*  CHAMBER_TO_RUN()
	{
		// sprintf(CHAMBER_RUN,)
		// return CHAMBER_RUN;

		int i = 0;
		for (i = 0; i < _rsp.CHAMBER_CNT; i++)
		{
			_rsp.CHAMBER_TO_RUN[i] = '0' + _rsp.CHAMBER_TO_RUN[i];
		}

		_rsp.CHAMBER_TO_RUN[i] = 0;

		return (const char*)_rsp.CHAMBER_TO_RUN;
	}
private:
	tMTCP_payload_SEQU_RSP	_rsp;
	char*  CHAMBER_RUN[128];

};


///////////////////////////////////////////
//Chamber4
//////////////////////////////////////////



/////////////////////////////////////////////
//Ginbal
/////////////////////////////////////////////
class ALPR :CFrameBase
{
public:
	ALPR();
	~ALPR();
	float	HOT_SPOT_THETA()			// theta angle for hot spot
	{
		return m_ALPR_RSP.HOT_SPOT_THETA;
	}

	float	HOT_SPOT_PHI()
	{
		return m_ALPR_RSP.HOT_SPOT_PHI;
	}

	tMTCP_payload_ALPR_RSP Get_ALPR_RSP_MSG();
	tMTCP_payload_ALPR_RSP SendALPR();

private:
	tMTCP_payload_ALPR_RSP m_ALPR_RSP;
};


class ALPH :CFrameBase
{
public:
	ALPH();
	~ALPH();
	SET_PRO(int, IMG_CNT);
	SET_PRO(int, IMG_WIDTH);
	SET_PRO(int, IMG_HEIGHT);
	SET_PRO(int, IMG_SIZE);
	SET_PRO(float, SOCKET_DCR);	
	int FORWARD_V(const char* v);
	int FORWARD_I(const char* v);
	//SET_PRO(float, I_DR);
	//SET_PRO(float, V_FOR);

	int SetImage(const char*  imagepath)
	{
		return 0;
	}
	
	int real_setRawdata(int file_count, string* image_path);
	int setRawdata(int file_count, char** filename);
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendALPH();
	std::string image_path[10];
	int image_count = 0;

private:
	tMTCP_payload_ALPH_REQ _req;
	//char* m_data;
	char* all_data;
	long rawdata_len; //0 or length of m_data
	
};
///////////////////////////////////////////
///YGTS
//////////////////////////////////////////
class YGTS :public CFrameBase
{
public:
	YGTS();
	~YGTS();
public:
	SET_PRO(float, Y_I_ACTIVE);
	SET_PRO(float, Y_I_STANDBY);
	SET_PRO(float, I_CONDITION_A);
	SET_PRO(uint8_t, FAULT_STATUS_A);
	SET_PRO(float, I_CONDITION_B1);
	SET_PRO(uint8_t, FAULT_STATUS_B1);
	SET_PRO(float, I_CONDITION_B2);
	SET_PRO(uint8_t, FAULT_STATUS_B2);
	SET_PRO(float, I_CONDITION_B3);
	SET_PRO(uint8_t, FAULT_STATUS_B3);
	SET_PRO(float, I_CONDITION_C);
	SET_PRO(uint8_t, FAULT_STATUS_C);
	SET_PRO(float, I_CONDITION_D);
	SET_PRO(uint8_t, FAULT_STATUS_D);
	SET_PRO(float, I_CONDITION_E);
	SET_PRO(uint8_t, FAULT_STATUS_E_1);
	SET_PRO(uint8_t, FAULT_STATUS_E_2);
	SET_PRO(uint8_t, LEVEL_STATUS_PIN);
	SET_PRO(float, I_STATUS_PIN);
public:
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendYGTS();
private:
	tMTCP_payload_YGTS_PREDA_REQ _req;

};

class YGTSPOST : public CFrameBase
{
public:
	YGTSPOST();
	~YGTSPOST();
public:
	SET_PRO(float, Y_I_ACTIVE);
	SET_PRO(float, Y_I_STANDBY);
	SET_PRO(float, I_CONDITION_A);
	SET_PRO(uint8_t, FAULT_STATUS_A);
	SET_PRO(float, V_FORWARD_STATUS);
	SET_PRO(float, I_FORWARD_STATUS);
	SET_PRO(uint8_t, LEVEL_STATUS_PIN);
	SET_PRO(float, I_STATUS_PIN);
public:
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendYGTSPOST();
private:
	tMTCP_payload_YGTS_POSTDA_REQ _req;

};

///////////////////////////////////////////
///YGTR
//////////////////////////////////////////
class YGTR : public CFrameBase
{
public:
	YGTR();
	~YGTR();
public:
	float I_CAL();
	float I_PULSEMAX_B1();
	float I_PULSEMAX_B2();
	float I_PULSEMAX_B3();
	uint8_t SEL_I_PULSEMAX();
	uint8_t SEL_I_SEVERE();
		
public:
	tMTCP_payload_YGTR_RSP SendYGTR();
private:
	tMTCP_payload_YGTR_RSP m_Rep;
};

///////////////////////////////////////////
///YGWR
//////////////////////////////////////////
class YGWR : public CFrameBase
{
public:
	YGWR();
	~YGWR();
public:
	uint8_t CNT();
	char* ADDR();
	char* DATA();
public:
	tMTCP_payload_YGWR_RSP SendYGWR();
private:
	tMTCP_payload_YGWR_RSP m_Rep;
};

///////////////////////////////////////////
///YGWC
//////////////////////////////////////////
class YGWC :public CFrameBase
{
public:
	YGWC();
	~YGWC();
public:
	SET_PRO(uint8_t, CONFIG_CRC_H);
	SET_PRO(uint8_t, CONFIG_CRC_L);
	SET_PRO(uint8_t, CONFIG_CRC_H_CALC);
	SET_PRO(uint8_t, CONFIG_CRC_L_CALC);
	SET_PRO(uint8_t, PROG0_REG);
	int Y_NVM_1(char * value);
	int Y_NVM_2(char * value);
	int Y_NVM_3(char * value);
public:
	int setvalue(char*  what, char*  value);
	tMTCP_payload_TEST_RSP SendYGWC();
private:
	tMTCP_payload_YGWC_REQ _req;
};

#endif

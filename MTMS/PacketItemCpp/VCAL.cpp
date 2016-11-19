#include "../mtcp_module.h"


VCAL::VCAL()
{
	memset(&m_VCAL_REQ, 0, sizeof(m_VCAL_REQ));
	allRawdata = nullptr;
	m_TTDPData = nullptr;
	m_ADIPData = nullptr;
	TTDP_len = 0;
	ADIP_len = 0;
	all_len = 0;

	this->ttdp_Path.clear();
	this->adip_Path.clear();
}


VCAL::~VCAL()
{
	if (allRawdata){
		delete[] allRawdata;
		allRawdata = nullptr;
	}
}


int VCAL::NTC_STEADY_STATE(float v)
{
	m_VCAL_REQ.NTC_STEADY_STATE = v;
	return 0;
}
int VCAL::T_STEADY_STATE(float v)
{
	m_VCAL_REQ.T_STEADY_STATE = v;
	return 0;
}
int VCAL::PEAK_OPTICAL_POWER_FINAL(float v)
{
	m_VCAL_REQ.PEAK_OPTICAL_POWER_FINAL = v;
	return 0;
}
int VCAL::I_PULSE_FINAL(float v)
{
	m_VCAL_REQ.I_PULSE_FINAL = v;
	return 0;
}
int VCAL::V_PULSE_FINAL(float v)
{
	m_VCAL_REQ.V_PULSE_FINAL = v;
	return 0;
}
int VCAL::PEAK_WAVELENGTH(float v)
{
	m_VCAL_REQ.PEAK_WAVELENGTH = v;
	return 0;
}

int VCAL::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "NTC_STEADY_STATE")){
		NTC_STEADY_STATE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "T_STEADY_STATE")){
		T_STEADY_STATE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "PEAK_OPTICAL_POWER_FINAL")){
		PEAK_OPTICAL_POWER_FINAL(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "I_PULSE_FINAL")){
		I_PULSE_FINAL(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "V_PULSE_FINAL")){
		V_PULSE_FINAL(atof(value));
		return 0;
	}
	
	if (0 == strcmp(what, "PEAK_WAVELENGTH")){
		PEAK_WAVELENGTH(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FWHM_WAVELENGTH")){
		FWHM_WAVELENGTH(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "WL_DATA_LEN")){
		WL_DATA_LEN(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "ITER_CNT")){
		ITER_CNT(atoi(value));
		return 0;
	}


	return -1;
}

int VCAL::FWHM_WAVELENGTH(float v)
{
	m_VCAL_REQ.FWHM_WAVELENGTH = v;
	return 0;
}
int VCAL::WL_DATA_LEN(int v)
{
	m_VCAL_REQ.WL_DATA_LEN = v;
	return 0;
}
int VCAL::ITER_CNT(int v)
{
	m_VCAL_REQ.ITER_CNT = v;
	return 0;
}

int VCAL::pushData(float I, float v, float power, const char * v_pulse_raw, float power_raw)
{
	return 0;
}

int VCAL::setTTDP(char* filename)
{
	/*float wave[4096] = { 0 };
	float meas[4096] = { 0 };*/
	if (NULL == filename)
	{
		return -1;
	}
	ttdp_Path = std::string(filename);
	return 0;
}

int VCAL::setADIP(char * filename)
{
	
	if (NULL == filename)
	{
		return -1;
	}
	adip_Path = std::string(filename);
	return 0;
}

int VCAL::setAllRawdata()
{
	 if (!m_ADIPData){
		if (!m_TTDPData){
			//  no ADIP,no TTDP
			if (allRawdata){
				delete[] allRawdata;
				allRawdata = nullptr;
			}
			all_len = 0;
		}
		//only TTDP
		else{
			if (allRawdata){
				delete[] allRawdata;
				allRawdata = nullptr;
			}
			all_len = TTDP_len;
			allRawdata = new char[all_len];
			memset(allRawdata, 0, all_len);
			memcpy(allRawdata, m_TTDPData, TTDP_len);
		}
	}
	else{
		if (!m_TTDPData){
			//only ADIP
			if (allRawdata){
				delete[] allRawdata;
				allRawdata = nullptr;
			}
			all_len = ADIP_len;
			allRawdata = new char[all_len];
			memset(allRawdata, 0, all_len);
			memcpy(allRawdata, m_ADIPData, ADIP_len);
		}
		// both TTDP and ADIP
		else{
			if (allRawdata){
				delete[] allRawdata;
				allRawdata = nullptr;
			}
			all_len = ADIP_len + TTDP_len;
			allRawdata = new char[all_len];
			memset(allRawdata, 0, all_len);
			memcpy(allRawdata, m_TTDPData, TTDP_len);
			memcpy(allRawdata + TTDP_len, m_ADIPData, ADIP_len);
		}
	}

	return 0;
}

int VCAL::real_setADIP(char* filename)
{
	printf("###Read VCAL ADIP raw data from : %s\r\n", filename);
	int adip_count = ADIPCount(filename);
	printf("###ADIP Count : %d\r\n", adip_count);
	if (adip_count <= 0){
		return -1;
	}
	if (m_ADIPData){
		delete[] m_ADIPData;
		m_ADIPData = nullptr;
		ADIP_len = 0;
	}
	m_ADIPData = new tMTCP_data_ADIP[adip_count];
	memset(m_ADIPData, 0, adip_count*sizeof(tMTCP_data_ADIP));

	printf("###start to analyze ADIP\r\n");
	int ret = analyseADIP(filename, m_ADIPData, adip_count);
	printf("###analyze ADIP return : %d\r\n", ret);

	ADIP_len = adip_count*sizeof(tMTCP_data_ADIP);
	m_VCAL_REQ.ITER_CNT = adip_count;

	printf("###Read VCAL ADIP finished!\r\n");
	return 0;
}

int VCAL::real_setTTDP(char* filename)
{
	printf("###Read VCAL TTDP raw data from : %s\r\n", filename);
	int count = val_isRawData(filename);
	if (count < 1){
		cout << "read rawData file fail!" << endl;
		return -1;
	}
	float * wave = new float[count];
	float * meas = new float[count];
	memset(wave, 0, sizeof(float)*count);
	memset(meas, 0, sizeof(float)*count);

	AnalyseValue(filename, wave, meas,count);
	if (m_TTDPData){
		delete[] m_TTDPData;
		m_TTDPData = nullptr;
		TTDP_len = 0;
	}
	m_TTDPData = new tMTCP_data_TTDP[count];
	int i = 0;
	//for (i = 0; i<4096 && i < count; i++){
	for (i = 0; i < count; i++){
		m_TTDPData[i].wavelength = wave[i];
		m_TTDPData[i].measure = meas[i];
	}
	m_VCAL_REQ.WL_DATA_LEN = i;
	TTDP_len = sizeof(tMTCP_data_TTDP)*i;
	cout << "rawdata_len:" << TTDP_len << endl;
	return 0;
	delete[] wave;
	delete[] meas;
	wave = nullptr;
	meas = nullptr;

	printf("###Read VCAL TTDP finished!\r\n");
	return 0;
}

int VCAL::fill_ALL_Rawdata()
{	
	printf("start to fill VCAL raw data!\r\n");
	if (real_setADIP((char *)adip_Path.c_str()) < 0)
	{
		printf("###Read VCAL ADIP failed!\r\n");
		ADIP_len = 0;
		//return -1;
		m_VCAL_REQ.ITER_CNT = 0;
	}



	if (real_setTTDP((char *)ttdp_Path.c_str()) < 0)
	{
		printf("###Read VCAL TTDP failed!\r\n");
		TTDP_len = 0;
		//return -1;
		m_VCAL_REQ.WL_DATA_LEN = 0;
	}
	
	if (allRawdata){
		delete[] allRawdata;
		allRawdata = nullptr;
	}

	all_len = ADIP_len + TTDP_len;
	
	if (all_len <= 0)
	{
		printf("VCAL: params failed!\n");
		return -1;
	}

	allRawdata = new char[all_len];
	memset(allRawdata, 0, all_len);
	memcpy(allRawdata, m_TTDPData, TTDP_len);
	memcpy(allRawdata + TTDP_len, m_ADIPData, ADIP_len);
	
	return 0;
}

tMTCP_payload_TEST_RSP VCAL::SendVCAL()
{
	if (fill_ALL_Rawdata() < 0){
		return tMTCP_payload_TEST_RSP();
	}
	//all_len = 0;
	//m_VCAL_REQ.ITER_CNT = 0;
	//m_VCAL_REQ.WL_DATA_LEN = 0;

	int ret = SendFrame(kMTCP_CTRL_VCAL, &m_VCAL_REQ, sizeof(m_VCAL_REQ), allRawdata, all_len);
	if (m_ADIPData){
		delete[] m_ADIPData;
		m_ADIPData = nullptr;
		ADIP_len = 0;
	}
	if (m_TTDPData){
		delete[] m_TTDPData;
		m_TTDPData = nullptr;
		TTDP_len = 0;
	}
	if (allRawdata){
		delete[] allRawdata;
		allRawdata = nullptr;
		all_len = 0;
	}
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	
	return rep;
}

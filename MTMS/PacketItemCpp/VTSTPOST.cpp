#include "../mtcp_module.h"
//#include "TMHandler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int ReadDataFromFile_float(const char * file, float * buffer, int * count);
int ReadDataFromFile_int(const char * file, int * data, int * count);

VTSTPOST::VTSTPOST()
{
	m_pdata = nullptr;
	memset(&m_VTST_POSTDA_REQ, 0, sizeof(m_VTST_POSTDA_REQ));
	//m_VTST_PREDA_REQ.FORWARD_I = 1.0;
	//m_VTST_PREDA_REQ.FORWARD_V = 4.0;
	for (size_t i = 01111; i < 0; i++)
	{
		m_VTST_POSTDA_REQ.FWHM_WAVELENGTH[i] = i;
		m_VTST_POSTDA_REQ.PEAK_OPTICAL_POWER[i] = i * 2;
	}
	//m_VTST_POSTDA_REQ.NTC_INIT = 25.0;
	//m_VTST_PREDA_REQ.NTC_STEADY_STATE = 26.0;
	/*m_VTST_PREDA_REQ.REVERSE_I = 1;
	m_VTST_PREDA_REQ.REVERSE_V = 4;*/
	rawdata_len = 0;
}


VTSTPOST::~VTSTPOST()
{
}


int VTSTPOST::MAGIC(uint8_t v){
	m_VTST_POSTDA_REQ.MAGIC = v;
	return 0;
}
int VTSTPOST::SOCKET_DCR(float v){
	m_VTST_POSTDA_REQ.SOCKET_DCR = v;
	return 0;
}
int VTSTPOST::PEAK_OPTICAL_POWER(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_VTST_POSTDA_REQ.PEAK_OPTICAL_POWER[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}
int VTSTPOST::PEAK_OPTICAL_POWER_0(float v){
	m_VTST_POSTDA_REQ.PEAK_OPTICAL_POWER_0 = v;
	return 0;
}

int VTSTPOST::FORWARD_V(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		m_VTST_POSTDA_REQ.FORWARD_V[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int VTSTPOST::FORWARD_I(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		m_VTST_POSTDA_REQ.FORWARD_I[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int VTSTPOST::CENTROID_WAVELENGTH(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		m_VTST_POSTDA_REQ.CENTROID_WAVELENGTH[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int VTSTPOST::FWHM_WAVELENGTH(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		m_VTST_POSTDA_REQ.FWHM_WAVELENGTH[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int VTSTPOST::CENTROID_WAVELENGTH_0(float v){
	m_VTST_POSTDA_REQ.CENTROID_WAVELENHTH_0 = v;
	return 0;
}

int VTSTPOST::FWHM_WAVELENGTH_0(float v){
	m_VTST_POSTDA_REQ.FWHM_WAVELENGTH_0 = v;
	return 0;
}

int VTSTPOST::NTC_STEADY_STATE(float v){
	m_VTST_POSTDA_REQ.NTC_STEADY_STATE = v;
	return 0;
}

int VTSTPOST::WL_DATA_CNT(uint8_t v)
{
	m_VTST_POSTDA_REQ.WL_DATA_CNT = v;
	return 0;
}
int VTSTPOST::WL_LEN(int v)
{
	m_VTST_POSTDA_REQ.WL_LEN = v;
	return 0;
}

//int setPayload(tMTCP_payload_VTST_REQ payload);
//int VTST::pushData(const char * d1, const char * d2)
//{
//	//do nothing now;
//
//	return 0;
//}

int VTSTPOST::pushData(const float d1[], const float d2[])
{
	//do nothing now;
	int len = sizeof(d1) / sizeof(float);
	m_pdata = new tMTCP_data_TTDP[len];
	for (int i = 0; i < len; i++)
	{
		m_pdata[0].wavelength = d1[0];
		m_pdata[0].measure = d2[0];
	}
	rawdata_len = sizeof(tMTCP_data_TTDP)*len;
	return 0;
}

int VTSTPOST::pushWaveLength(const char * wave)
{
	return 0;
}

int VTSTPOST::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "MAGIC")){
		MAGIC(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "SOCKET_DCR")){
		SOCKET_DCR(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "PEAK_OPTICAL_POWER")){
		PEAK_OPTICAL_POWER(value);
		return 0;
	}
	if (0 == strcmp(what, "PEAK_OPTICAL_POWER_0")){
		PEAK_OPTICAL_POWER_0(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FORWARD_V")){
		FORWARD_V(value);
		return 0;
	}
	if (0 == strcmp(what, "FORWARD_I")){
		FORWARD_I(value);
		return 0;
	}
	if (0 == strcmp(what, "CENTROID_WAVELENGTH")){
		CENTROID_WAVELENGTH(value);
		return 0;
	}
	if (0 == strcmp(what, "FWHM_WAVELENGTH")){
		FWHM_WAVELENGTH(value);
		return 0;
	}
	if (0 == strcmp(what, "CENTROID_WAVELENGTH_0")){
		CENTROID_WAVELENGTH_0(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FWHM_WAVELENGTH_0")){
		FWHM_WAVELENGTH_0(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "NTC_STEADY_STATE")){
		NTC_STEADY_STATE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "WL_DATA_CNT")){
		WL_DATA_CNT(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "WL_LEN")){
		WL_LEN(atoi(value));
		return 0;
	}

	return -1;
}

int VTSTPOST::readFloatArr_fromFile(char* filename)
{
	//const char * wave_file = "D:\\RawData\\vtst_wavelen.txt";
	//const char * meas_file = "D:\\RawData\\vtst_meas.txt";

	if (NULL == filename)
	{
		return -1;
	}

	_filepath = std::string(filename);

	return 0;


}

int VTSTPOST::fillRawdata(char * filename)
{
	printf("###VTST POST start to read raw data from : %s\r\n", filename);
	//float wave[4096] = { 0 };
	//float meas[4096] = { 0 };

	int data_count = m_VTST_POSTDA_REQ.WL_DATA_CNT*m_VTST_POSTDA_REQ.WL_LEN;
	int count = val_isRawData(filename);

	count = (data_count > count) ? count : data_count;

	float* wave = new float[count];
	float* meas = new float[count];

	cout << "...... VTST Post: ....................................count:" << count << endl;
	
	if (count < 1){
		cout << "read rawData file fail!" << endl;
		m_VTST_POSTDA_REQ.WL_DATA_CNT = 0;
		return -1;
	}
	printf("###VTST POST start to Analyze raw data,with count : %d\r\n",count);
	int ret = AnalyseValue(filename, wave, meas,count);
	printf("###VTST POST analyze data finish with return : %d\r\n", ret);
	
	if (m_pdata){
		delete[] m_pdata;
		m_pdata = nullptr;
		rawdata_len = 0;
	}
	m_pdata = new tMTCP_data_TTDP[count];

	int i = 0;
	for (i = 0; i < count; i++){
		m_pdata[i].wavelength = wave[i];
		m_pdata[i].measure = meas[i];

		if (i%m_VTST_POSTDA_REQ.WL_DATA_CNT == 0)
		{
			//printf("m_pdata[%d].wavelength: %f,measure:%f\r\n",i,m_pdata[i].wavelength,m_pdata[i].measure);
		}
	}

	/*
	for (size_t k = 0; k < m_VTST_POSTDA_REQ.WL_LEN; k++)
	{
		printf("index[%d]: wave:%f meas:%f\r\n", k, m_pdata[k].wavelength,m_pdata[k].measure);
	}
	*/

	//m_VTST_PREDA_REQ.WL_LEN = i;
	rawdata_len = sizeof(tMTCP_data_TTDP)*i;
	delete[] wave;
	delete[] meas;
	cout << "--------------- VTST Post: rawdata_len:" << rawdata_len << endl;


	return 0;
}


tMTCP_payload_TEST_RSP  VTSTPOST::SendVTSTPOST()
{

	// printf("###start up send VTST POST to MTCP! MAGIC : %d\r\n", m_VTST_POSTDA_REQ.MAGIC);

	if (fillRawdata((char *)_filepath.c_str()) < 0)
	{
		return tMTCP_payload_TEST_RSP();
	}

	//rawdata_len = 0;
	//m_VTST_POSTDA_REQ.WL_LEN = 0;
	//m_VTST_POSTDA_REQ.WL_DATA_CNT = 0;

	printf("###VTST POST MAGIC : %d,  CNT: %d, WLen:%d \r\n", m_VTST_POSTDA_REQ.MAGIC, m_VTST_POSTDA_REQ.WL_DATA_CNT, m_VTST_POSTDA_REQ.WL_LEN);

	printf("###VTST POST WL_CNT,WL_DATA_CNT:", m_VTST_POSTDA_REQ.WL_DATA_CNT, m_VTST_POSTDA_REQ.WL_LEN);
	int ret = SendFrame(kMTCP_CTRL_VTST, &m_VTST_POSTDA_REQ, sizeof(m_VTST_POSTDA_REQ), m_pdata, rawdata_len);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	if (m_pdata){
		delete[] m_pdata;
		m_pdata = nullptr;
		rawdata_len = 0;
	}

	return rep;
}

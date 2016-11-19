#include "../mtcp_module.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int ReadDataFromFile_float(const char * file, float * buffer, int * count);
int ReadDataFromFile_int(const char * file, int * data, int * count);

C1SS::C1SS()
{
	memset(&m_C1SS_REQ, 0, sizeof(m_C1SS_REQ));
	m_C1SS_REQ.I_DR = 1;
	m_pdata = nullptr;
	rawdata_len = 0;
}


C1SS::~C1SS()
{
	
}


int C1SS::NTC_STEADY_STATE(float v)
{
	m_C1SS_REQ.NTC_STEADY_STATE = v;
	return 0;
}

int C1SS::RSVD(const char* v)
{
	memcpy(m_C1SS_REQ.RSVD, v, 8);
	return 0;
}
int C1SS::I_DR(float v)
{
	m_C1SS_REQ.I_DR = v;
	return 0;
}

int C1SS::V_FORWARD(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_C1SS_REQ.V_FORWARD[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int C1SS::PEAK_OPTICAL_POWER(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_C1SS_REQ.PEAK_OPTICAL_POWER[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int C1SS::PEAK_WAVELENGTH(float v)
{
	m_C1SS_REQ.PEAK_WAVELENGTH = v;
	return 0;
}

int C1SS::FWHM_WAVELENGTH(float v)
{
	m_C1SS_REQ.FWHM_WAVELENGTH = v;
	return 0;
}

int C1SS::WL_DATA_LEN(float v)
{
	m_C1SS_REQ.WL_DATA_LEN = v;
	return 0;
}

int C1SS::readFloatArr_fromFile(char* filename)
{
	//const char * wave_file = "D:\\RawData\\vtst_wavelen.txt";
	//const char * meas_file = "D:\\RawData\\vtst_meas.txt";
	//float wave[1024];
	//float meas[1024];
	//int count;
	//int ret = ReadDataFromFile_float(meas_file, meas, &count);
	//if (ret<0)
	//{
	//	printf("###Read raw dat file failed!");
	//	//return 0;
	//}
	//if (ret<0)
	//{
	//	printf("###Read raw dat file failed!");
	//	return -1;
	//}
	/*FILE * f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	char * buffer = new char[size + 1];
	memset(buffer, 0, size + 1);
	fseek(f, 0, SEEK_SET);
	fread(buffer, size, 1, f);
	fclose(f);*/

	float wave[4096] = {0};
	float meas[4096] = {0};
	int count = val_isRawData(filename);
	if (count < 1){
		cout << "read rawData file fail!" << endl;
		return -1;
	}

	AnalyseValue(filename, wave, meas);
	if (m_pdata || nullptr != m_pdata){
		delete[] m_pdata;
		m_pdata = nullptr;
		rawdata_len = 0;
	}
	m_pdata = new tMTCP_data_TTDP[count];
	int i = 0;
	for (i = 0; i< 4096 && i < count; i++){
		m_pdata[i].measure = meas[i];
		m_pdata[i].wavelength = wave[i];
	}
	m_C1SS_REQ.WL_DATA_LEN = i;
	rawdata_len = sizeof(tMTCP_data_TTDP)*i;
	cout <<"rawdata_len:" <<rawdata_len << endl;
	return 0;
}

int C1SS::pushData(const float d1[], const float d2[])
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

int C1SS::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "NTC_STEADY_STATE")){
		NTC_STEADY_STATE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "RSVD")){
		RSVD(value);
		return 0;
	}
	if (0 == strcmp(what, "I_DR")){
		I_DR(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "V_FORWARD")){
		V_FORWARD(value);
		return 0;
	}
	if (0 == strcmp(what, "PEAK_OPTICAL_POWER")){
		PEAK_OPTICAL_POWER(value);
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
		WL_DATA_LEN(atof(value));
		return 0;
	}

	return -1;
}

tMTCP_payload_TEST_RSP C1SS::SendC1SS()
{
	/*
	//packet raw data;
	const char * wave_file = "C:\\RawData\\c1ss_wavelen.dat";
	const char * meas_file = "C:\\RawData\\c1ss_meas.dat";
	float wave[1024];
	float meas[1024];
	int count;
	int ret = ReadDataFromFile_float(wave_file, wave, &count);
	if (ret<0)
	{
		printf("###Read raw dat file failed!");
	}
	ret = ReadDataFromFile_float(meas_file, meas, &count);
	if (ret<0)
	{
		printf("###Read raw dat file failed!");
	}

	tMTCP_data_TTDP * p = new tMTCP_data_TTDP[count];

	for (size_t i = 0; i < count; i++)
	{
		p[i].measure = meas[i];
		p[i].wavelength = wave[i];
	}

	m_C1SS_REQ.WL_DATA_LEN = count;
	ret = SendFrame(kMTCP_CTRL_C1SS, &m_C1SS_REQ, sizeof(m_C1SS_REQ), p, count * sizeof(tMTCP_data_TTDP));
	delete[] p;
	*/

	int ret = SendFrame(kMTCP_CTRL_C1SS, &m_C1SS_REQ, sizeof(m_C1SS_REQ), m_pdata, rawdata_len);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	RecvResponse(&rep, sizeof(rep));
	if (m_pdata || m_pdata != nullptr){
		delete[] m_pdata;
		m_pdata = nullptr;
		rawdata_len = 0;
	}
	return rep;
}
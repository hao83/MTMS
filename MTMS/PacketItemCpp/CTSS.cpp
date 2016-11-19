#include "../mtcp_module.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//int ReadDataFromFile_float(const char * file, float * buffer, int * count);
//int ReadDataFromFile_int(const char * file, int * data, int * count);

CTSS::CTSS()
{
	memset(&m_CTSS_REQ, 0, sizeof(m_CTSS_REQ));
	//m_CTSS_REQ.I_DR = 1;
	//m_pdata = nullptr;
	//rawdata_len = 0;
}


CTSS::~CTSS()
{

}


int CTSS::NTC_STEADY_STATE(float v)
{
	m_CTSS_REQ.NTC_STEADY_STATE = v;
	return 0;
}

//int CTSS::RSVD(const char* v)
//{
//	memcpy(m_CTSS_REQ.RSVD, v, 8);
//	return 0;
//}
//int CTSS::I_DR(float v)
//{
//	m_CTSS_REQ.I_DR = v;
//	return 0;
//}
int CTSS::I_FORWARD(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_CTSS_REQ.I_FORWARD[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}
int CTSS::V_FORWARD(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_CTSS_REQ.V_FORWARD[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int CTSS::PEAK_OPTICAL_POWER(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_CTSS_REQ.PEAK_OPTICAL_POWER[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

//int CTSS::PEAK_WAVELENGTH(float v)
//{
//	m_CTSS_REQ.PEAK_WAVELENGTH = v;
//	return 0;
//}
//
//int CTSS::FWHM_WAVELENGTH(float v)
//{
//	m_CTSS_REQ.FWHM_WAVELENGTH = v;
//	return 0;
//}
//
//int CTSS::WL_DATA_LEN(float v)
//{
//	m_CTSS_REQ.WL_DATA_LEN = v;
//	return 0;
//}

//int CTSS::readFloatArr_fromFile(char* filename)
//{
//	if (NULL == filename)
//	{
//		return -1;
//	}
//
//	_filepath = std::string(filename);
//
//	return 0;
//}

//int CTSS::fillRawdata(char* filename)
//{
//	float wave[4096] = { 0 };
//	float meas[4096] = { 0 };
//	int count = val_isRawData(filename);
//	if (count < 1){
//		cout << "read rawData file fail!" << endl;
//		return -1;
//	}
//
//	AnalyseValue(filename, wave, meas);
//	if (m_pdata || nullptr != m_pdata){
//		delete[] m_pdata;
//		m_pdata = nullptr;
//		rawdata_len = 0;
//	}
//	m_pdata = new tMTCP_data_TTDP[count];
//	int i = 0;
//	for (i = 0; i< 4096 && i < count; i++){
//		m_pdata[i].measure = meas[i];
//		m_pdata[i].wavelength = wave[i];
//	}
//	//m_CTSS_REQ.WL_DATA_LEN = i;
//	rawdata_len = sizeof(tMTCP_data_TTDP)*i;
//	cout << "rawdata_len:" << rawdata_len << endl;
//	return 0;
//
//}

//int CTSS::pushData(const float d1[], const float d2[])
//{
//	//do nothing now;
//	int len = sizeof(d1) / sizeof(float);
//	m_pdata = new tMTCP_data_TTDP[len];
//	for (int i = 0; i < len; i++)
//	{
//		m_pdata[0].wavelength = d1[0];
//		m_pdata[0].measure = d2[0];
//	}
//	rawdata_len = sizeof(tMTCP_data_TTDP)*len;
//	return 0;
//}

int CTSS::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "NTC_STEADY_STATE")){
		NTC_STEADY_STATE(atof(value));
		return 0;
	}
	/*if (0 == strcmp(what, "RSVD")){
	RSVD(value);
	return 0;
	}
	if (0 == strcmp(what, "I_DR")){
	I_DR(atof(value));
	return 0;
	}*/
	if (0 == strcmp(what, "I_FORWARD")){
		I_FORWARD(value);
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
	/*if (0 == strcmp(what, "PEAK_WAVELENGTH")){
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
	}*/

	return -1;
}

tMTCP_payload_TEST_RSP CTSS::SendCTSS()
{
	//if (fillRawdata((char *)_filepath.c_str()) < 0)
	//{
	//	return tMTCP_payload_TEST_RSP();
	//}

	// int ret = SendFrame(kMTCP_CTRL_CTSS, &m_CTSS_REQ, sizeof(m_CTSS_REQ), m_pdata, rawdata_len);
	int ret = SendFrame(kMTCP_CTRL_CTSS, &m_CTSS_REQ, sizeof(m_CTSS_REQ), NULL, 0);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	RecvResponse(&rep, sizeof(rep));

	//if (m_pdata != nullptr){
	//delete[] m_pdata;
	//m_pdata = nullptr;
	//rawdata_len = 0;
	//}

	return rep;
}

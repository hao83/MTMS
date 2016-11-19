#include "../mtcp_module.h"
#include <iostream>

ALPH::ALPH()
{
	memset(&_req, 0, sizeof(_req));
	//_req.I_DR = 0.9;
	all_data = nullptr;
	rawdata_len = 0;
}

ALPH::~ALPH()
{
}

int ALPH::FORWARD_I(const char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		_req.FORWARD_I[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}
int ALPH::FORWARD_V(const char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		_req.FORWARD_I[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int ALPH::setRawdata(int file_count, char** filename) //file_count:
{
	if (file_count == 0)
	{
		return -1;
	}

	image_count = file_count;

	for (int i = 0; i < file_count; i++){
		image_path[i] = string(filename[i]);
	}
	for (int i = 0; i < 16; i++){
		delete[] filename[i];
		filename[i] = nullptr;
	}
	delete[] filename;
	filename = nullptr;
	return 0;
}

int ALPH::real_setRawdata(int file_count, string* filename){
	FILE * f = fopen(filename[0].c_str(), "r");
	if (f == NULL)
	{
		printf("##read file fail!\n");
		return -1;
	}
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	cout << "----------------size:" << size << endl;
	fclose(f);
	char* m_data = new char[size];
	all_data = new char[size*file_count];
	memset(all_data, 0, size*file_count);

	long index = 0;
	for (int i = 0; i < file_count; i++){
		FILE * f = fopen(filename[i].c_str(), "r");
		if (f == NULL)
		{
			return -1;
		}
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		memset(m_data, 0, size);
		fseek(f, 0, SEEK_SET);
		fread(m_data, size, 1, f);
		fclose(f);
		memcpy((all_data + index), m_data, size);
		index += size;
	}
	rawdata_len = size*file_count;
	if (m_data){
		delete[] m_data;
		m_data = nullptr;

	}
	// _req.IMG_WIDTH = 3296;
	// _req.IMG_HEIGHT = 2472;
	_req.IMG_SIZE = size;
	_req.IMG_CNT = file_count;
	return 0;
}

int ALPH::setvalue(char*  what, char*  value)
{
		if (0 == strcmp(what, "IMG_CNT")){
			IMG_CNT(atoi(value));
			return 0;
		}
		if (0 == strcmp(what, "IMG_WIDTH")){
			IMG_WIDTH(atoi(value));
			return 0;
		}
		if (0 == strcmp(what, "IMG_HEIGHT")){
			IMG_HEIGHT(atoi(value));
			return 0;
		}
		if (0 == strcmp(what, "IMG_SIZE")){
			IMG_SIZE(atoi(value));
			return 0;
		}
		
		if (0 == strcmp(what, "SOCKET_DCR")){
			SOCKET_DCR(atoi(value));
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
		/*if (0 == strcmp(what, "I_DR")){
			I_DR(atof(value));
			return 0;
		}
		if (0 == strcmp(what, "V_FOR")){
			V_FOR(atof(value));
			return 0;
		}*/
		return 0;
}


tMTCP_payload_TEST_RSP ALPH::SendALPH()
{
	real_setRawdata(image_count,image_path);
	int ret = SendFrame(kMTCP_CTRL_ALPH, &_req, sizeof(_req), all_data, rawdata_len);
	cout << "send ok !!! rawdata_len is :" << rawdata_len << endl;
	if (all_data){
		delete[] all_data;
		all_data = nullptr;
	}
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(rep));
	RecvResponse(&rep, sizeof(rep));
	return rep;
}

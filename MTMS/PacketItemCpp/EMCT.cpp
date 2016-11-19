#include "../mtcp_module.h"


EMCT::EMCT()
{
	memset(&m_EMCT_REQ, 0, sizeof(m_EMCT_REQ));

	m_data = new char[BUFFER_SIZE + 1];
	m_size = BUFFER_SIZE;
	m_length = 0;
	m_file.clear();
}


EMCT::~EMCT()
{
	delete[] m_data;
}

int EMCT::IMG_WIDTH(unsigned short v)
{
	m_EMCT_REQ.IMG_WIDTH = v;
	return 0;
}
int EMCT::IMG_HEIGHT(unsigned short v)
{
	m_EMCT_REQ.IMG_HEIGHT = v;
	return 0;
}
int EMCT::IMG_SIZE(unsigned int v)
{
	m_EMCT_REQ.IMG_SIZE = v;
	return 0;
}
int EMCT::CLUSTER_THRESHOLD(float v)
{
	m_EMCT_REQ.CLUSTER_THRESHOLD = v;
	return 0;
}
int EMCT::VALID_EMITTER_CNT(unsigned char v)
{
	m_EMCT_REQ.VALID_EMITTER_CNT = v;
	return 0;
}
int EMCT::EMITTER_CENTER_X(const char * data)
{
	printf("###EMCT : start to set raw data of EMITTER_CENTER_X\r\n");
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_EMCT_REQ.EMITTER_CENTER_X[index++] = atoi(k);
		if (index >= 256) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	printf("###EMCT : set EMITTER_CENTER_X finished! count : %d\r\n",index);
	return 0;
}
int EMCT::EMITTER_CENTER_Y(const char * data)
{
	printf("###EMCT : start to set raw data of EMITTER_CENTER_Y\r\n");
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_EMCT_REQ.EMITTER_CENTER_Y[index++] = atoi(k);
		if (index >= 256) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	printf("###EMCT : set EMITTER_CENTER_Y finished! count : %d\r\n", index);
	return 0;
}
int EMCT::EMITTER_SIZE(const char * data)
{
	printf("###EMCT : start to set raw data of EMITTER_SIZE\r\n");
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_EMCT_REQ.EMITTER_SIZE[index++] = atoi(k);
		if (index >= 256) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	printf("###EMCT : set EMITTER_SIZE finished! count : %d\r\n", index);
	return 0;
}

int EMCT::FORWARD_V(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_EMCT_REQ.FORWARD_V[index++] = atof(k);
		if (index >= 10) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int EMCT::FORWARD_I(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_EMCT_REQ.FORWARD_I[index++] = atof(k);
		if (index >= 10) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int EMCT::UNIF_MAX(float v)
{
	m_EMCT_REQ.UNIF_MAX = v;
	return 0;
}
int EMCT::UNIF_MIN(float v)
{
	m_EMCT_REQ.UNIF_MIN = v;
	return 0;
}
int EMCT::UNIF_STD(float v)
{
	m_EMCT_REQ.UNIF_STD = v;
	return 0;
}
int EMCT::SOCKET_DCR(float v)
{
	m_EMCT_REQ.SOCKET_DCR = v;
	return 0;
}

int EMCT::setRawdata(char* filename) //return -1 read fail,return 0 read success
{
	m_file = std::string(filename);
	return 0;
}

int EMCT::setvalue(char *what, char* value)
{
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
	if (0 == strcmp(what, "CLUSTER_THRESHOLD")){
		CLUSTER_THRESHOLD(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "VALID_EMITTER_CNT")){
		VALID_EMITTER_CNT(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "EMITTER_CENTER_X")){
		EMITTER_CENTER_X(value);
		return 0;
	}
	if (0 == strcmp(what, "EMITTER_CENTER_Y")){
		EMITTER_CENTER_Y(value);
		return 0;
	}
	if (0 == strcmp(what, "EMITTER_SIZE")){
		EMITTER_SIZE(value);
		return 0;
	}
	if (0 == strcmp(what, "UNIF_MAX")){
		UNIF_MAX(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "UNIF_MIN")){
		UNIF_MIN(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "UNIF_STD")){
		UNIF_STD(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "SOCKET_DCR")){
		SOCKET_DCR(atof(value));
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

	return -1;
}

int EMCT::real_setRawdata()
{
	if (m_file.length()<=0)
	{
		return -1;
	}

	FILE * fp = fopen(m_file.c_str(), "rb");
	if (NULL == fp)
	{
		printf("##read file fail!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	int img_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(m_size<img_len)
	{
		delete[] m_data;
		m_data = new char[img_len + 1];
		m_size = img_len;
		m_length = 0;
	}

	fread(m_data, img_len, 1, fp);
	fclose(fp);

	m_length = img_len;

	m_EMCT_REQ.IMG_SIZE = m_length;

	return 0;
}

tMTCP_payload_TEST_RSP EMCT::SendEMCT()
{
	real_setRawdata();

	int ret = SendFrame(kMTCP_CTRL_EMCT, &m_EMCT_REQ, sizeof(m_EMCT_REQ), m_data, m_length);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	int recv_cnt = RecvResponse(&rep, sizeof(rep));

	return rep;
}
#include "../mtcp_module.h"


BMPF::BMPF()
{
	memset(&m_BMPF_REQ, 0, sizeof(m_BMPF_REQ));
	m_BMPF_REQ.HOT_SPOT_LOC_X = 123;

	m_data = new char[BUFFER_SIZE + 1];
	m_size = BUFFER_SIZE;
	m_length = 0;
	m_file.clear();
}


BMPF::~BMPF()
{
	delete[] m_data;
}

int BMPF::IMG_WIDTH(unsigned short v)
{
	m_BMPF_REQ.IMG_WIDTH = v;
	return 0;
}

int BMPF::IMG_HEIGHT(unsigned short v)
{
	m_BMPF_REQ.IMG_HEIGHT = v;
	return 0;
}
int BMPF::IMG_SIZE(unsigned short v)
{
	m_BMPF_REQ.IMG_SIZE = v;
	return 0;
}
int BMPF::TOTAL_OPTICAL_POWER(float v)
{
	m_BMPF_REQ.TOTAL_OPTICAL_POWER = v;
	return 0;
}
int BMPF::BEAM_SIZE(float v)
{
	m_BMPF_REQ.BEAM_SIZE = v;
	return 0;
}
int BMPF::HOT_SPOT_POWER(float v)
{
	m_BMPF_REQ.HOT_SPOT_POWER = v;
	return 0;
}
int BMPF::HOT_SPOT_LOC_X(unsigned short v)
{
	m_BMPF_REQ.HOT_SPOT_LOC_X = v;
	return 0;
}
int BMPF::HOT_SPOT_LOC_Y(unsigned short v)
{
	m_BMPF_REQ.HOT_SPOT_LOC_Y = v;
	return 0;
}
int BMPF::SOCKET_DCR(float v)
{
	m_BMPF_REQ.SOCKET_DCR = v;
	return 0;
}

int BMPF::FORWARD_I(const char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_BMPF_REQ.FORWARD_I[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int BMPF::FORWARD_V(const char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_BMPF_REQ.FORWARD_V[index++] = atof(k);
		if (index > 9) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int BMPF::setRawdata(char* filename) //return -1 read fail,return 0 read success
{
	m_file = std::string(filename);

	return 0;
}

int BMPF::setvalue(char *what, char* value)
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
	if (0 == strcmp(what, "TOTAL_OPTICAL_POWER")){
		TOTAL_OPTICAL_POWER(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "BEAM_SIZE")){
		BEAM_SIZE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "HOT_SPOT_POWER")){
		HOT_SPOT_POWER(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "HOT_SPOT_LOC_X")){
		HOT_SPOT_LOC_X(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "HOT_SPOT_LOC_Y")){
		HOT_SPOT_LOC_Y(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "SOCKET_DCR")){
		SOCKET_DCR(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FORWARD_I")){
		FORWARD_I(value);
		return 0;
	}
	if (0 == strcmp(what, "FORWARD_V")){
		FORWARD_V(value);
		return 0;
	}
	return -1;
}

int BMPF::real_setImgData()
{
	if (m_file.length()<=0)
	{
		return 0;
	}

	FILE * fp = fopen(m_file.c_str(), "r");
	if (NULL == fp)
	{
		printf("##read file fail!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	int img_len = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	if (m_size < img_len)
	{
		delete[] m_data;
		m_size = 0;
		m_length = 0;
		m_data = new char[img_len + 1];
		m_size = img_len;
	}

	fseek(fp, 0, SEEK_SET);
	fread(m_data, img_len, 1, fp);
	fclose(fp);

	m_length = img_len;
	
	m_BMPF_REQ.IMG_SIZE = img_len;

	return img_len;
}

int BMPF::real_setRawData()
{
	if (m_file.length() <= 0)
	{
		return 0;
	}

	FILE * fp = fopen(m_file.c_str(), "r");
	if (NULL == fp)
	{
		printf("##read file fail!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);

	int img_size = ftell(fp);
	int img_len = m_BMPF_REQ.IMG_WIDTH * m_BMPF_REQ.IMG_HEIGHT * sizeof(float);
	int head_len = img_size - img_len;

	if (m_size < img_len)
	{
		delete[] m_data;
		m_size = 0;
		m_length = 0;
		m_data = new char[img_len + 1];
		m_size = img_len;
	}

	fseek(fp, head_len, SEEK_SET);
	fread(m_data, img_len, 1, fp);
	fclose(fp);

	m_length = img_len;

	m_BMPF_REQ.IMG_SIZE = img_len;

	return img_len;
}

tMTCP_payload_TEST_RSP BMPF::SendBMPF()
{
	// real_setRawData();
	real_setImgData();

	int ret = SendFrame(kMTCP_CTRL_BMPF, &m_BMPF_REQ, sizeof(m_BMPF_REQ), m_data, m_length);

	tMTCP_payload_TEST_RSP rep;

	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));
	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;
}

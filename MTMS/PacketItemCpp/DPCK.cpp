#include "../mtcp_module.h"


DPCK::DPCK()
{
	memset(&m_DPCK_REQ, 0, sizeof(m_DPCK_REQ));
}


DPCK::~DPCK()
{
}


int DPCK::C_I_IDLE_LOW(float v)
{
	m_DPCK_REQ.C_I_IDLE_LOW = v;
	return 0;
}
int DPCK::C_NVM_DATA(const char * data)	//1,2,3,4,5
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_DPCK_REQ.C_NVM_DATA[index++] = atof(k);
		if (index >= 128) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int DPCK::Y_DEVICE_ID_H(uint8_t v)
{
	m_DPCK_REQ.Y_DEVICE_ID_H = v;
	return 0;
}

int DPCK::Y_DEVICE_ID_L(uint8_t v)
{
	m_DPCK_REQ.Y_DEVICE_ID_L = v;
	return 0;
}

int DPCK::Y_REVISION_MAJOR(uint8_t v)
{
	m_DPCK_REQ.Y_REVISION_MAJOR = v;
	return 0;
}

int DPCK::Y_REVISION_MINOR(uint8_t v)
{
	m_DPCK_REQ.Y_REVISION_MINOR = v;
	return 0;
}

int DPCK::Y_TRACE_ID(const char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		m_DPCK_REQ.Y_TRACE_ID[index++] = atof(k);
		if (index > 3) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
	return 0;
}
int DPCK::Y_I_ACTIVE(float v)
{
	m_DPCK_REQ.Y_I_ACTIVE = v;
	return 0;
}

int DPCK::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "C_I_IDLE_LOW")){
		C_I_IDLE_LOW(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "C_NVM_DATA")){
		C_NVM_DATA(value);
		return 0;
	}
	if (0 == strcmp(what, "Y_DEVICE_ID_H")){
		Y_DEVICE_ID_H(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "Y_DEVICE_ID_L")){
		Y_DEVICE_ID_L(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "Y_REVISION_MAJOR")){
		Y_REVISION_MAJOR(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "Y_REVISION_MINOR")){
		Y_REVISION_MINOR(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "Y_TRACE_ID")){
		Y_TRACE_ID(value);
		return 0;
	}
	if (0 == strcmp(what, "Y_I_ACTIVE")){
		Y_I_ACTIVE(atof(value));
		return 0;
	}
	return -1;
}

tMTCP_payload_TEST_RSP DPCK::SendDPCK()
{
	int ret = SendFrame(kMTCP_CTRL_DPCK, &m_DPCK_REQ, sizeof(m_DPCK_REQ), nullptr, 0);
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	RecvResponse(&rep, sizeof(rep));
	return rep;
}

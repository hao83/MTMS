#include "../mtcp_module.h"


DPWC::DPWC()
{
	memset(&m_DPWC_REQ, 0, sizeof(m_DPWC_REQ));
}


DPWC::~DPWC()
{
}

int DPWC::NVM_DATA(const char * data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		m_DPWC_REQ.NVM_DATA[index++] = atof(k);
		if (index >=128) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}

int DPWC::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "NVM_DATA")){
		NVM_DATA(value);
		return 0;
	}
	
	return -1;
}

tMTCP_payload_TEST_RSP DPWC::SendDPWC()
{
	int ret = SendFrame(kMTCP_CTRL_DPWC, &m_DPWC_REQ, sizeof(m_DPWC_REQ), nullptr, 0);
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	RecvResponse(&rep, sizeof(rep));
	return rep;
}
#include "../mtcp_module.h"

#include <string>

DPWR::DPWR()
{
	memset(&m_Rep, 0, sizeof(m_Rep));
}


DPWR::~DPWR()
{
}

char * DPWR::GetData()
{
	printf("DPWR raw Data:");
		uint8_t *p = (uint8_t*)&(m_Rep);
	for (size_t i = 0; i < sizeof(m_Rep); i++)
	{
		printf("0x%02x\t", p[i]);
	}
	printf("\r\n");
	std::string str;
	memset(m_buffer, 0, 2048);
	printf("DPWR Data:%d\r\n",m_Rep.CNT);
	for (size_t i = 0; i < m_Rep.CNT; i++)
	{
		char tmp[32] = { 0 };
		sprintf(tmp, "0x%02x=0x%02x;", m_Rep.ADDR[i], m_Rep.DATA[i]);
		printf(tmp);
		strcat(m_buffer, tmp);
		//sprintf(m_buffer, "0x%02x=0x%02x;", m_Rep.ADDR[i], m_Rep.DATA[i]);
	}
	return m_buffer;
}


tMTCP_payload_DPWR_RSP DPWR::SendDPWR()
{
	int ret = SendFrame(kMTCP_CTRL_DPWR, nullptr, 0, nullptr, 0);

	memset(&m_Rep, 0, sizeof(m_Rep));
	ret = RecvResponse(&m_Rep, sizeof(m_Rep));
	return m_Rep;
}
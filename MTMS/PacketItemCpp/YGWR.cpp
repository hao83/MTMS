#include "../mtcp_module.h"
YGWR::YGWR()
{
	memset(&m_Rep, 0, sizeof(m_Rep));
}
YGWR::~YGWR()
{

}

tMTCP_payload_YGWR_RSP YGWR::SendYGWR()
{
	int ret = SendFrame(kMTCP_CTRL_YGWR, nullptr, 0, nullptr, 0);
	memset(&m_Rep, 0, sizeof(m_Rep));
	RecvResponse(&m_Rep, sizeof(m_Rep));
	return m_Rep;
}
uint8_t YGWR::CNT(){
	return m_Rep.CNT;
}

char* YGWR::ADDR(){
	return (char *)m_Rep.ADDR;
}
char* YGWR::DATA(){
	return (char *)m_Rep.DATA;
}
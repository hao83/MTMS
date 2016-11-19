#include "../mtcp_module.h"
YGTR::YGTR()
{
	memset(&m_Rep, 0, sizeof(m_Rep));
}
YGTR::~YGTR()
{

}

tMTCP_payload_YGTR_RSP YGTR::SendYGTR()
{
	int ret = SendFrame(kMTCP_CTRL_YGTR, nullptr, 0, nullptr, 0);
	memset(&m_Rep, 0, sizeof(m_Rep));
	RecvResponse(&m_Rep, sizeof(m_Rep));
	return m_Rep;
}
float YGTR::I_PULSEMAX_B1(){
	return m_Rep.I_PULSEMAX_B1;
}
float YGTR::I_PULSEMAX_B2(){
	return m_Rep.I_PULSEMAX_B2;
}
float YGTR::I_PULSEMAX_B3(){
	return m_Rep.I_PULSEMAX_B3;
}
float YGTR::I_CAL(){
	return m_Rep.I_CAL;
}
uint8_t YGTR::SEL_I_PULSEMAX(){
	return m_Rep.SEL_I_PULSEMAX;
}
uint8_t YGTR::SEL_I_SEVERE(){
	return m_Rep.SEL_I_SEVERE;
}

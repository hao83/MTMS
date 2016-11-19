#include "../mtcp_module.h"


VDCR::VDCR()
{
	memset(&m_Rep, 0, sizeof(m_Rep));
}


VDCR::~VDCR()
{
}


tMTCP_payload_VDCR_RSP VDCR::SendVDCR()
{
	// char c = 'I';
	// int ret = SendFrame(kMTCP_CTRL_VDCR, (void*)&c, 1, nullptr, 0);

	int ret =  SendFrame(kMTCP_CTRL_VDCR, nullptr, 0, nullptr, 0);
	memset(&m_Rep, 0, sizeof(m_Rep));
	RecvResponse(&m_Rep, sizeof(m_Rep));
	return m_Rep;
}

float VDCR::I_DR()
{
	return m_Rep.I_DR;
}

float VDCR::T_SETTLE()
{
	return m_Rep.T_SETTLE;
}
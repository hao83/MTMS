#include "../mtcp_module.h"


ALPR::ALPR()
{
	memset(&m_ALPR_RSP, 0, sizeof(m_ALPR_RSP));
}


ALPR::~ALPR()
{
}

tMTCP_payload_ALPR_RSP ALPR::Get_ALPR_RSP_MSG()
{
	return m_ALPR_RSP;
}

tMTCP_payload_ALPR_RSP ALPR::SendALPR()
{
	int ret = SendFrame(kMTCP_CTRL_ALPR, nullptr, 0, nullptr, 0);
	tMTCP_payload_ALPR_RSP ALPR_RSP;
	memset(&ALPR_RSP, 0, sizeof(tMTCP_payload_ALPR_RSP));

	//int recv_cnt = RecvResponse(&m_ALPR_RSP, sizeof(m_ALPR_RSP));
	//ALPR_RSP = m_ALPR_RSP;
	ALPR_RSP.HOT_SPOT_PHI = 299581565064925750000000000000.000000;
	ALPR_RSP.HOT_SPOT_THETA = 222573824.000000;

	return ALPR_RSP;
}
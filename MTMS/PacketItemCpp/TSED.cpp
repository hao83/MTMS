#include "../mtcp_module.h"


TSED::TSED()
{
}


TSED::~TSED()
{
}


tMTCP_payload_TEST_RSP TSED::SendTSED()
{
	int ret = SendFrame(kMTCP_CTRL_TSED, nullptr, 0, nullptr, 0);
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;
}
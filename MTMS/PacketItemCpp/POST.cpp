#include "../mtcp_module.h"

POST::POST()
{
}


POST::~POST()
{
}

tMTCP_payload_POST_RSP POST::SendPOST()
{
	int ret = SendFrame(kMTCP_CTRL_POST, nullptr, 0, nullptr, 0);

	memset(&_rsp, 0, sizeof(tMTCP_payload_POST_RSP));
	int recv_cnt = RecvResponse(&_rsp, sizeof(_rsp));

	return _rsp;
}
#include "../mtcp_module.h"

SEQU::SEQU()
{
}


SEQU::~SEQU()
{
}


tMTCP_payload_SEQU_RSP SEQU::SendSEQU()
{
	int ret = SendFrame(kMTCP_CTRL_SEQU, nullptr, 0, nullptr, 0);

	memset(&_rsp, 0, sizeof(_rsp));
	int recv_cnt = RecvResponse(&_rsp, sizeof(_rsp));

	
	return _rsp;
}
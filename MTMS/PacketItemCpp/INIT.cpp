#include "../mtcp_module.h"


INIT::INIT()
{
	memset(&_req, 0, sizeof(_req));
}


INIT::~INIT()
{
}

int INIT::TSTC_NAME(char* data)
{
	memcpy((char*)_req.TSTC_NAME, data,32);
	return 0;
}

int INIT::TSTC_ID(float data)
{
	_req.TSTC_ID = data;
	return 0;
}
int INIT::TSTC_SW_VER(char* data)
{
	memcpy((char*)_req.TSTC_SW_VER, data,32);
	return 0;
}
int INIT::LOT_NAME(char* data)
{
	memcpy((char*)_req.LOT_NAME, data,32);
	return 0;
}
int INIT::LOT_SIZE(uint32_t data)
{
	_req.LOT_SIZE = data;
	return 0;
}
int INIT::DIFFUSER_LOT_ID(char* data)
{
	memcpy((char*)_req.DIFFUSER_LOT_ID, data, 32);
	return 0;
}
int INIT::OPERATOR(char* data)
{
	memcpy((char*)_req.OPERATOR, data, 32);
	return 0;
}

int INIT::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "TSTC_NAME")){
		TSTC_NAME(value);
		return 0;
	}
	if (0 == strcmp(what, "TSTC_ID")){
		TSTC_ID(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "TSTC_SW_VER")){
		TSTC_SW_VER(value);
		return 0;
	}
	if (0 == strcmp(what, "LOT_NAME")){
		LOT_NAME(value);
		return 0;
	}
	if (0 == strcmp(what, "LOT_SIZE")){
		LOT_SIZE(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "DIFFUSER_LOT_ID")){
		DIFFUSER_LOT_ID(value);
		return 0;
	}
	if (0 == strcmp(what, "OPERATOR")){
		OPERATOR(value);
		return 0;
	}
	return -1;
}

tMTCP_payload_TEST_RSP INIT::SendINIT()
{
	int ret = SendFrame(kMTCP_CTRL_INIT, (void*)&_req, sizeof(_req), nullptr, 0);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));
	int recv_cnt = RecvResponse(&rep, sizeof(rep));

	return rep;
}

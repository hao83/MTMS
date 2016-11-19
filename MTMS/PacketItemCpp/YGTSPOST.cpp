#include "../mtcp_module.h"
#include <iostream>
YGTSPOST::YGTSPOST()
{
	memset(&_req, 0, sizeof(_req));
}
YGTSPOST::~YGTSPOST()
{

}

int YGTSPOST::setvalue(char*  what, char*  value)
{
	if (0 == strcmp(what, "Y_I_ACTIVE")){
		Y_I_ACTIVE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "Y_I_STANDBY")){
		Y_I_STANDBY(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "I_CONDITION_A")){
		I_CONDITION_A(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_A")){
		FAULT_STATUS_A(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "V_FORWARD_STATUS")){
		V_FORWARD_STATUS(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_FORWARD_STATUS")){
		I_FORWARD_STATUS(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "LEVEL_STATUS_PIN")){
		LEVEL_STATUS_PIN(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_STATUS_PIN")){
		I_STATUS_PIN(atof(value));
		return 0;
	}
	return 0;
}
tMTCP_payload_TEST_RSP YGTSPOST::SendYGTSPOST()
{
	int ret = SendFrame(kMTCP_CTRL_YGTS, &_req, sizeof(_req), nullptr, 0);
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_YGTS_POSTDA_REQ));
	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;
}

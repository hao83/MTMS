#include "../mtcp_module.h"
#include <iostream> 
#include "../CLog.h"

#define SLOG1(x)			CLog::Instance()->WriteLog(x);
#define SLOG2(x, y)			CLog::Instance()->WriteJoinLog(x, y);	
#define LOG1(x)				CLog::Instance()->WriteLog(__FILE__, __LINE__, x)
#define LOG2(x, y)			CLog::Instance()->WriteJoinLog(__FILE__, __LINE__, x, y)
/***********************************/

YGTS::YGTS()
{
	memset(&_req, 0, sizeof(_req));
}
YGTS::~YGTS()
{

}

int YGTS::setvalue(char*  what, char*  value)
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
	if (0 == strcmp(what, "I_CONDITION_B1")){
		I_CONDITION_B1(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_B1")){
		FAULT_STATUS_B1(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_CONDITION_B2")){
		I_CONDITION_B2(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_B2")){
		FAULT_STATUS_B2(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_CONDITION_B3")){
		I_CONDITION_B3(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_B3")){
		FAULT_STATUS_B3(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_CONDITION_C")){
		I_CONDITION_C(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_C")){
		FAULT_STATUS_C(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_CONDITION_D")){
		I_CONDITION_D(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_D")){
		FAULT_STATUS_D(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "I_CONDITION_E")){
		I_CONDITION_E(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_E_1")){
		FAULT_STATUS_E_1(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "FAULT_STATUS_E_2")){
		FAULT_STATUS_E_2(atoi(value));
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
tMTCP_payload_TEST_RSP YGTS::SendYGTS()
{
	int ret = SendFrame(kMTCP_CTRL_YGTS, &_req, sizeof(_req), nullptr, 0);
	char buffer[1024] = { 0 };
	sprintf(buffer, "Send YGTS Frame with return : %d\r\n", ret);
	
	SLOG1(buffer);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));
	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;
}

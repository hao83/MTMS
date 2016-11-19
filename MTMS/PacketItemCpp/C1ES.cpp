#include "../mtcp_module.h"


C1ES::C1ES()
{
	memset(&_req, 0, sizeof(_req));
	_req.AE_POWER = 2.5;
}


C1ES::~C1ES()
{
}

int C1ES::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "IMG_CNT")){
		IMG_CNT(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "NTC_STEADY_STATE")){
		NTC_STEADY_STATE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "SCREEN_REFLECTANCE")){
		SCREEN_REFLECTANCE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "HOT_SPOT_THETA")){
		HOT_SPOT_THETA(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "HOT_SPOT_PHI")){
		HOT_SPOT_PHI(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "AE_POWER")){
		AE_POWER(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "OPTICAL_POWER")){
		OPTICAL_POWER(atof(value));
		return 0;
	}

	return -1;
}

int C1ES::get_payload_C1ES(){
	return _req.HOT_SPOT_PHI;
}
tMTCP_payload_TEST_RSP C1ES::SendC1ES()
{
	int ret = SendFrame(kMTCP_CTRL_C1ES, (void*)&_req, sizeof(_req), nullptr, 0);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));
	//rep.T_ERRS = "OK";
	int recv_cnt = RecvResponse(&rep, sizeof(rep));

	return rep;

}
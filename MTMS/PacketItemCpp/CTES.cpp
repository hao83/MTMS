#include "../mtcp_module.h"


CTES::CTES()
{
	memset(&_req, 0, sizeof(_req));
	//_req.AE_POWER = 2.5;
}


CTES::~CTES()
{
}

int CTES::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "IMG_CNT")){
		IMG_CNT(atoi(value));
		return 0;
	}
	/*if (0 == strcmp(what, "NTC_STEADY_STATE")){
	NTC_STEADY_STATE(atof(value));
	return 0;
	}*/
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

int CTES::get_payload_CTES(){
	return _req.HOT_SPOT_PHI;
}
tMTCP_payload_TEST_RSP CTES::SendCTES()
{
	int ret = SendFrame(kMTCP_CTRL_CTES, (void*)&_req, sizeof(_req), nullptr, 0);

	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));
	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;

}
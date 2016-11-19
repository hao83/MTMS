#include "../mtcp_module.h"


FLDP::FLDP()
{
	memset(&_req, 0, sizeof(_req));
	_req.HFNU = 3.5;
}


FLDP::~FLDP()
{
}

int FLDP::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "IMG_CNT")){
		IMG_CNT(atoi(value));
		return 0;
	}
	//if (0 == strcmp(what, "NTC_STEADY_STATE")){
	//	NTC_STEADY_STATE(atof(value));
	//	return 0;
	//}

	if (0 == strcmp(what, "SCREEN_REFLECTANCE")){
		SCREEN_REFLECTANCE(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "POWER_IN_FOV")){
		POWER_IN_FOV(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "POWER_EFFICIENCY")){
		POWER_EFFICIENCY(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "HFNU")){
		HFNU(atof(value));
		return 0;
	}
	//if (0 == strcmp(what, "I_DR")){
	//	I_DR(atof(value));
	//	return 0;
	//}
	//if (0 == strcmp(what, "V_FOR")){
	//	V_FOR(atof(value));
	//	return 0;
	//}
	if (0 == strcmp(what, "LFNU_P_C")){
		LFNU_P_C(value);
		return 0;
	}
	if (0 == strcmp(what, "OPTICAL_POWER_MAX")){
		OPTICAL_POWER_MAX(value);
		return 0;
	}
	if (0 == strcmp(what, "OPTICAL_POWER_MIN")){
		OPTICAL_POWER_MIN(value);
		return 0;
	}
	if (0 == strcmp(what, "OPTICAL_POWER_AVG")){
		OPTICAL_POWER_AVG(value);
		return 0;
	}
	if (0 == strcmp(what, "SOCKET_DCR")){
		SOCKET_DCR(atof(value));
		return 0;
	}
	if (0 == strcmp(what, "FORWARD_V")){
		FORWARD_V(value);
		return 0;
	}
	if (0 == strcmp(what, "FORWARD_I")){
		FORWARD_I(value);
		return 0;
	}
	return -1;
}



tMTCP_payload_TEST_RSP FLDP::SendFLDP()
{
	_req.IMG_CNT = 9;
	int ret = SendFrame(kMTCP_CTRL_FLDP, (void*)&_req, sizeof(_req), nullptr, 0);
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));

	RecvResponse(&rep, sizeof(rep));

	return rep;

}
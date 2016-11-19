#include "../mtcp_module.h"


TSCR::TSCR()
{
	memset(&m_TSCR_REQ, 0, sizeof(m_TSCR_REQ));
	strcpy((char *)m_TSCR_REQ.STATION_SW_VER, "V1.0");
}


TSCR::~TSCR()
{
}

int TSCR::setDUTSN(const char * sn)
{
	memcpy((char *)m_TSCR_REQ.DUT_SN, sn,32);
	return 0;
}
int TSCR::setSocketSN(const char* sn)
{
	memcpy((char*)m_TSCR_REQ.SOCKET_SN, sn,32);
	return 0;
}

int TSCR::setMode(int mode)
{
	m_TSCR_REQ.TEST_MODE = mode;
	return 0;
}

int TSCR::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "SOCKET_ID")){
		setSlotID(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "SOCKET_SN")){
		setSocketSN(value);
		return 0;
	}
	if (0 == strcmp(what, "STATION_ID")){
		setStationID(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "STATION_SW_VER")){
		setSWVersion(value);
		return 0;
	}
	if (0 == strcmp(what, "DUT_SN")){
		setDUTSN(value);
		return 0;
	}
	if (0 == strcmp(what, "TEST_MODE")){
		setMode(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "SOCKET_DCR")){
		setSOCKETDCR(atof(value));
	}
	return -1;
}

tMTCP_payload_TSCR_RSP TSCR::SendTSCR()
{
	int ret = SendFrame(kMTCP_CTRL_TSCR, &m_TSCR_REQ, sizeof(m_TSCR_REQ),nullptr, 0);
	tMTCP_payload_TSCR_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TSCR_RSP));

	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;
}

int TSCR::setSlotID(int slotid)
{
	m_TSCR_REQ.SOCKET_ID = slotid;
	return 0;
}

int TSCR::setStationID(int station)
{
	m_TSCR_REQ.STATION_ID = station;
	return 0;
}

int TSCR::setSWVersion(const char * sw)
{
	memcpy((char *)m_TSCR_REQ.STATION_SW_VER, sw,32);
	return 0;
}

int TSCR::setSOCKETDCR(float dcr)
{
	m_TSCR_REQ.SOCKET_DCR = dcr;
	return 0;
}

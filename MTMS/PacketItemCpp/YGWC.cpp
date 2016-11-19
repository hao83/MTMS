#include "../mtcp_module.h"
YGWC::YGWC()
{
	memset(&_req, 0, sizeof(_req));
}
YGWC::~YGWC()
{

}

	
int YGWC::Y_NVM_1(char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		_req.Y_NVM_1[index++] = atof(k);
		if (index >= 18) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}
int YGWC::Y_NVM_2(char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		_req.Y_NVM_2[index++] = atof(k);
		if (index >= 18) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}
int YGWC::Y_NVM_3(char* data)
{
	char * p = new char[strlen(data) + 1];
	strcpy(p, data);

	char * dd;
	char * k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		printf("get : %s\r\n", k);
		_req.Y_NVM_3[index++] = atof(k);
		if (index >= 18) break;
		k = strtok(NULL, ",");
	}
	delete[] p;
	p = nullptr;
	return 0;
}
int YGWC::setvalue(char*  what, char*  value)
{
	if (0 == strcmp(what, "CONFIG_CRC_H")){
		CONFIG_CRC_H(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "CONFIG_CRC_L")){
		CONFIG_CRC_L(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "PROG0_REG")){
		PROG0_REG(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "CONFIG_CRC_H_CALC")){
		CONFIG_CRC_H_CALC(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "CONFIG_CRC_L_CALC")){
		CONFIG_CRC_L_CALC(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "Y_NVM_1")){
		Y_NVM_1(value);
		return 0;
	}
	if (0 == strcmp(what, "Y_NVM_2")){
		Y_NVM_2(value);
			return 0;
		}
	if (0 == strcmp(what, "Y_NVM_3")){
		Y_NVM_3(value);
		return 0;
	}
	return 0;
}
tMTCP_payload_TEST_RSP YGWC::SendYGWC()
{
	int ret = SendFrame(kMTCP_CTRL_YGWC, &_req, sizeof(_req), nullptr, 0);
	tMTCP_payload_TEST_RSP rep;
	memset(&rep, 0, sizeof(tMTCP_payload_TEST_RSP));
	int recv_cnt = RecvResponse(&rep, sizeof(rep));
	return rep;
}

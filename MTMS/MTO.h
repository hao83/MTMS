// 系统内部消息传递 的数据格式对象

#ifndef __MTO_H__
#define __MTO_H__

#include "MTCProto.h"

#pragma pack(1)
typedef struct MTO_DATA{
	uint32_t ID;
	uint8_t type;
	uint32_t target;
	void* data;

}MTO_DATA;
#pragma pack()

class MTO
{
public:
	MTO();
	~MTO();
	
private:
	MTO_DATA _mto;
};

#endif
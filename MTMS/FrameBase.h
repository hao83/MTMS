#ifndef __FRAMEBASE__H__
#define __FRAMEBASE__H__

#pragma once

#include "MTCProto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/***********����������־**************/
//#include "CLog.h"
//#define SLOG1(x)			CLog::Instance()->WriteLog(x);
//#define SLOG2(x, y)			CLog::Instance()->WriteJoinLog(x, y);	
//#define LOG1(x)				CLog::Instance()->WriteLog(__FILE__, __LINE__, x)
//#define LOG2(x, y)			CLog::Instance()->WriteJoinLog(__FILE__, __LINE__, x, y)
/***********************************/

class CFrameBase
{
public:
	CFrameBase();
	virtual ~CFrameBase();

protected:
	int SendFrame(uint32_t ctrl
		, void * payload, int payload_size
		, void * data, int datasize);
	int RecvResponse(void * buffer, int len);

	int mp_packet_header(
		INOUT void *	buffer,
		IN uint32_t    MTCP,
		IN uint32_t	   CTRL,   //  packet control id (defined below)
		IN uint32_t    PLEN,   //  payload length (if any), CTRL specific
		IN uint16_t    ERRC,   //  valid only for backward packet, tMTCP_ERR code (defined below)
		IN uint8_t     SEQN    //  forward packet sequence number
	);

	// ��REQ�ķ�������ת��ΪJson��д�뵽buffer����
	// ����ֵ:>0=Json�����ݳ���, 0=��ʾ��,-1=��ʾ����
	virtual int getResponseToJson(char * buffer, int size){ return 0; }
};

#endif // _FRAME_BASE__
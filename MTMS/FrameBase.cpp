#include "FrameBase.h"

#include <WinSock2.h>

#include <cassert>
#include <cstdlib>
#include <string>

#include "MTCP_Rosaline.h"

#include "MTCPConnection.h"

#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

#define ASSERT			assert

/*********** program log **************/
#include "CLog.h"
#define SLOG1(x)			CLog::Instance()->WriteLog(x);
#define SLOG2(x, y)			CLog::Instance()->WriteJoinLog(x, y);	
#define LOG1(x)				CLog::Instance()->WriteLog(__FILE__, __LINE__, x)
#define LOG2(x, y)			CLog::Instance()->WriteJoinLog(__FILE__, __LINE__, x, y)
/***********************************/

const size_t MP_HEAD_SIZE = sizeof(tMTCP_header);
const size_t MP_PALOAD_CHECKSUM_SIZE = 1;

CFrameBase::CFrameBase()
{
}


CFrameBase::~CFrameBase()
{
}


int CFrameBase::mp_packet_header(
	INOUT void *	buffer,
	IN uint32_t    MTCP,
	IN uint32_t	   CTRL,   //  packet control id (defined below)
	IN uint32_t    PLEN,   //  payload length (if any), CTRL specific
	IN uint16_t    ERRC,   //  valid only for backward packet, tMTCP_ERR code (defined below)
	IN uint8_t     SEQN    //  forward packet sequence number
	)
{
	ASSERT(buffer);

	tMTCP_header * ptMTCP_header = (tMTCP_header *)(buffer);

	ptMTCP_header->MTCP = (MTCP_PREAMBLE);

	// ptMTCP_header->ERRC = htons(ERRC);
	// ptMTCP_header->CTRL = htonl(CTRL);
	// ptMTCP_header->PLEN = htonl(PLEN);

	ptMTCP_header->MTCP = (MTCP);
	ptMTCP_header->ERRC = (ERRC);
	ptMTCP_header->CTRL = (CTRL);

	if (PLEN > 0)
	{
		ptMTCP_header->PLEN = (PLEN + 1);
	}
	else
	{
		ptMTCP_header->PLEN = (PLEN);
	}

	//ptMTCP_header->PLEN = 0x2000002;
	ptMTCP_header->SEQN = SEQN;

	ptMTCP_header->H_CS = _MTCP_calculateCheckSum((uint8_t*)buffer, 15);

	/*char * p = (char *)buffer;
	for (int k = 0; k < 13; k++)
	{
		printf("buffer[%d]:0x%02x,%c\r\n", k, p[k], p[k]);
	}
	printf("pHeader->PLEN:0x%x\r\n", ptMTCP_header->PLEN);
	printf("H_CS:%02X\r\n", ptMTCP_header->H_CS);*/
	

	return MP_SUCCESSFUL;
}


int CFrameBase::SendFrame(uint32_t ctrl, void * payload
	, int payload_size
	, void * data, int datasize)
{
	///-------header-payload-data-checksum
	/////

	ASSERT(payload_size >= 0);
	ASSERT(datasize >= 0);

	int PALOAD_SIZE = payload_size;
	int PACKET_SIZE = MP_HEAD_SIZE + payload_size + datasize;

	if ((payload_size + datasize) > 0)
	{
		PACKET_SIZE += MP_PALOAD_CHECKSUM_SIZE;
	}

	// packet 
	char * buffer = new char[PACKET_SIZE + 1];
	//memset(buffer, 0, PACKET_SIZE + 1);
	buffer[PACKET_SIZE] = 0;

	uint32_t plen = payload_size + datasize;

	//package header
	mp_packet_header(buffer, MTCP_PREAMBLE, ctrl, plen, 0, 0);

	//copy payload
	if (payload_size > 0)
	{
		memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)payload, payload_size);
	}

	//copy data
	if (datasize > 0)
	{
		memcpy((void*)(buffer + MP_HEAD_SIZE + payload_size), (void*)data, datasize);
	}

	//calc check sum
	if ((payload_size + datasize)> 0)
	{
		buffer[MP_HEAD_SIZE + payload_size + datasize] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), payload_size + datasize);
	}

	/*
	printf("Will Send data : ");
	for (size_t i = 0; i < MP_HEAD_SIZE + payload_size + datasize; i++)
 	{
		printf("0x%02x\t", buffer[i]);
	}
	printf("\r\n");
	*/

	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);

	delete[] buffer;

	if (snd_cnt != PACKET_SIZE)
	{
		return MP_SOCKET_FAILED;
	}

	// printf("send message to napa sucess...............\n");

	return MP_SUCCESSFUL;
}

int CFrameBase::RecvResponse(void * buffer, int len)
{
	//Read header
	tMTCP_header header;
	int rcv_cnt = mp_recv_data((char *)&header, sizeof(header));
	if (rcv_cnt != sizeof(header))
	{
		printf("Receive header failed!\r\n");
		return MP_FAILED;
	}
#if _DEBUG
	else
	{
		char * p = (char *)&header;
		printf("Header raw data:\r\n");
		for (size_t i = 0; i < sizeof(header); i++)
		{
			printf("0x%02x,", p[i]);
		}
		printf("\r\n");
		printf("Payload length : %d\r\n", header.PLEN);
	}
#endif //_DEBUG

	//check checksum
	uint8_t h_cs = _MTCP_calculateCheckSum((uint8_t*)&header, sizeof(header) - 1);
	if (h_cs != header.H_CS)
	{
		char temp[128];
		sprintf_s(temp, 127, "warning!!!! host pHdr->H_CS(%d)!=_MTCP_calculateCheckSum(%d)", header.H_CS, h_cs);
		SLOG1(temp);
		printf("%s\n", temp);
	}

	//check error
	if (kMTCP_ERR_OK != header.ERRC)
	{
		char temp[128];
		sprintf_s(temp, 127, "warning!!!! MTCP HOST error code:%d", header.ERRC);
		SLOG1(temp);
		printf("%s\n", temp);

		if (_MTCP_isCloseOnError_errorCode(header.ERRC))
		{
			mp_close_session();
		}

		return header.ERRC * (-1);
	}

	// successful!!!
	if (header.PLEN <= 0)
	{
		return MP_SUCCESSFUL;
	}

	//Read payload
	int payLen = header.PLEN - 1;
	if (payLen < len)
	{
		char temp[128];
		sprintf_s(temp, 127, "warning!!!! buffer (%d, %d) size", payLen, len);
		SLOG1(temp);
		printf("%s\n", temp);

		return MP_MTCP_HOST_FAILED;
	}

	rcv_cnt = mp_recv_data((char *)buffer, payLen);
	if (payLen != rcv_cnt)
	{
		char temp[128];
		sprintf_s(temp, 127, "warning!!!! recv payload error(%d, %d) size", payLen, rcv_cnt);
		SLOG1(temp);
		printf("%s\n", temp);

		return MP_SOCKET_FAILED;
	}

	//read check sum
	uint8_t checksum = _MTCP_calculateCheckSum((uint8_t*)buffer, payLen);
	uint8_t p_cs = 0;
	rcv_cnt = mp_recv_data((char*)&p_cs, 1);
	if (1 != rcv_cnt)
	{
		char temp[128];
		sprintf_s(temp, 127, "warning!!!! recv payload checksum error(%d, %d) size", 1, rcv_cnt);
		SLOG1(temp);
		printf("%s\n", temp);
		return MP_SOCKET_FAILED;
	}

	if (checksum != p_cs)
	{
		char temp[128];
		sprintf_s(temp, 127, "warning!!!! host P_CS(%d)!=_MTCP_calculateCheckSum(%d)", checksum, p_cs);
		SLOG1(temp);
		printf("%s\n", temp);
	}

	return payLen;
}

//////////////////////////////////////////////////////////////////////////
// end file(s)!!!
//////////////////////////////////////////////////////////////////////////
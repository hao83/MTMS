//////////////////////////////////////////////////////////////////////////
//
// File Name		: MTCProto.cpp
//
// Description		: Defines the exported functions for the DLL application.
//
// Author			: James.Gu
//
// Date				: 09/25/2016
//
//////////////////////////////////////////////////////////////////////////
#include "MTCProto.h"
#include "MTCPConnection.h"

#include "MTCP_Rosaline.h"

#include <WinSock2.h>

#include <cassert>
#include <cstdlib>
#include <string>


#pragma comment(lib, "ws2_32.lib")

#define ASSERT			assert

//const size_t MP_HEAD_SIZE = sizeof(tMTCP_header);
//const size_t MP_PALOAD_CHECKSUM_SIZE = 1;
//
//
//// packet message header
//static int mp_packet_header(
//	INOUT void *	buffer,
//	IN uint32_t	   CTRL,   //  packet control id (defined below)
//	IN uint32_t    PLEN,   //  payload length (if any), CTRL specific
//	IN uint16_t    ERRC,   //  valid only for backward packet, tMTCP_ERR code (defined below)
//	IN uint8_t     SEQN    //  forward packet sequence number
//	)
//{
//	ASSERT(buffer);
//
// 	tMTCP_header * ptMTCP_header = (tMTCP_header *)(buffer);
//
//	ptMTCP_header->MTCP = (MTCP_PREAMBLE);
//
//	// ptMTCP_header->ERRC = htons(ERRC);
//	// ptMTCP_header->CTRL = htonl(CTRL);
//	// ptMTCP_header->PLEN = htonl(PLEN);
//
//	ptMTCP_header->ERRC = (ERRC);
//	ptMTCP_header->CTRL = (CTRL);
//
//	if (PLEN > 0)
//	{
//		ptMTCP_header->PLEN = (PLEN + 1);
//	}
//	else
//	{
//		ptMTCP_header->PLEN = (PLEN);
//	}
//
//	ptMTCP_header->SEQN = SEQN;
//
//	ptMTCP_header->H_CS = _MTCP_calculateCheckSum((uint8_t*)buffer, 13);
//
//	return MP_SUCCESSFUL;
//}


// TSCR
//int mp_tscr_req(
//IN tMTCP_payload_TSCR_REQ * req,
//OUT tMTCP_payload_TSCR_RSP * rsp)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_TSCR_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, PALOAD_SIZE);
//
//	mp_packet_header(buffer, kMTCP_CTRL_TSCR, PALOAD_SIZE, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer+MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_TSCR_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//// TEST
//int mp_test_req(OUT tMTCP_payload_TEST_RSP * rsp)
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//// !!!!!!kMTCP_CTRL_TEST no found!!!!!!
//#ifndef kMTCP_CTRL_TEST
//#define kMTCP_CTRL_TEST		'test'
//#endif //kMTCP_CTRL_TEST 
//
//	mp_packet_header(buffer, kMTCP_CTRL_TEST, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_TEST_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//
//// TSED
//int mp_tsed_req()
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	mp_packet_header(buffer, kMTCP_CTRL_TSED, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// SEQU
//int mp_sequ_req(
//OUT tMTCP_payload_SEQU_RSP * rsp
//)
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	mp_packet_header(buffer, kMTCP_CTRL_SEQU, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_SEQU_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//
//// INIT
//int mp_init_req(
//IN const tMTCP_payload_INIT_REQ * req)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_INIT_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_INIT, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// VTST
//int mp_vtst_req(
//IN const tMTCP_payload_VTST_PREDA_REQ * req,
//IN const tMTCP_data_TTDP * pTTDP)
//{
//	ASSERT(req);
//	ASSERT(pTTDP);
//
//
//	return 0;
//}
//
//
//// DPCK
//int mp_dpck_req(
//IN const tMTCP_payload_DPCK_REQ * req)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_DPCK_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_DPCK, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// DPWR
//int mp_dpwr_req(
//OUT tMTCP_payload_DPWR_RSP * rsp)
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	mp_packet_header(buffer, kMTCP_CTRL_DPWR, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_DPWR_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//
//// DPWC
//int mp_dpwc_req(
//IN const tMTCP_payload_DPWC_REQ * req)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_DPWC_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_DPWC, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//// VCAL
//int mp_vcal_req(
//IN const tMTCP_payload_VCAL_REQ	* req,
//IN const tMTCP_data_TTDP * pTTDP,		// array
//IN tMTCP_data_ADIP * pADIP				// array
//)
//{
//	return 0;
//}
//
//// VDCR
//int mp_vdcr_req(
//OUT tMTCP_payload_VDCR_RSP * rsp)
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	mp_packet_header(buffer, kMTCP_CTRL_VDCR, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_VDCR_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//
//// C1SS
//int mp_c1ss_req(
//IN const tMTCP_payload_CTSS_REQ * req,
//OUT void * rsp)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_CTSS_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_CTSS, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//
//// EMCT
//int mp_emct_req(
//IN const tMTCP_payload_EMCT_REQ * req,
//IN const void * imageData)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_EMCT_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_EMCT, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// BMPF
//int mp_bmpf_req(
//IN const tMTCP_payload_BMPF_REQ * req,
//IN const void * imgRawData)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_BMPF_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_BMPF, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// FLDP
//int mp_fldp_req(
//IN const tMTCP_payload_FLDP_REQ * req)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_FLDP_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_FLDP, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// FLDI
//int mp_fldi_req(
//IN const tMTCP_payload_FLDI_REQ * req,
//IN const uint16_t * pFlatFieldCal,
//IN void * imgRawData)
//{
//	return 0;
//}
//
//
//// C1ES
//int mp_c1es_req(
//IN const tMTCP_payload_CTES_REQ * req)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_CTES_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_CTES, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// ALPR
//int mp_alpr_req(
//OUT tMTCP_payload_ALPR_RSP * rsp)
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	mp_packet_header(buffer, kMTCP_CTRL_ALPR, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_ALPR_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//
//// ALPH
//int mp_alph_req(
//IN const tMTCP_payload_ALPH_REQ * req,
//OUT tMTCP_payload_ALPR_RSP * rsp)
//{
//	ASSERT(req);
//
//	const int PALOAD_SIZE = sizeof(tMTCP_payload_ALPH_REQ);
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)req, plen);
//
//	mp_packet_header(buffer, kMTCP_CTRL_ALPH, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//
//// POST
//int mp_post_req(
//OUT tMTCP_payload_POST_RSP * rsp)
//{
//	const int PALOAD_SIZE = 0;
//	const int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char buffer[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//
//	mp_packet_header(buffer, kMTCP_CTRL_POST, plen, 0, 0);
//
//	buffer[MP_HEAD_SIZE + PALOAD_SIZE] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	if (NULL == rsp)
//	{
//		return MP_SUCCESSFUL;
//	}
//
//	const int RSP_PACK_SIZE = sizeof(tMTCP_payload_POST_RSP);
//	const int RCV_BUFFER_SIZE = MP_HEAD_SIZE + RSP_PACK_SIZE;
//	char rcv_buf[RCV_BUFFER_SIZE + 1];
//	rcv_buf[RCV_BUFFER_SIZE] = 0;
//
//	int rcv_cnt = mp_recv_data(rcv_buf, RCV_BUFFER_SIZE);
//	if (RCV_BUFFER_SIZE != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	memcpy((void*)rsp, (void*)(rcv_buf + MP_HEAD_SIZE), RSP_PACK_SIZE);
//
//	return MP_SUCCESSFUL;
//}
//
//
//// YGCL
//int mp_ygcl_req()
//{
//	return 0;
//}
//
//
//// YGTS
//int mp_ygts_req(){
//	return 0;
//}
//
////////////////////////////////////////////////////////////////////////////
////
////////////////////////////////////////////////////////////////////////////
//int SendFrame(uint32_t ctrl,void * payload
//	, int payload_size
//	, void * data, int datasize)
//{
//
//	///-------header-payload-data-checksum
//	/////
//
//	int PALOAD_SIZE = payload_size;
//	int PACKET_SIZE = MP_HEAD_SIZE + PALOAD_SIZE + datasize+ MP_PALOAD_CHECKSUM_SIZE;
//
//	// packet 
//	char *buffer = new char[PACKET_SIZE + 1];
//	buffer[PACKET_SIZE] = 0;
//
//	uint32_t plen = PALOAD_SIZE;
//	
//	//package header
//	mp_packet_header(buffer, ctrl, plen, 0, 0);
//
//	//copy payload
//	if (payload_size)
//	{
//		memcpy((void*)(buffer + MP_HEAD_SIZE), (void*)payload, plen);
//	}
//
//	//copy data
//	if (datasize)
//	{
//		memcpy((void*)(buffer + MP_HEAD_SIZE+payload_size), (void*)data, datasize);
//	}
//
//	//calc check sum
//	buffer[MP_HEAD_SIZE + payload_size+datasize] = _MTCP_calculateCheckSum((uint8_t*)(buffer + MP_HEAD_SIZE), PALOAD_SIZE);
//
//	int snd_cnt = mp_send_data(buffer, PACKET_SIZE);
//	delete[] buffer;
//	if (snd_cnt != PACKET_SIZE)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}
//
//int RecvResponse(void * buffer, int len)
//{
//	int rcv_cnt = mp_recv_data((char *)buffer, len);
//	if (len != rcv_cnt)
//	{
//		return MP_SOCKET_FAILED;
//	}
//
//	return MP_SUCCESSFUL;
//}

//////////////////////////////////////////////////////////////////////////
// end file(s)
//////////////////////////////////////////////////////////////////////////

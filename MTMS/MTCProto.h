//////////////////////////////////////////////////////////////////////////
//
// File Name		: MTCProto.h
//
// Description		: Defines the exported functions for the DLL application.
//
// Author			: James.Gu
//
// Date				: 09/25/2016
//
//////////////////////////////////////////////////////////////////////////
#ifndef __MTCPROTOCOL__H__
#define __MTCPROTOCOL__H__

#include "ml_mtcp.h"
#include "MTCP_Rosaline.h"

#define IN	
#define OUT
#define INOUT

#define MP_SUCCESSFUL		0					// successfult

#define MP_FAILED			-999				// failed
#define MP_MTCP_HOST_FAILED	-998				// socket failed
#define MP_SOCKET_FAILED	-997				// socket failed


#if (_WIN32)
#define MTCP_EXPORT		__declspec(dllexport)
#else //
// ...
#endif //

typedef unsigned int		uint32_t;
typedef unsigned short		uint16_t;
typedef unsigned char		uint8_t;

typedef int			int32_t;
typedef short		int16_t;


//#ifdef __cplusplus
//extern "C"{
//#endif // __cplusplus
//
//	// TSCR
//	MTCP_EXPORT
//		int mp_tscr_req(
//		IN tMTCP_payload_TSCR_REQ * req,
//		OUT tMTCP_payload_TSCR_RSP * rsp
//		);
//
//	// TEST
//	MTCP_EXPORT
//		int mp_test_req(
//		OUT tMTCP_payload_TEST_RSP * rsp
//		);
//
//	// TSED
//	MTCP_EXPORT
//		int mp_tsed_req();
//
//	// SEQU
//	MTCP_EXPORT
//		int mp_sequ_req(
//		OUT tMTCP_payload_SEQU_RSP * rsp
//		);
//
//	// INIT
//	MTCP_EXPORT
//		int mp_init_req(
//		IN const tMTCP_payload_INIT_REQ * req
//		);
//
//	// VTST
//	MTCP_EXPORT
//		int mp_vtst_req(
//		IN const tMTCP_payload_VTST_POSTDA_REQ * reqPost,
//		IN const tMTCP_payload_VTST_PREDA_REQ * reqPre,
//		IN const tMTCP_data_TTDP * pTTDP		//array
//		);
//
//	// DPCK
//	MTCP_EXPORT
//		int mp_dpck_req(
//		IN const tMTCP_payload_DPCK_REQ * req
//		);
//
//	// DPWR
//	MTCP_EXPORT
//		int mp_dpwr_req(
//		OUT tMTCP_payload_DPWR_RSP * rsp
//		);
//
//	// DPWC
//	MTCP_EXPORT
//		int mp_dpwc_req(
//		IN const tMTCP_payload_DPWC_REQ * req
//		);
//
//	// VCAL
//	MTCP_EXPORT
//		int mp_vcal_req(
//		IN const tMTCP_payload_VCAL_REQ	* req,
//		IN const tMTCP_data_TTDP * pTTDP,		// array
//		IN tMTCP_data_ADIP * pADIP				// array
//		);
//
//	// VDCR
//	MTCP_EXPORT
//		int mp_vdcr_req(
//		OUT tMTCP_payload_VDCR_RSP * rsp
//		);
//
//	// C1SS
//	MTCP_EXPORT
//		int mp_c1ss_req(
//		IN const tMTCP_payload_CTSS_REQ * req
//		);
//
//
//	// EMCT
//	MTCP_EXPORT
//		int mp_emct_req(
//		IN const tMTCP_payload_EMCT_REQ * req,
//		IN const void * imageData
//		);
//
//	// BMPF
//	MTCP_EXPORT
//		int mp_bmpf_req(
//		IN const tMTCP_payload_BMPF_REQ * req,
//		IN const void * imgRawData
//		);
//
//
//	// FLDP
//	MTCP_EXPORT
//		int mp_fldp_req(
//		IN const tMTCP_payload_FLDP_REQ * req
//		);
//
//
//	// FLDI
//	MTCP_EXPORT
//		int mp_fldi_req(
//		IN const tMTCP_payload_FLDI_REQ * req,
//		IN const uint16_t * pFlatFieldCal,
//		IN void * imgRawData
//		);
//
//	// C1ES
//	MTCP_EXPORT
//		int mp_c1es_req(
//		IN const tMTCP_payload_CTES_REQ * req
//		);
//
//	// ALPR
//	MTCP_EXPORT
//		int mp_alpr_req(
//		OUT tMTCP_payload_ALPR_RSP * rsp
//		);
//
//
//	// ALPH
//	MTCP_EXPORT
//		int mp_alph_req(
//		IN const tMTCP_payload_ALPH_REQ * req,
//		OUT tMTCP_payload_ALPR_RSP * rsp
//		);
//
//
//	// POST
//	MTCP_EXPORT
//		int mp_post_req(
//		OUT tMTCP_payload_POST_RSP * rsp
//		);
//
//	// YGCL
//	MTCP_EXPORT
//		int mp_ygcl_req(
//		);
//
//
//	// YGTS
//	MTCP_EXPORT
//		int mp_ygts_req(
//		);
//
//
//	int SendFrame(void * payload
//		, int payload_size
//		, void * data = nullptr, int datasize = 0);
//	int RecvResponse(void * buffer, int len);
//#ifdef __cplusplus
//}
//#endif //__plusplus

//////////////////////////////////////////////////////////////////////////
#endif //__MTCPROTOCOL__H__
//////////////////////////////////////////////////////////////////////////



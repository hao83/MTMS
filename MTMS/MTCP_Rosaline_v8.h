//
//  ml_rosaline_mtcp.h
//  Rosaline
//
//  Created by Yida Zhang on 8/8/16.
//  Copyright © 2016 Apple. All rights reserved.
//

#ifndef _ML_ROSALINE_MTCP_H_
#define _ML_ROSALINE_MTCP_H_

/*
 *  revision history
 *  rev1    08/08/16     initial revision
 *  rev2    09/15/16     implement all existing test coverage packets
 *  rev3    09/15/16     add chamber info in tMTCP_payload_SEQU_RSP
 *                       update tMTCP_payload_TSCR_REQ to include chamber and socket info
 *                       update tMTCP_payload_SEQU_RSP to include chamber info
 *                       update tMTCP_payload_INIT_REQ to include lot info
 *                       add FWHM field in tMTCP_payload_VTST_REQ
 *  rev4    09/17/16     correct doppler nvm data write request to tMTCP_payload_DPWR_RSP
 *                       correct the size description for tMTCP_payload_DPWC_REQ
 *                       add peak wavelength and fwhm wavelength in tMTCP_payload_VCAL_REQ
 *  rev5    09/19/16     correct payload length tMTCP_payload_EMCT_REQ, add IMG_SIZE
 *                       add IMG_INDEX in tMTCP_payload_FLDI_REQ, remove MTC_STEADY_STATE field
 *  rev6    09/21/16     tMTCP_data_ADIP add voltage and optical power raw reading
 *                       tMTCP_payload_C1SS_REQ add wavelength, I_cal and forward voltage
 *                       tMTCP_payload_FLDP_REQ add power efficiency and 9 individual LFNU
 *  rev7    09/22/16     tMTCP_payload_VCAL_REQ add Voltage in final result
 *  rev8    09/23/16     tMTCP_payload_EMCT_REQ add drive current and forward voltage
 *                       tMTCP_payload_BMPF_REQ add drive current and forward voltage
 *                       tMTCP_payload_FLDP_REQ add drive current and forward voltage and optical power for each region
 *                       tMTCP_payload_ALPH_REQ add drive current and voltage
 * rev9 09/26/16 add different port for PreBI, PreDA, PostDA, Alpha
 * rev10 10/18/16 add tMTCP_payload_YGTS_REQ for Yogi Fault Condition Test, add tMTCP_payload_YGTR_RSP for current
request
 *					     add tMTCP_payload_YGWR_RSP and tMTCP_payload_YGWC_REQ for Yogi NVM Write test
 *						 rev11 10/23/16 add SOCKET_DCR for tMTCP_payload_TSCR_REQ
 *						 add Lot size, diffuser lot info and operator info in tMTCP_payload_INIT_REQ
 *						 tMTCP_payload_VTST_REQ add first pulse optical power, remove reverse I/V
 *						 tMTCP_payload_DPCK_REQ extend to have Yogi register field
 *						 tMTCP_payload_VDCR_RSP remove DCR and T_STEADY_STATE field
 *						 tMTCP_payload_YGTS_REQ remove trace Id, add statue pin level and current
 *
 */

#include "ml_mtcp.h"

/*  MTCP server port
 */
#define MTCP_PORT_DEFAULT			(61804)
#define MTCP_PORT_PREBI				(61805)
#define MTCP_PORT_PREDA				(61806)
#define MTCP_PORT_POSTDA			(61807)
#define MTCP_PORT_ALPHA				(61808)
/*------------------------------------------------------------------------------
 *  Generic Test Result Response Packet
 *  Direction
 *      Apple Test SW -> Tester Controller
 *  Payload
 *      tMTCP_payload_TEST_RSP
 *      . P_CS (1-byte)
 *  Note
 *      CTRL of the header is the transaction specific.
 *      field description below
 *      Tester Controller will evaluate the information:
 *          T_ERRC      the error code of the evaluation
 *          T_ERRS      the error string corresponding ERRC
 */
#pragma pack(1)
typedef struct tMTCP_payload_TEST_RSP{
	uint16_t    T_ERRC;     // result error code, 0 is ok
	uint8_t     T_ERRS[30]; // error string
} tMTCP_payload_TEST_RSP, *tMTCP_payload_TEST_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Test Session Creation
 */

#define kMTCP_CTRL_TSCR     'tscr'
/*  Tester Connect Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  payload
 *      none
 *  Note
 *      at the beginning, tester controller send this packet to
 *      request connection to Apple Test SW
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_TSCR_Deprecated_REQ{
	uint8_t		SOCKET_ID;			// dut id on socket, position 0, 1, 2
	int8_t		SOCKET_SN[32];		// socket serial number
	uint8_t		STATION_ID;			// Test Chamber ID, 0 for loading/unloading
	int8_t		STATION_SW_VER[32];	// Test Chamber SW Version, 31 char max
	int8_t		DUT_SN[32];			// dut serial number, 31 char max
	uint8_t		TEST_MODE;			// Mode to Run Test, 0 : Normal, 1 : GRR, 2 : Audit
} tMTCP_payload_TSCR_Deprecated_REQ, *tMTCP_payload_TSCR_Deprecated_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_TSCR_REQ{
	uint8_t		SOCKET_ID;			// dut id on socket, position 0, 1, 2
	int8_t      SOCKET_SN[32];		// socket serial number
	float		SOCKET_DCR;			// DCR value for current socket, in mOhm
	uint8_t		STATION_ID;			// Test Chamber ID, 0 for loading/unloading
	int8_t      STATION_SW_VER[32];	// Test Chamber SW Version, 31 char max
	int8_t      DUT_SN[32];			// dut serial number, 31 char max
	uint8_t		TEST_MODE;			// Mode to Run Test, 0 : Normal, 1 : GRR, 2 : Audit
} tMTCP_payload_TSCR_REQ, *tMTCP_payload_TSCR_REQ_ptr;
#pragma pack()

/*
 *  Tester Connect Response
 *  Direction
 *      Apple Test SW -> Tester Controller
 *  Payload
 *      . tMTCP_payload_TSCR_RSP
 *      . P_CS (1-byte)
 *  Timeout
 *      Apple Test SW will wait maximum 10s for this response
 *
 *----------------------------------------------------------------------------*/

#pragma pack(1)
typedef struct tMTCP_payload_TSCR_RSP{
	uint8_t      HOST_SW_VERSION[32];  // apple test sw version, 31 char max
} tMTCP_payload_TSCR_RSP, *tMTCP_payload_TSCR_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Test Session Wrap Up
 */
#define kMTCP_CTRL_TSED     'tsed'

/*  Tester Disconnect Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 *  Note
 *      at the end of the test session, tester send this packet to Apple Test SW
 */

/*  Tester Disconnect Response
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_TEST_RSP
 *      . P_CS (1-byte)
 *  Timeout
 *      Test Software will wait maximum 2s for this response
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Test Sequence Information
 */
#define kMTCP_CTRL_SEQU		'sequ'
/*  Test Sequence Information Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Test Sequence Information Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_ALPR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_SEQU_RSP {
	uint8_t		TEST_ITEM_CNT;		// number of total test items
	uint32_t	TEST_CTRL_CODE[256];// list of test to be executed, label with CTRL
	uint8_t		CHAMBER_CNT;		// number of chambers
	uint8_t		CHAMBER_TO_RUN[8];	// list of chambers to run 1,2,3,4,5
} tMTCP_payload_SEQU_RSP, *tMTCP_payload_SEQU_RSP_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Test Initialization
 */
#define kMTCP_CTRL_INIT		'init'
/*  Test Initialization Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  payload
 *		. tMTCP_payload_INIT_REQ
 *      . P_CS (1-byte)
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_INIT_REQ_Deprecated{
	int8_t		TSTC_NAME[32];		// tester name, (tester SN or Etc.), 31 char max
	uint8_t		TSTC_ID;			// tester ID, 0~15
	int8_t		TSTC_SW_VER[32];	// Test Automation SW Version, 31 char max
	int8_t		LOT_NAME[32];		// lot information, 31 char max
} tMTCP_payload_INIT_Deprecated_REQ, *tMTCP_payload_INIT_REQ_Deprecated_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_INIT_REQ{
	int8_t		TSTC_NAME[32];		// tester name, (tester SN or Etc.), 31 char max
	uint8_t     TSTC_ID;			// tester ID, 0~15
	int8_t      TSTC_SW_VER[32];	// Test Automation SW Version, 31 char max
	int8_t      LOT_NAME[32];		// lot information, 31 char max
	uint32_t	LOT_SIZE;			// lot size, 4 Billion limit
	int8_t		DIFFUSER_LOT_ID[32];// diffuser lot id, 31 char max
	int8_t		OPERATOR[32];		// operator name, 31 char max
} tMTCP_payload_INIT_REQ, *tMTCP_payload_INIT_REQ_ptr;
#pragma pack()

/*  Test Initialization Response
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_TEST_RSP
 *      . P_CS (1-byte)
 *  Timeout
 *      Test Software will wait maximum 2s for this response
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Valencia LIV
 */
#define kMTCP_CTRL_VTST		'vtst'
/*  Valencia LIV Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+--------------+---+------------------------+------+
 *      |tMTCP_payload_VTST_REQ | WL Reading 1 | ~ | WL Reading WL_DATA_CNT | P_CS |
 *      +-------+---------------+--------------+---+------------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_VTST_REQ)
 *                           + sizeof(tMTCP_data_TTDP) x WL_LEN x WL_DATA_CNT
 *                           + 1 (P_CS)
 *
 */

#pragma pack(1)
typedef struct tMTCP_payload_VTST_Deprecated_REQ{
	float		NTC_INIT;			// NTC read at beginning, in C
	float		PEAK_OPTICAL_POWER[10]; // 10 pulse peak optical power, in mW
	float		REVERSE_V;			// reverse voltage, in V
	float		REVERSE_I;			// reverse current, in uA
	float		FORWARD_V;			// forward voltage, in V
	float		FORWARD_I;			// forward current, in uA
	float		NTC_STEADY_STATE;	// NTC read when DUT reach steady state, in C
	float		T_STEADY_STATE;		// Time to reach steady state, in ms
	float		PEAK_WAVELENGTH[10]; // 10 pulse peak wavelength, in nm
	float		FWHM_WAVELENGTH[10]; // 10 pulse wavelength FWHM, in nm
	uint8_t		WL_DATA_CNT;		// spectrometer reading times
	uint16_t	WL_LEN;				// data count from one spectrometer reading
} tMTCP_payload_VTST_Deprecated_REQ, *tMTCP_payload_VTST_Deprecated_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_payload_VTST_REQ{
	float		NTC_INIT;					// NTC read at beginning, in C
	float		PEAK_OPTICAL_POWER[10];// 10 pulse peak optical power, in mW
	float		PEAK_OPTICAL_POWER_0; 
	uint8_t		RSVD[4];
	float		FORWARD_V;					// forward voltage, in V
	float		FORWARD_I;					// forward current, in uA
	float		NTC_STEADY_STATE;			// NTC read when DUT reach steady state, in C
	uint8_t		RSVD2[4];
	float		PEAK_WAVELENGTH[10];		// 10 pulse peak wavelength, in nm
	float		FWHM_WAVELENGTH[10];		// 10 pulse wavelength FWHM, in nm
	uint8_t		WL_DATA_CNT;				// spectrometer reading times
	uint16_t	WL_LEN;						// data count from one spectrometer reading
} tMTCP_payload_VTST_REQ, *tMTCP_payload_VTST_REQ_ptr;
#pragma pack()
/*  Spectrometer Reading structure
 *
 *      +---------------------+---+---------------------------+
 *      | tMTCP_data_TTDP (0) | ~ | tMTCP_data_TTDP (WL_LEN-1)|
 *      +---------------------+---+---------------------------+
 *
 *      +       WL_LEN x sizeof(tMTCP_data_TTDP)
 *     Total one spctrometer reading data length = sizeof(tMTCP_data_TTDP) x WL_LEN;
 *
 */
#pragma pack(1)
typedef struct tMTCP_data_TTDP{
	float	wavelength;		// wavelength in nm
	float	measure;		// photo counts
} tMTCP_data_TTDP;
#pragma pack()

/*  Valencia LIV Test Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Doppler Communication Check
 */
#define kMTCP_CTRL_DPCK		'dpck'
/*  Doppler Communication Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_DPCK_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_DPCK_REQ{
	float	I_IDLE_LOW;			// low power idle mode current
	uint8_t NVM_DATA[128];		// NVM reading raw bytes
	uint8_t Y_DEVICE_ID_H; // High Byte for Device Id, Addr 0x00
	uint8_t Y_DEVICE_ID_L; // Low Byte for Device Id, Addr 0x01
	uint8_t Y_REVISION_MAJOR; // High Byte for Revision, Addr 0x02
	uint8_t Y_REVISION_MINOR; // Low Byte for Revision, Addr 0x03
	uint8_t Y_TRACE_ID[4]; // Trace Id, Addr 0x04 to 0x07
} tMTCP_payload_DPCK_REQ, *tMTCP_payload_DPCK_REQ_ptr;
#pragma pack()

/*  Doppler Communication Check Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Doppler NVM Data Write Request
 */
#define kMTCP_CTRL_DPWR		'dpwr'
/*  Doppler NVM Data Write Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 *
 */

/*  Doppler NVM Data Write Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_DPWR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_DPWR_RSP{
	uint8_t CNT;			// Count of bytes to write
	uint8_t ADDR[128];		// Address of bytes to write
	uint8_t DATA[128];		// data of bytes to write
} tMTCP_payload_DPWR_RSP, *tMTCP_payload_DPWR_RSP_ptr;
#pragma pack()
/*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Doppler NVM Data Write Check
 */
#define kMTCP_CTRL_DPWC		'dpwc'
/*  Doppler NVM Data Write Check Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_DPWC_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_DPWC_REQ{
	uint8_t NVM_DATA[128];		// NVM reading raw bytes
} tMTCP_payload_DPWC_REQ, *tMTCP_payload_DPWC_REQ_ptr;
#pragma pack()

/*  Doppler NVM Data Write Check Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Valencia Drive Current Calibration
 */
#define kMTCP_CTRL_VCAL		'vcal'
/*  Valencia Drive Current Calibration Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+------------+------------------+------+
 *      |tMTCP_payload_VCAL_REQ | WL Reading | Calibration Data | P_CS |
 *      +-------+---------------+------------+------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_VCAL_REQ)
 *                           + WL Reading structure size
 *                           + Calibration data structure size
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */

/*  WL Reading structure
 *
 *      +---------------------+---+---------------------------+
 *      | tMTCP_data_TTDP (0) | ~ | tMTCP_data_TTDP (WL_LEN-1)| 
 *      +---------------------+---+---------------------------+
 *
 *      +       WL_LEN x sizeof(tMTCP_data_TTDP)
 *     Total one spctrometer reading data length = sizeof(tMTCP_data_TTDP) x WL_DATA_LEN;
 *
 */

/*  Calibration data structure
 *
 *      +---------------------+---+-----------------------------+
 *      | tMTCP_data_ADIP (0) | ~ | tMTCP_data_ADIP (ITER_CNT-1)|
 *      +---------------------+---+-----------------------------+
 *
 *      +       ITER_CNT x sizeof(tMTCP_data_ADIP)
 *     Total calibration data length = ITER_CNT x sizeof(tMTCP_data_ADIP);
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_VCAL_REQ{
	float		NTC_STEADY_STATE;			// NTC read when DUT reach steady state, in C
	float		T_STEADY_STATE;				// Time to reach steady state, in ms
	float		PEAK_OPTICAL_POWER_FINAL;	// final peak optical power
	float		I_PULSE_FINAL;				// final pulse current
	float		V_PULSE_FINAL;				// final pulse voltage
	float		PEAK_WAVELENGTH;			// peak wavelength, in nm
	float		FWHM_WAVELENGTH;			// wavelength FWHM, in nm
	uint16_t	WL_DATA_LEN;				// data count from one spectrometer reading
	uint16_t	ITER_CNT;					// number of adjust current iternations
} tMTCP_payload_VCAL_REQ, *tMTCP_payload_VCAL_REQ_ptr;
#pragma pack()

#pragma pack(1)
typedef struct tMTCP_data_ADIP{
	float	I_pulse;		// I_pulse at this step
	float	V_pulse;		// forward voltage at this step
	float	optical_power;	// peak optical power with I_pulse
	float	V_pulse_raw[10];
	float	optical_power_row[340];
} tMTCP_data_ADIP;
#pragma pack()

/*  Valencia Drive Current Calibration Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Request Valencia Calibated Drive Current
 */
#define kMTCP_CTRL_VDCR		'vdcr'
/*  Valencia Calibrated Drive Current Request Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Valencia Calibrated Drive current Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_VDCR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_VDCR_RSP{
	float I_DR;				// valencia calibrated drive current
	float T_SETTLE;			// time to reach steady state
} tMTCP_payload_VDCR_RSP, *tMTCP_payload_VDCR_RSP_ptr;
#pragma pack()
/*
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 *  Class 1 Skin Safety Test
 */
#define kMTCP_CTRL_C1SS		'c1ss'
/*  Class 1 Skin Safety Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+------------+------+
 *      |tMTCP_payload_C1SS_REQ | WL Reading | P_CS |
 *      +-------+---------------+------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_C1SS_REQ)
 *                           + sizeof(tMTCP_data_TTDP) x WL_LEN
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */

/*  WL Reading structure
 *
 *      +---------------------+---+---------------------------+
 *      | tMTCP_data_TTDP (0) | ~ | tMTCP_data_TTDP (WL_LEN-1)|
 *      +---------------------+---+---------------------------+
 *
 *      +       WL_LEN x sizeof(tMTCP_data_TTDP)
 *     Total one spctrometer reading data length = sizeof(tMTCP_data_TTDP) x WL_LEN;
 *
 */
#pragma pack(1)
typedef struct 
tMTCP_payload_C1SS_REQ{
	float		NTC_STEADY_STATE;			// NTC read when DUT reach steady state, in C
	uint8_t		RSVD[8];
	float		I_DR;						// drive current, in A
	float		V_FORWARD[10];				// forward voltage for 10 pulses, in Vs
	float		PEAK_OPTICAL_POWER[10];		// total optical power for 10 pulses
	float		PEAK_WAVELENGTH;			// peak wavelength, in nm
	float		FWHM_WAVELENGTH;			// wavelength FWHM, in nm
	uint16_t	WL_DATA_LEN;				// data count from one spectrometer reading
} tMTCP_payload_C1SS_REQ, *tMTCP_payload_C1SS_REQ_ptr;
#pragma pack()

/*  Class 1 Skin Safety Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  VSCEL Emitter Test
 */
#define kMTCP_CTRL_EMCT		'emct'
/*  VSCEL Emitter Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+------------+------+
 *      |tMTCP_payload_EMCT_REQ | Image Data | P_CS |
 *      +-------+---------------+------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_EMCT_REQ)
 *                           + IMG_SIZE in bytes
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_EMCT_REQ{
	uint16_t	IMG_WIDTH;				// Image Width;
	uint16_t	IMG_HEIGHT;				// Image Height;
	uint32_t	IMG_SIZE;				// Image File Size
	float		NTC_STEADY_STATE;		// NTC read when DUT reach steady state, in C
	float		CLUSTER_THRESHOLD;		// Threshold for Clustering, percentage of max pixel intensity
	uint8_t		VALID_EMITTER_CNT;		// valid emitter counts
	uint16_t	EMITTER_CENTER_X[256];	// X position of each emitter center, in pixel
	uint16_t	EMITTER_CENTER_Y[256];	// Y position of each emitter center, in pixel
	uint16_t	EMITTER_SIZE[256];		// Number of pixels in each emitter
	float		MAX_POWER[256];			// max optical power of each emitter
	float		MIN_POWER[256];			// min optical power of each emitter
	float		AVG_POWER[256];			// average optical power of each emitter
	float		STD_POWER[256];			// optical power stanard deviation of each emitter
	float		I_DR;					// drive current
	float		V_FOR;					// forward voltage measurement
} tMTCP_payload_EMCT_REQ, *tMTCP_payload_EMCT_REQ_ptr;
#pragma pack()

/*  VSCEL Emitter Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *  VSCEL Mode Profile Test
 */
#define kMTCP_CTRL_BMPF		'bmpf'
/*  VSCEL Emitter Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+----------------+------+
 *      |tMTCP_payload_BMPF_REQ | Image Raw Data | P_CS |
 *      +-------+---------------+----------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_EMCT_REQ)
 *                           + IMG_SIZE
 *                           + 1 (P_CS)
 *
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_BMPF_REQ{
	uint16_t	IMG_WIDTH;				// Image Width;
	uint16_t	IMG_HEIGHT;				// Image Height;
	uint32_t	IMG_SIZE;				// Image Binary Size;
	float		NTC_STEADY_STATE;		// NTC read when DUT reach steady state, in C
	float		TOTAL_OPTICAL_POWER;	// total optical power
	float		BEAM_SIZE;				// beam size
	float		HOT_SPOT_POWER;			// max optical power in aperture scan
	uint16_t	HOT_SPOT_LOC_X;			// hot spot position in X
	uint16_t	HOT_SPOT_LOC_Y;			// hot spot position in Y
	float		I_DR;					// drive current
	float		V_FOR;					// forward voltage measurement
} tMTCP_payload_BMPF_REQ, *tMTCP_payload_BMPF_REQ_ptr;
#pragma pack()

/*  VSCEL Mode Profile Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Flood Performance Test
 */
#define kMTCP_CTRL_FLDP		'fldp'
/*  Flood Performance Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_FLDP_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_FLDP_REQ{
	uint8_t		IMG_CNT;				// number of images
	float		NTC_STEADY_STATE;		// NTC read when DUT reach steady state, in C
	float		SCREEN_REFLECTANCE;		// calibrated screen reflectance
	float		POWER_IN_FOV;			// power in FOV
	float		POWER_EFFICIENCY;		// optical power efficiency
	float		HFNU;
	float		LFNU_P_C[9];			// P1C1, P1C2, P1C3, P2C1, P2C2, P2C3, P3C1, P3C2, P3C3
	float		OPTICAL_POWER_MAX[9];	// Optical power max for each region
	float		OPTICAL_POWER_MIN[9];	// optical power min for each region
	float		OPTICAL_POWER_AVG[9];	// optical power avg for each region
	float		I_DR;					// drive current
	float		V_FOR;					// forward voltage measurement
} tMTCP_payload_FLDP_REQ, *tMTCP_payload_FLDP_REQ_ptr;
#pragma pack()

#define kMTCP_CTRL_FLDI		'fldi'
/*  Flood Performance Image Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *
 *      +-------+---------------+----------------------+----------------+------+
 *      |tMTCP_payload_FLDI_REQ | Flat Field Cal Array | Image Raw Data | P_CS |
 *      +-------+---------------+----------------------+----------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_FLDI_REQ)
 *                           + IMG_WIDTH x IMG_HEIGHT x sizeof(float)
 *                           + IMAGE_SIZE
 *                           + 1 (P_CS)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_FLDI_REQ{
	uint8_t		IMG_INDEX;				// Image Index;
	uint16_t	IMG_WIDTH;				// Image Width;
	uint16_t	IMG_HEIGHT;				// Image Height;
	uint32_t	IMG_SIZE;				// Image size;
	int16_t		POS_X;					// camera coordinate X
	int16_t		POS_Y;					// camera coordicate Y
	float		POWER_TO_PIXEL_RATIO;	// power to pixel ratio
} tMTCP_payload_FLDI_REQ, *tMTCP_payload_FLDI_REQ_ptr;
#pragma pack()

/*  Flood Performance Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Eye Safety Test
 */
#define kMTCP_CTRL_C1ES		'c1es'
/*  Flood Performance Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      . tMTCP_payload_FLDP_REQ
 *      . P_CS (1-byte)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_C1ES_REQ{
	uint8_t		IMG_CNT;				// number of images
	float		NTC_STEADY_STATE;		// NTC read when DUT reach steady state, in C
	float		SCREEN_REFLECTANCE;		// calibrated screen reflectance
	float		HOT_SPOT_THETA;			// theta angle for hot spot
	float		HOT_SPOT_PHI;			// phi angle for hot spot
	float		AE_POWER;				// energy level for AE
	float		OPTICAL_POWER;			//
} tMTCP_payload_C1ES_REQ, *tMTCP_payload_C1ES_REQ_ptr;
#pragma pack()

/*  Calss 1 Eye Safety Test Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      if there's MTCP error,
 *          zero payload
 *      if there's no MTCP error,
 *          . tMTCP_payload_TEST_RSP
 *          . P_CS (1-byte)
 *
 *  Note
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Alpha Measurement Position Request
 */
#define kMTCP_CTRL_ALPR		'alpr'
/*  Class 1 Alpha Measurement Position Request Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Class 1 Alpha Measurement Position Request Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_ALPR_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_ALPR_RSP {
	float	HOT_SPOT_THETA;			// theta angle for hot spot
	float	HOT_SPOT_PHI;			// phi angle for hot spot
} tMTCP_payload_ALPR_RSP, *tMTCP_payload_ALPR_RSP_ptr;
#pragma pack()
/*
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *  Class 1 Alpha Measurement Test
 */
#define kMTCP_CTRL_ALPH		'alph'
/*  Class 1 Alpha measurement Test Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload 
 *
 *      +-------+---------------+----------------+---+----------------------+------+
 *      |tMTCP_payload_ALPH_REQ | Image Binary 1 | ~ | Image Binary IMG_CNT | P_CS |
 *      +-------+---------------+----------------+---+----------------------+------+
 *
 *    Total payload length = sizeof(tMTCP_payload_ALPH_REQ)
 *                           + IMG_SIZE x IMG_CNT;
 *                           + 1 (P_CS)
 *  Note
 *
 */
#pragma pack(1)
typedef struct tMTCP_payload_ALPH_REQ{
	uint8_t		IMG_CNT;		// number of images
	uint16_t	IMG_WIDTH;		// image width
	uint16_t	IMG_HEIGHT;		// image height
	uint32_t	IMG_SIZE;		// image binary size for each image
	float		I_DR;			// drive current
	float		V_FOR;			// forward voltage
} tMTCP_payload_ALPH_REQ, *tMTCP_payload_ALPH_REQ_ptr;
#pragma pack()

/*------------------------------------------------------------------------------
 *  Data Posting to Panda
 */
#define kMTCP_CTRL_POST		'post'
/*  Data Posting to Panda Request
 *  Direction
 *      Tester Controller -> Apple Test SW
 *  Payload
 *      none
 */

/*  Data Posting to Panda Response
 *
 *  Direction
 *      Apple Test SW -> Test Controller
 *  Payload
 *      . tMTCP_payload_POST_RSP
 *      . P_CS (1-byte)
 *
 *  Note
 */
#pragma pack(1)
typedef struct tMTCP_payload_POST_RSP{
	uint16_t    T_ERRC;     // result error code, 0 is ok
	uint8_t     T_ERRS[30]; // error string
	uint8_t		T_BIN;		// test binning information
} tMTCP_payload_POST_RSP, *tMTCP_payload_POST_RSP_ptr;
#pragma pack()

// To be defined
/*------------------------------------------------------------------------------
*	Yogi Fault Condition Test
*/
#define kMTCP_CTRL_YGTS 'ygts'
/* Yogi Fault Condition Test Request
* Direction
* Tester Controller -> Apple Test SW
* Payload
* . tMTCP_payload_YGTS_REQ
* . P_CS (1-byte)
* Note
*
*/
#pragma pack(1)
typedef struct tMTCP_payload_YGTS_REQ{
	float		I_CONDITION_A;
	uint8_t		FAULT_STATUS_A;
	float		I_CONDITION_B;
	uint8_t		FAULT_STATUS_B;
	float		I_CONDITION_C;
	uint8_t		FAULT_STATUS_C;
	float		I_CONDITION_D;
	uint8_t	    FAULT_STATUS_D;
	float		I_CONDITION_E;
	uint8_t		FAULT_STATUS_E_1;
	uint8_t		FAULT_STATUS_E_2;
	uint8_t		LEVEL_STATUS_PIN;
	float		I_STATUS_PIN;
} tMTCP_payload_YGTS_REQ, *tMTCP_payload_YGTS_REQ_ptr;
#pragma pack()

/* Yogi Fault Condition Test Response
*
* Direction
* Apple Test SW -> Test Controller
* Payload
* if there's MTCP error,
* zero payload
* if there's no MTCP error,
* . tMTCP_payload_TEST_RSP
* . P_CS (1-byte)
*
* Note
*
*----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
* Yogi Calibrated Current Request
*/
#define kMTCP_CTRL_YGTR 'ygtr'
/* Yogi Calibrated Current Request
* Direction
* Tester Controller -> Apple Test SW
* Payload
* none
*
*/
/* Yogi Calibrated Current Request Response
*
* Direction
* Apple Test SW -> Test Controller
* Payload
* . tMTCP_payload_YGTR_RSP
* . P_CS (1-byte)
*
* Note
*/
#pragma pack(1)
typedef struct tMTCP_payload_YGTR_RSP{
	float I_PULSEMAX; // Yogi calibrated drive current, IPULSEMAX
} tMTCP_payload_YGTR_RSP, *tMTCP_payload_YGTR_RSP_ptr;
#pragma pack()
/*-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
* Yogi NVM Write Data Request
*/
#define kMTCP_CTRL_YGWR 'ygwr'
/* Yogi NVM Write Data Request
* Direction
* Tester Controller -> Apple Test SW
* Payload
* none
*
*/
/* Yogi NVM Write Data Request Response
*
* Direction
* Apple Test SW -> Test Controller
* Payload
* . tMTCP_payload_YGWR_RSP
* . P_CS (1-byte)
*
* Note
*/
#pragma pack(1)
typedef struct tMTCP_payload_YGWR_RSP{
	uint8_t CNT; // Count of bytes to write
	uint8_t ADDR[16]; // Address of bytes to write
	uint8_t DATA[16]; // data of bytes to write
} tMTCP_payload_YGWR_RSP, *tMTCP_payload_YGWR_RSP_ptr;
#pragma pack()
/*-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
* Yogi NVM Data Write Check Check
*/
#define kMTCP_CTRL_YGWC 'ygwc'
/* Yogi NVM Data Write Check Request
* Direction
* Tester Controller -> Apple Test SW
* Payload
* . tMTCP_payload_YGWC_REQ
* . P_CS (1-byte)
* Note
*
*/
#pragma pack(1)
typedef struct tMTCP_payload_YGWC_REQ{
	uint8_t CONFIG_CRC_H; // CRC_H from TEST_CONFIG_CRC_H Register
	uint8_t CONFIG_CRC_L; // CRC_L from TEST_CONFIG_CRC_L Register
	uint8_t PROG0_REG; //
} tMTCP_payload_YGWC_REQ, *tMTCP_payload_YGWC_REQ_ptr;
#pragma pack()
/* Yogi NVM Data Write Check Response
*
* Direction
* Apple Test SW -> Test Controller
* Payload
* if there's MTCP error,
* zero payload
* if there's no MTCP error,
* . tMTCP_payload_TEST_RSP
* . P_CS (1-byte)
*
* Note
*
*----------------------------------------------------------------------------*/
#endif /* _ML_ROSALINE_MTCP_H_ */














//
///*------------------------------------------------------------------------------
// *  Yogi Calibration
// */
//#define kMTCP_CTRL_YGCL		'ygcl'
///*------------------------------------------------------------------------------
// *  Yogi Test
// */
//#define kMTCP_CTRL_YGTS		'ygts'
//
//#endif /* _ML_ROSALINE_MTCP_H_ */

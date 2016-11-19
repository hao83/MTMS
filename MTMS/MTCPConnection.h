//////////////////////////////////////////////////////////////////////////
//
// File Name		: MTCPConnection.h
//
// Description		: Defines the exported functions for the DLL application.
//
// Author			: James.Gu
//
// Date				: 09/25/2016
//
//////////////////////////////////////////////////////////////////////////
#ifndef __MTCPCONNECTION__H__
#define __MTCPCONNECTION__H__


#if (_WIN32)
#define MP_EXPORT		__declspec(dllexport)
#else //
// ...
#endif //

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

	// open session
	MP_EXPORT 
		int mp_open_session(const char * address, int port, int timeout);

	// close session
	MP_EXPORT
		void mp_close_session();

	// send data
	MP_EXPORT
		int mp_send_data(const char * buffer, int length);

	// recv data
	MP_EXPORT
		int mp_recv_data(char * buffer, int size);

	// run
	MP_EXPORT
		int mp_service_run();

#ifdef __cplusplus
}
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
#endif //__MTCPCONNECTION__H__
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// File Name		: MTCPConnection.cpp
//
// Description		: Defines the exported functions for the DLL application.
//
// Author			: James.Gu
//
// Date				: 09/25/2016
//
//////////////////////////////////////////////////////////////////////////

#include "MTCPConnection.h"

#include <mutex>
#include <cassert>

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define MP_SUCCESSFUL		0
#define MP_SOCKET_FAILED	-2					// socket failed

#define ASSERT			assert


static std::string						peer_address = std::string("127.0.0.1");
static int								peer_port = 61804;

boost::asio::io_service			ioservice;
boost::asio::ip::tcp::socket	gs(ioservice);

int								gTimeouts = 5000;

void disconnect_handle(const boost::system::error_code& ec)
{
	if (ec)
	{
		boost::system::error_code e;
		gs.close(e);
	}
	else
	{
		//
	}
}

void time_wait_handle(const boost::system::error_code& ec)
{
	if (ec)
	{
		//
	}
}


// auto reconnect
static int reconnect()
{
	if (gs.is_open())
	{
		// std::cout << "connect server success....." << std::endl;
		return MP_SUCCESSFUL;
	}

	boost::asio::ip::tcp::endpoint
		peer_endpoint(boost::asio::ip::address::from_string(peer_address.c_str()), peer_port);
	boost::system::error_code ec;

	if (gTimeouts > 0)
	{
		boost::asio::deadline_timer timeObject(ioservice, boost::posix_time::microseconds(gTimeouts));
		gs.async_connect(peer_endpoint, disconnect_handle);
		timeObject.async_wait(time_wait_handle);
		ioservice.run_one(ec);
	}
	else
	{
		gs.connect(peer_endpoint, ec);
	}

	if (ec)
	{
		gs.close(ec);
		return MP_SOCKET_FAILED;
	}

	if (!gs.is_open())
	{
		gs.close(ec);
		return MP_SOCKET_FAILED;
	}

	return MP_SUCCESSFUL;
}

const char * getVersion()
{
	return "MTCP Wapper v2.2";
}

// open session
int mp_open_session(const char * address, int port, int timeout)
{
	assert(address);
	assert(port > 0);

	if (gs.is_open())
	{
		return MP_SUCCESSFUL;
	}

	peer_address = std::string(address);
	peer_port = port;

	boost::asio::ip::tcp::endpoint 
		peer_endpoint(boost::asio::ip::address::from_string(peer_address.c_str()), peer_port);
	boost::system::error_code ec;

	gTimeouts = timeout;

	if (timeout > 0)
	{
		boost::asio::deadline_timer timeObject(ioservice, boost::posix_time::milliseconds(timeout));
		gs.async_connect(peer_endpoint, disconnect_handle);
		timeObject.async_wait(time_wait_handle);		
		ioservice.run_one(ec);
		if (ec)
		{
			gs.close(ec);
			return MP_SOCKET_FAILED;
		}
	}
	else
	{
		gs.connect(peer_endpoint, ec);
		if (ec)
		{
			gs.close(ec);
			return MP_SOCKET_FAILED;
		}
	}

	if (!gs.is_open())
	{
		gs.close(ec);
		return MP_SOCKET_FAILED;
	}

	return MP_SUCCESSFUL;
}

// close session
void mp_close_session()
{
	boost::system::error_code ec;
	gs.close(ec);
}

int mp_service_run()
{
	boost::system::error_code ec;

	while (true)
	{
		ioservice.run(ec);
	};

	return 0;
}


// send a message
int mp_send_data(const char * buffer, int length)
{
	ASSERT(buffer);
	ASSERT(length > 0);

	if (!gs.is_open())
	{
		reconnect();

		if (!gs.is_open())
		{
			return MP_SOCKET_FAILED;
		}
	}

	ASSERT(gs.is_open());

	boost::system::error_code ec;

	size_t snd_cnt = 0;
	while (snd_cnt<length)
	{
		size_t snd = gs.write_some(boost::asio::buffer((buffer+snd_cnt), (length-snd_cnt)), ec);
		if ((ec == boost::asio::error::eof) || (snd<0))
		{
			gs.close(ec);
			return MP_SOCKET_FAILED;
		}

		snd_cnt += snd;
	};

	return snd_cnt;
}

// read some data from peer host
int mp_recv_data(char * buffer, int size)
{
	ASSERT(buffer);
	ASSERT(size > 0);

	if (!gs.is_open())
	{
		reconnect();
		if (!gs.is_open())
		{
			return MP_SOCKET_FAILED;
		}
	}

	boost::system::error_code ec;

	size_t rcv_cnt = 0;
	size_t rcv = 0;
	while (rcv_cnt<size)
	{
		rcv = gs.read_some(boost::asio::buffer((buffer+rcv_cnt), (size-rcv_cnt)), ec);
		if ((ec==boost::asio::error::eof) || (rcv<0))
		{
			gs.close(ec);
			return MP_SOCKET_FAILED;
		}
		if (rcv == 0){
			break;
		}
		rcv_cnt += rcv;
	};

	return rcv_cnt;
}

int Open(const char * address, int port, int timeout)
{
	return mp_open_session(address, port, timeout);
}

void Close()
{
	mp_close_session();
}

/////////////////////////////////
//int ReadDataFromFile_float(const char * file, float * data,int * count)
//{
//	FILE * f = fopen(file, "r");
//	if (f==NULL)
//	{
//		count = 0;
//		return -1;
//	}
//	fseek(f, 0, SEEK_END);
//	long size = ftell(f);
//	char * buffer = new char[size + 1];
//	memset(buffer, 0, size + 1);
//	fseek(f, 0, SEEK_SET);
//	fread(buffer, size, 1, f);
//	fclose(f);
//
//	char * exp = ",";
//	char * token = strtok(buffer, exp);
//	int index = 0;
//
//	while (token)
//	{
//		data[index++] = strtof(token, nullptr);
//		token = strtok(NULL, exp);
//	}
//	*count = index;
//	delete[] buffer;
//	return 0;
//}
//
//int ReadDataFromFile_int(const char * file, int * data, int * count)
//{
//	FILE * f = fopen(file, "r");
//	fseek(f, 0, SEEK_END);
//	long size = ftell(f);
//	char * buffer = new char[size + 1];
//	memset(buffer, 0, size + 1);
//	fseek(f, 0, SEEK_SET);
//	fread(buffer, size, 1, f);
//	fclose(f);
//
//	char * exp = ",";
//	char * token = strtok(buffer, exp);
//	int index = 0;
//
//	while (token)
//	{
//		data[index++] = strtol(token, nullptr, 10);
//		token = strtok(NULL, exp);
//	}
//	*count = index;
//	delete[] buffer;
//	return 0;
//}
//////////////////////////////////////////////////////////////////////////
// end file(s)
//////////////////////////////////////////////////////////////////////////

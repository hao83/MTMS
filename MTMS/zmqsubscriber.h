#ifndef _ZMQSUBSCRIBER_H
#define _ZMQSUBSCRIBER_H

#include <stdio.h>
#include <string>
#include <thread>
#include "zmq.h"

//typedef void * (*ReplayCallBack)(void * buf,long len,void * context);

class ZmqSubscriber
{
public:
    ZmqSubscriber(void * context);
    virtual ~ZmqSubscriber();

public:
    int Connect(const char * address);
	int Connect(const char* szSvrIp, int port);
    int Close();

    //void Start();
    //int SetFilter(const char * filter);
    //int SetCallBack(ReplayCallBack cb,void * ctx);
	//std::string GetIp() const { return _ip; }
	//int GetPort() const { return _port; }

	int recv_n(char * buffer, int size);

private:
	// int resv(char *data, int length);

private:
	//void* OnSubscriberData(void *pdata, long len);

private:
    //static void * threadEntry(void * arg);
    //bool m_bMonitorSubscrib;
    void * m_context;
    void * m_socket;

	//std::thread* m_thread;
	//int m_timeout;

    //ReplayCallBack m_pCallBack;
    //void * m_pCBcontext;
    //std::string m_strFilter;

	//std::string _ip;
	//int _port;
};

#endif /* _ZMQSUBSCRIBER_H */

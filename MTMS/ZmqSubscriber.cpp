#include "ZmqSubscriber.h"
#include <iostream>

#include <vector>
#define CLI_CR_RED          "\033[31m"
#define CLI_CR_YELLOW       "\033[33m"
#define CLI_CR_CLOSE        "\033[0m"

ZmqSubscriber::ZmqSubscriber(void * context)
	:m_context(context)
{
    //m_pCallBack = NULL;
    //m_pCBcontext = NULL;

    //m_bMonitorSubscrib =true;

    //m_context = NULL;
    m_socket = NULL;

	//_ip = "0.0.0.0";
	//_port = -9999;
}

ZmqSubscriber::~ZmqSubscriber()
{
    //if (m_bMonitorSubscrib)
    //    Close();
	if (m_socket)
	{
		Close();
	}
}

int ZmqSubscriber::Close()
{
    //m_bMonitorSubscrib = false;
    //m_thread->join();
    //delete m_thread;

	int l = 0;
	zmq_setsockopt(m_socket, ZMQ_LINGER, &l, sizeof(l));//fix destroy hungs
	zmq_close(m_socket);

	//zmq_term(m_context);
	//zmq_ctx_destroy(m_context);

    return 0;
}

int ZmqSubscriber::Connect(const char*  szSvrIp, int port)
{
	char address[64];
	sprintf(address, "tcp://%s:%d", szSvrIp, port);
	return Connect(address);
}

int ZmqSubscriber::Connect(const char *address)
{
    //m_context = zmq_ctx_new();
    //if (!m_context) {
    //    std::cerr<<CLI_CR_RED<<"[Subscriber] "<< "Subscriber::failed to create context! with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
    //    return -1;
    //}

    m_socket = zmq_socket(m_context, ZMQ_SUB);


    if (!m_socket) {
        std::cerr<<CLI_CR_RED<<"[Subscriber] " << "Subscriber::failed to create subscriber socket! with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
        return -2;
    }

	const std::string m_strFilter = std::string("101");
    int ret = zmq_setsockopt(m_socket, ZMQ_SUBSCRIBE, m_strFilter.c_str(), m_strFilter.length());
    if (ret !=0) {
        std::cerr<<CLI_CR_RED<<"[Subscriber] " << "Subscriber::failed to set subscriber filter! with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
        return -3;
    }

    ret = zmq_connect(m_socket, address);
    if (ret < 0)
	{
        std::cerr<<CLI_CR_RED<<"[Subscriber] " << "Subscriber::Subscriber socket failed to connect port number :"<<address<<" with return value :"<<ret<<"with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
        return ret;
    }
	else
	{
		std::cout << "[Subscriber] " << "Subscriber::Connect to server,bind with address :" << address << std::endl;
	}

	int m_timeout = 10000;
	zmq_setsockopt(m_socket, ZMQ_RCVTIMEO, &m_timeout, sizeof(m_timeout));

	//std::string str = address;
	//int pos = str.find("tcp://");
	//if (pos >= 0)
	//{
	//	int slen = strlen("tcp://");
	//	str = str.substr(pos + slen, str.size() - slen);
	//	pos = str.find(":");
	//	if (pos >= 0)
	//	{
	//		_ip = str.substr(0, pos);
	//		std::string sport = str.substr(pos + 1, str.size() - pos - 1);
	//		_port = atoi(sport.c_str());
	//	}
	//}

    return 0;
}

//void ZmqSubscriber::Start()
//{
//	m_bMonitorSubscrib = true;
//	m_thread = new std::thread(ZmqSubscriber::threadEntry, this);
//}
//
//int ZmqSubscriber::SetCallBack(ReplayCallBack cb, void *ctx)
//{
//    m_pCallBack = cb;
//    m_pCBcontext = ctx;
//    return 0;
//}
//
//int ZmqSubscriber::SetFilter(const char *filter)
//{
//    m_strFilter = filter;
//    return 0;
//}

//int ZmqSubscriber::resv(char *data, int length){
//	char *buf = new char[1024];
//	memset(buf,0,1024);
//	DWORD gotLen = zmq_recv(m_socket, buf, 1024, 0);
//	memcpy(data, buf, 1024);
//	length = strlen(data);
//	return 0;
//}

int ZmqSubscriber::recv_n(char * buffer, int size)
{
	return zmq_recv(m_socket, buffer, size, 0);
}


//void * ZmqSubscriber::OnSubscriberData(void *pdata, long len) 
//{
//    if (len > 0)
//    {
//        if (m_pCallBack)
//        {  //defaule call cb function.
//            const char * pret = (const char *)m_pCallBack(pdata,len,m_pCBcontext);
//			std::cout << "SubData" << std::endl;
//            return (void *)pret;
//        }
//    }
//    else
//    {
//        //dump out
//        std::cout<<"[Subscriber] Get Data :"<<"error!"<<std::endl;
//    }
//    return NULL;
//}
//
//void * ZmqSubscriber::threadEntry(void *arg)
//{
//	ZmqSubscriber * pThis = (ZmqSubscriber *)arg;
//	while (pThis->m_bMonitorSubscrib)
//	{
//		zmq_pollitem_t items[1];
//		items[0].socket = pThis->m_socket;
//		items[0].events = ZMQ_POLLIN;
//
//		int rc = zmq_poll(items, ZMQ_POLLIN, 500);
//		if (rc <= 0)
//		{
//			continue;
//		}
//
//		std::vector<unsigned char> data_buffer;
//		data_buffer.clear();
//
//		zmq_msg_t msg;
//		zmq_msg_init(&msg);
//		zmq_msg_recv(&msg, pThis->m_socket, ZMQ_NOBLOCK);
//		void * pbuffer = zmq_msg_data(&msg);
//		size_t len = zmq_msg_size(&msg);
//		if (len <= 0) {
//			zmq_msg_close(&msg);
//			continue;
//		}
//
//		for (int i = 0; i<len; i++) {
//			data_buffer.push_back(((unsigned char *)pbuffer)[i]);
//		}
//		zmq_msg_close(&msg);
//
//		while (true)
//		{
//			//get more data in a frame
//			long more;
//			size_t more_len = sizeof(more);
//			int ret = zmq_getsockopt(pThis->m_socket, ZMQ_RCVMORE, &more, &more_len);
//			if (ret < 0)
//			{
//				std::cout << "Get More data failed!" << zmq_strerror(zmq_errno()) << std::endl;
//				return NULL;
//			}
//			if (!more)
//			{    //no more data
//				data_buffer.push_back(0);   //append 0 char
//				break;
//			}
//
//			zmq_msg_t part;
//			zmq_msg_init(&part);
//			zmq_msg_recv(&part, pThis->m_socket, ZMQ_NOBLOCK);
//			void * pbuffer = zmq_msg_data(&part);
//			size_t len = zmq_msg_size(&part);
//
//			data_buffer.push_back('!');//seperator
//			data_buffer.push_back('@');
//			data_buffer.push_back('#');
//			for (int i = 0; i<len; i++) 
//			{
//				data_buffer.push_back(((unsigned char *)pbuffer)[i]);
//			}
//			zmq_msg_close(&part);
//		}
//
//		pThis->OnSubscriberData(&data_buffer[0], data_buffer.size());
//	}
//	return NULL;
//}
 
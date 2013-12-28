#ifndef _RISKEMULIBRARY_NETWORK_SERVICECONTAINER_H_
#define _RISKEMULIBRARY_NETWORK_SERVICECONTAINER_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

struct ServiceContainer
{
	ServiceContainer()
	{
		this->work = new io_service::work(this->ioservice);
        this->thread = boost::thread(boost::bind(&boost::asio::io_service::run, &this->ioservice));
	}

	~ServiceContainer()
	{
		delete this->work; this->work = NULL;
		this->ioservice.stop();
		if(this->thread.joinable()) this->thread.join();
	}

	io_service ioservice;
    io_service::work *work;
	boost::thread thread;
};

typedef boost::shared_ptr<ServiceContainer> ServiceContainer_Ptr;

#endif //_RISKEMULIBRARY_NETWORK_SERVICECONTAINER_H_

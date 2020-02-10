#include <iostream>
#include <iomanip>
#include <chrono>
#include <functional>
#include "boost_warns.h"
#include <boost/asio.hpp>
#pragma warning(pop)

namespace asio = boost::asio;
namespace ph = std::placeholders;


void the_sync_way(asio::io_context& ioc) {
	std::cout << "Waiting 3 seconds synchronously ...";
	asio::steady_timer timer{ ioc, std::chrono::seconds{3} };
	timer.wait();
	std::cout << " done" << std::endl << std::endl;
}


class MyContext : public std::enable_shared_from_this<MyContext>
{
private:
	int m_count;
	asio::io_context& m_ioc;
	asio::steady_timer m_timer;

public:
	MyContext(asio::io_context& ioc)
		: m_count{ 5 }
		, m_ioc{ ioc }
		, m_timer{ ioc }
	{}

	void tick(const boost::system::error_code& ec) {
		if (--m_count > 0) {
			std::cout << ".";
			m_timer.expires_at(m_timer.expiry() + std::chrono::seconds(1));
			m_timer.async_wait(std::bind(&MyContext::tick, this->shared_from_this(), ph::_1));
		}
		else {
			std::cout << ". done" << std::endl;
		}
	}

	~MyContext() {
		std::cout << "MyContext is destroyed" << std::endl << std::endl;
	}

void start() {
		std::cout << "Wait 5 seconds asynchronousely ";
		m_timer.expires_from_now(std::chrono::seconds(1));
		m_timer.async_wait(
			std::bind(&MyContext::tick, this->shared_from_this(), ph::_1));
	}
};

void the_async_way(asio::io_context& ioc)
{
	auto pctx = std::make_shared<MyContext>(ioc);
	pctx->start();
}


int main() {
	boost::asio::io_context ioc;

	// The synchronous way
	the_sync_way(ioc);

	// The asynchronous way
	the_async_way(ioc);
		
	ioc.run();
	return 0;
}

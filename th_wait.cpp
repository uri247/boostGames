#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include "boost_warns.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#pragma warning( pop )



int main()
{
    boost::asio::io_service ioservice;
    std::shared_ptr<boost::asio::steady_timer> timers[10];

    for( int i=0; i<10; ++i) {
        auto t = std::make_shared<boost::asio::steady_timer>(ioservice, std::chrono::seconds{3});
        t->async_wait([i](const boost::system::error_code &e) {
            for(int b=0; b<1000000; ++b)
                ;
			auto th_id = std::this_thread::get_id();
            std::cout << "timer " << i << " on thread " << th_id << std::endl;
        });
        timers[i] = t;
    }

    std::thread th1{[&ioservice]() {ioservice.run(); }};
    std::thread th2{[&ioservice]() {ioservice.run(); }};

    th1.join();
    th2.join();
}
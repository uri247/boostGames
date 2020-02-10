#include <chrono>
#include <iostream>
#include <iomanip>
#include "boost_warns.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#pragma warning( pop )

using namespace boost::asio;

int main()
{
    io_service ioservice;

    steady_timer t4{ioservice, std::chrono::seconds{4}};
    t4.async_wait( [](const boost::system::error_code& ec) {
        std::cout << "4 secs" << std::endl;
    });

    steady_timer t3{ioservice, std::chrono::seconds{3}};
    t3.async_wait( [](const boost::system::error_code& ec) {
        std::cout << "3 secs" << std::endl;
    });

    ioservice.run();

}


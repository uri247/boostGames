#include <array>
#include <string>
#include <iostream>
#include <functional>
#include "boost_warns.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#pragma warning( pop )

using namespace boost::asio::ip;
using namespace boost::asio;
using error_code = boost::system::error_code;
using namespace std::placeholders;


class Program
{
private:
    io_service m_iocontext;
    tcp::resolver m_resolv;
    tcp::socket m_tcp_socket;
    std::array<char, 4096> m_bytes;
    std::function<void(const error_code&, const tcp::resolver::iterator&)> m_resolveHandler;
    std::function<void(const error_code&)> m_connectHandler;
    std::function<void(const error_code&, size_t)> m_readHandler;


public:
    Program()
        :m_iocontext{}
        ,m_resolv{m_iocontext}
        ,m_tcp_socket{m_iocontext}
        ,m_bytes{}
    {
        // prepare the found function head of time. not necessary but make the code a little bit more readable
        m_resolveHandler = std::bind(&Program::resolve_handler, this, _1, _2);
        m_readHandler = std::bind(&Program::read_handler, this, _1, _2);
        m_connectHandler = std::bind(&Program::connect_handler, this, _1);
    }

    int main() {
        tcp::resolver::query q{"www.google.com", "80"};
        m_resolv.async_resolve(q, m_resolveHandler);
        m_iocontext.run();
        return 0;
    }

    void resolve_handler(const error_code& ec, const tcp::resolver::iterator& it) {
        if( !ec ) {
            std::cout << "resolved!" << std::endl;
            m_tcp_socket.async_connect(*it, m_connectHandler);
        }
    }

    void connect_handler(const error_code& ec) {
        if( !ec ) {
            std::string r =
                "GET / HTTP/1.1\r\n"
                "Host: www.google.com\r\n"
                "\r\n";
            write(m_tcp_socket, buffer(r));
            m_tcp_socket.async_read_some(buffer(m_bytes), m_readHandler);
        }
    }

    void read_handler(const error_code &ec, size_t bytes_transferred) {
        if( !ec ) {
            std::cout.write(m_bytes.data(), bytes_transferred);
            m_tcp_socket.async_read_some(buffer(m_bytes), m_readHandler);
        }
        else {
            std::cout << "this is the end";
        }
    }
};



int main() {
    Program program;
    return program.main();
}

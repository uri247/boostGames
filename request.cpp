#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "boost_warns.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#pragma warning( pop )


namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ip = asio::ip;
namespace ph = std::placeholders;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


// Report a failure
void
fail(beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}


class Session : public std::enable_shared_from_this<Session>
{
	tcp::resolver m_resolver;
	beast::tcp_stream m_stream;
	beast::flat_buffer m_buffer; // (Must persist between reads)
	http::request<http::empty_body> m_req;
	http::response<http::string_body> m_resp;

public:
	explicit Session(asio::io_context& ioc)
		: m_resolver{ asio::make_strand(ioc) }
		, m_stream{ asio::make_strand(ioc) }
	{
	}

	// Start the asynchronous operation
	void run(char const* host, char const* port, char const* target, int version)
	{
		// Set up an HTTP GET request message
		m_req.version(version);
		m_req.method(http::verb::get);
		m_req.target(target);
		m_req.set(http::field::host, host);
		m_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Look up the domain name
		m_resolver.async_resolve(host, port, std::bind(&Session::on_resolve, shared_from_this(), ph::_1, ph::_2));
	}

	void on_resolve(beast::error_code ec, tcp::resolver::results_type results)
	{
		if (ec)
			return fail(ec, "resolve");

		// Set a timeout on the operation
		m_stream.expires_after(std::chrono::seconds(30));

		// Make the connection on the IP address we get from a lookup
		m_stream.async_connect(results, std::bind(&Session::on_connect, shared_from_this(), ph::_1, ph::_2));
	}

	void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
	{
		if (ec)
			return fail(ec, "connect");

		// Set a timeout on the operation
		m_stream.expires_after(std::chrono::seconds(30));

		// Send the HTTP request to the remote host
		http::async_write(m_stream, m_req, std::bind(&Session::on_write, shared_from_this(), ph::_1, ph::_2));
	}

	void on_write(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		if (ec)
			return fail(ec, "write");

		// Receive the HTTP response
		http::async_read(m_stream, m_buffer, m_resp, std::bind(&Session::on_read, shared_from_this(), ph::_1, ph::_2));
	}

	void on_read(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		if (ec)
			return fail(ec, "read");

		// Write the message to standard out
		std::cout << m_resp << std::endl;

		// Gracefully close the socket
		m_stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		// not_connected happens sometimes so don't bother reporting it.
		if (ec && ec != beast::errc::not_connected)
			return fail(ec, "shutdown");

		// If we get here then the connection is closed gracefully
	}
};


class Program {
private:
	asio::io_context m_ioc;

public:
	Program() :m_ioc{}
	{}

	int Program::main() {
		const std::string host = "www.google.com";
		const std::string port = "http";
		const std::string target = "/";
		const int version = 11;

		auto session = std::make_shared<Session>(m_ioc);
		session->run(host.c_str(), port.c_str(), target.c_str(), version);

		m_ioc.run();

		return EXIT_SUCCESS;
	}
};


int main(int argc, char** argv)
{
	Program program{};
	program.main();
}

#include <iostream>
#include <fstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;

void init()
{
    // construct a sink
    using text_sink = sinks::synchronous_sink< sinks::text_ostream_backend >;
    auto sink = boost::make_shared<text_sink>();

    // create the backend
    auto backend = boost::make_shared<std::ofstream>("sample.log");

    // Add a stream to write log
    sink->locked_backend()->add_stream(backend);

    logging::core::get()->add_sink(sink);
    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
}

int main(int, char* [])
{
    init();

    BOOST_LOG_TRIVIAL(trace) << "A trace message";
    BOOST_LOG_TRIVIAL(debug) << "A debug message";
    BOOST_LOG_TRIVIAL(info) << "A info message";
    BOOST_LOG_TRIVIAL(warning) << "A warning message";
    BOOST_LOG_TRIVIAL(error) << "A error message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal message";

}
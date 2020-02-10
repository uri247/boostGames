#include <iostream>
#include <iomanip>
#include <string>
#include "boost_warns.h"
#include <boost/exception/diagnostic_information.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#pragma warning( pop )


int main()
{
    // My birthday
    boost::gregorian::date d{1965, 6, 11};
    std::cout <<
        "year: " << d.year() << std::endl <<
        "month: " << d.month() << std::endl <<
        "day: " << d.day() << std::endl <<
        "day of week: " << d.day_of_week() << std::endl <<
        "end of month: " << d.end_of_month() << std::endl;

    // Trying bad date
    try {
        boost::gregorian::date feb29{2019, 2, 29};
        std::cout << feb29.day_of_year();
    }
//    catch (boost::gregorian::bad_day_of_month& e) {
    catch (boost::exception& e) {
        std::cout << boost::diagnostic_information(e, true) << "\n";
    }
}
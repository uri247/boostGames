#include <iostream>
#include <iomanip>
#include "boost_warns.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#pragma warning( pop )


int main()
{
    boost::scoped_ptr<int> p{new int{1}};
    std::cout << *p << std::endl;
    p.reset(new int{2});
    std::cout << *p << std::endl;
    p.reset();
    std::cout << std::boolalpha << static_cast<bool>(p) << std::endl;
}

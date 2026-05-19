#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

namespace utils {

std::ostream& bold_on(std::ostream& os)  { return os << "\e[1m"; }
std::ostream& bold_off(std::ostream& os) { return os << "\e[0m"; }

} // namespace utils

#endif // UTILS_HPP
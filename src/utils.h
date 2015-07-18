// Copyright [2015] <Chafic Najjar>

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <iomanip>
#include <sstream>
#include <string>

namespace utils {
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}
}

#endif  // SRC_UTILS_H_

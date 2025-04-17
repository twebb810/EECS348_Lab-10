#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>

bool isValidDouble(const std::string &expression);   // true if (+|-)A(.B) format

double parse_number(const std::string &expression);

std::string add(const std::string &lhs, const std::string &rhs);

#endif // __CALCULATOR_HPP

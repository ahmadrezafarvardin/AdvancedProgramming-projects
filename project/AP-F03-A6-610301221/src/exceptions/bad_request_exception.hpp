#ifndef BAD_REQUEST_EXCEPTION_HPP_INCLUDE
#define BAD_REQUEST_EXCEPTION_HPP_INCLUDE

#include <string>

#include "exception.hpp"

class BadRequestException : Exception {
public:
  BadRequestException() : Exception(){};
  BadRequestException(std::string message) : Exception(message){};
  static std::string exceptionMessage() { return "Bad Request"; }
};

#endif // BAD_REQUEST_EXCEPTION_HPP_INCLUDE

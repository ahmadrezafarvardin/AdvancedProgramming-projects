#ifndef NOT_FOUND_EXCEPTION_HPP_INCLUDE
#define NOT_FOUND_EXCEPTION_HPP_INCLUDE

#include <string>

#include "exception.hpp"

class NotFoundException : Exception {
public:
  NotFoundException() : Exception(){};
  NotFoundException(std::string message) : Exception(message){};
  static std::string exceptionMessage() { return "Not Found"; }
};

#endif // NOT_FOUND_EXCEPTION_HPP_INCLUDE

#ifndef EMPTY_EXCEPTION_HPP_INCLUDE
#define EMPTY_EXCEPTION_HPP_INCLUDE

#include <string>

#include "exception.hpp"

class EmptyException : Exception {
public:
  EmptyException() : Exception(){};
  EmptyException(std::string message) : Exception(message){};
  static std::string exceptionMessage() { return "Empty"; }
};

#endif // EMPTY_EXCEPTION_HPP_INCLUDE

#ifndef PERMISSION_DENIED_EXCEPTION_HPP_INCLUDE
#define PERMISSION_DENIED_EXCEPTION_HPP_INCLUDE

#include <string>

#include "exception.hpp"

class PermissionDeniedException : Exception {
public:
  PermissionDeniedException() : Exception(){};
  PermissionDeniedException(std::string message) : Exception(message){};
  static std::string exceptionMessage() { return "Permission Denied"; }
};

#endif // PERMISSION_DENIED_EXCEPTION_HPP_INCLUDE

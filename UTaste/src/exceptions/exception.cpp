#include "exception.hpp"

#include <string>

Exception::Exception() { message_ = ""; }

Exception::Exception(std::string message) { message_ = message; }

std::string Exception::getMessage() { return message_; }

#ifndef EXCEPTION_HPP_INCLUDE
#define EXCEPTION_HPP_INCLUDE

#include <string>
using namespace std;

class Exception {
public:
  Exception();
  Exception(std::string message);
  std::string getMessage();

protected:
  std::string message_;
};

#endif // EXCEPTION_HPP_INCLUDE

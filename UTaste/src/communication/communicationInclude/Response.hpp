#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "constantsCommunication.hpp"
#include <memory>
#include <string>

class Response {
public:
  virtual ~Response() = default;
  virtual string toString() { return message_; }
  virtual string getType() { return ""; }

  const string &getMessage() const { return message_; }
  Response(const string &message) : message_(message) {}

protected:
  string message_;
};

class OkResponse : public Response {
public:
  OkResponse(const string &message = OK) : Response(message) {}
  string getType() { return OK; }
};

class BadRequestResponse : public Response {
public:
  BadRequestResponse(const string &message = BAD_REQUEST) : Response(message) {}
  string getType() { return BAD_REQUEST; }
};

class EmptyResponse : public Response {
public:
  EmptyResponse(const string &message = EMPTY) : Response(message) {}
  string getType() { return EMPTY; }
};

class NotFoundResponse : public Response {
public:
  NotFoundResponse(const string &message = NOT_FOUND) : Response(message) {}
  string getType() { return NOT_FOUND; }
};

class PermissionDeniedResponse : public Response {
public:
  PermissionDeniedResponse(const string &message = PERMISSION_DEINIED)
      : Response(message) {}
  string getType() { return PERMISSION_DEINIED; }
};
#endif
#ifndef COMMANDFACTORY_HPP
#define COMMANDFACTORY_HPP
#include "CommandImpl.hpp"
#include "constantsCommunication.hpp"
#include <string>

class CommandFactory {
public:
  static unique_ptr<Command> createCmd(const string &commandString);

private:
  static vector<string> tokenizeCommand(const string &commandString);
  static unique_ptr<Command> createGetCmd(const string &action,
                                          const vector<string> &params);
  static unique_ptr<Command> createPostCmd(const string &action,
                                           const vector<string> &params);
  static unique_ptr<Command> createPutCmd(const string &action,
                                          const vector<string> &params);
  static unique_ptr<Command> createDeleteCmd(const string &action,
                                             const vector<string> &params);
  static bool isFullyQuotedToken(const string &token) {
    return token.front() == '"' && token.back() == '"';
  }
  static bool isQuoteStartToken(const string &token) {
    return token.front() == '"' && token.back() != '"';
  }
  static bool isQuoteEndToken(const string &token) {
    return token.front() != '"' && token.back() == '"';
  }
  static string stripQuotes(const string &token) {
    if (isFullyQuotedToken(token)) {
      return token.substr(1, token.length() - 2);
    }
    if (isQuoteStartToken(token)) {
      return token.substr(1);
    }
    if (isQuoteEndToken(token)) {
      return token.substr(0, token.length() - 1);
    }
    return token;
  }
};
#endif
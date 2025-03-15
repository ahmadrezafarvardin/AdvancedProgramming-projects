#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "CommandFactory.hpp"
#include <iostream>
using namespace std;

class CommandHandler {
public:
  CommandHandler(ReservationSystem &system) : system_(system) {}
  Response handleCommand(const string &commandString);

private:
  ReservationSystem &system_;
};
#endif
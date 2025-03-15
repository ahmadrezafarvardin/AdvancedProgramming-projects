#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "../../systemCore/sysCoreInclude/ReservationSystem.hpp"
#include "CommandHandler.hpp"
#include "constantsCommunication.hpp"
#include "httplib.h"
#include <string>
#include <unordered_map>

using namespace std;

class WebServer {
public:
  WebServer(ReservationSystem &system);
  void start(int port);

private:
  void setupRoutes();
  void setupAuthRoutes();
  void setupReserveRoutes();
  void setupBudgetRoutes();
  void setupRestaurantDetailsRoutes();
  void setupGetReservationRoutes();
  void setupHomeRoute();
  void setupAuthCheckRoute();

  string createSession(const string &username);
  string generateSessionId();
  string extractSessionId(const string &cookie);
  bool validateSession(const string &sessionId);
  string getUsernameFromSession(const string &sessionId);

  httplib::Server server_;
  ReservationSystem &system_;
  CommandHandler handler_;
  unordered_map<string, string> sessions_; // sessionId -> username
};
#endif
#include "communicationInclude/WebServer.hpp"
#include <random>
#include <sstream>

WebServer::WebServer(ReservationSystem &system)
    : system_(system), handler_(system) {
  setupRoutes();
}

void WebServer::start(int port) {
  server_.set_mount_point("/", "./frontend");
  cout << "Starting server on port " << port << endl;
  server_.listen("localhost", port);
}

void WebServer::setupRoutes() {
  setupAuthRoutes();
  setupReserveRoutes();
  setupBudgetRoutes();
  setupGetReservationRoutes();
  setupRestaurantDetailsRoutes();
  setupAuthCheckRoute();
}

void WebServer::setupAuthCheckRoute() {
  server_.Get("/api/check-auth",
              [this](const httplib::Request &req, httplib::Response &res) {
                string cookie = req.get_header_value("Cookie");
                string sessionId = extractSessionId(cookie);

                if (validateSession(sessionId)) {
                  res.status = 200;
                  res.set_content("Authenticated", "text/plain");
                } else {
                  res.status = 401;
                  res.set_content("Unauthenticated", "text/plain");
                }
              });
}

void WebServer::setupHomeRoute() {
  server_.Get("/pages/home.html",
              [this](const httplib::Request &req, httplib::Response &res) {
                string cookie = req.get_header_value("Cookie");
                string sessionId = extractSessionId(cookie);

                if (validateSession(sessionId)) {
                  ifstream file("./frontend/pages/home.html");
                  if (file) {
                    stringstream buffer;
                    buffer << file.rdbuf();
                    res.set_content(buffer.str(), "text/html");
                  } else {
                    res.status = 404;
                    res.set_content("Home page not found", "text/plain");
                  }
                } else {
                  res.status = 401;
                  res.set_content("Unauthorized", "text/plain");
                }
              });
}

void WebServer::setupAuthRoutes() {
  server_.Post("/api/signup",
               [this](const httplib::Request &req, httplib::Response &res) {
                 try {
                   string username = req.get_param_value("username");
                   string password = req.get_param_value("password");

                   string command = "POST signup ? username \"" + username +
                                    "\" password \"" + password + "\"";
                   Response response = handler_.handleCommand(command);

                   if (response.getMessage() == "OK") {
                     res.status = 200;
                     res.set_content("Signup successful", "text/plain");
                   } else {
                     res.status = 400;
                     res.set_content(response.toString(), "text/plain");
                   }

                 } catch (const exception &e) {
                   res.status = 500;
                   res.set_content("Internal server error", "text/plain");
                 }
               });

  server_.Post("/api/login", [this](const httplib::Request &req, httplib::Response &res) {
  try {
    string username = req.get_param_value("username");
    string password = req.get_param_value("password");

    string command = "POST login ? username \"" + username + "\" password \"" + password + "\"";
    Response response = handler_.handleCommand(command);

    if (response.getMessage() == "OK") {
      string sessionId = createSession(username);

      res.set_header("Set-Cookie", "sessionId=" + sessionId + "; Path=/; HttpOnly; SameSite=Strict");
      res.status = 200;
      res.set_content("Login successful", "text/plain");
    } else {
      res.status = 401;
      res.set_content(response.toString(), "text/plain");
    }

  } catch (const exception &e) {
    res.status = 500;
    res.set_content("Internal server error", "text/plain");
  }
});

  server_.Post("/api/logout", [this](const httplib::Request &req,
                                     httplib::Response &res) {
    try {
      string command = "POST logout ?";
      Response response = handler_.handleCommand(command);

      string cookie = req.get_header_value("Cookie");
      string sessionId = extractSessionId(cookie);
      if (!sessionId.empty()) {
        sessions_.erase(sessionId);
      }

      res.set_header("Set-Cookie",
                     "sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 "
                     "GMT; HttpOnly; SameSite=Strict");
      res.status = 200;
      res.set_content("Logout successful", "text/plain");
    } catch (const exception &e) {
      res.status = 500;
      res.set_content("Logout failed", "text/plain");
    }
  });
}

std::string urlDecode(const std::string &encoded) {
  std::string result;
  char ch;
  int i, ii, len = encoded.length();

  for (i = 0; i < len; i++) {
    if (encoded[i] != '%') {
      if (encoded[i] == '+')
        result += ' ';
      else
        result += encoded[i];
    } else {
      sscanf(encoded.substr(i + 1, 2).c_str(), "%x", &ii);
      ch = static_cast<char>(ii);
      result += ch;
      i = i + 2;
    }
  }
  return result;
}

void WebServer::setupReserveRoutes() {
  server_.Post("/api/reserve", [this](const httplib::Request &req,
                                      httplib::Response &res) {
    try {
      string cookie = req.get_header_value("Cookie");
      string sessionId = extractSessionId(cookie);

      if (!validateSession(sessionId)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
      }

      string restaurantName = req.get_param_value("restaurant_name");
      string tableId = req.get_param_value("table_id");
      string startTime = req.get_param_value("start_time");
      string endTime = req.get_param_value("end_time");
      string foodsStr = req.get_param_value("foods");

      // URL decode the foods string
      foodsStr = urlDecode(foodsStr);

      string command = "POST reserve ? restaurant_name \"" + restaurantName +
                       "\" table_id \"" + tableId + "\" start_time \"" +
                       startTime + "\" end_time \"" + endTime + "\"";

      if (!foodsStr.empty()) {
        command += " foods \"" + foodsStr + "\"";
      }

      Response response = handler_.handleCommand(command);

      if (response.getMessage() == BAD_REQUEST ||
          response.getMessage() == PERMISSION_DEINIED ||
          response.getMessage() == NOT_FOUND ||
          response.getMessage() == EMPTY) {
        res.status = 400;
        res.set_content(response.toString(), "text/plain");
      } else {
        res.status = 200;
        res.set_content(response.toString(), "text/plain");
      }

    } catch (const exception &e) {
      res.status = 500;
      res.set_content("Internal server error", "text/plain");
    }
  });
}

void WebServer::setupBudgetRoutes() {
  server_.Post("/api/increase_budget", [this](const httplib::Request &req,
                                              httplib::Response &res) {
    try {
      string cookie = req.get_header_value("Cookie");
      string sessionId = extractSessionId(cookie);

      if (!validateSession(sessionId)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
      }

      string amount = req.get_param_value("amount");
      string command = "POST increase_budget ? amount \"" + amount + "\"";
      Response response = handler_.handleCommand(command);

      if (response.getMessage() == BAD_REQUEST ||
          response.getMessage() == PERMISSION_DEINIED ||
          response.getMessage() == NOT_FOUND ||
          response.getMessage() == EMPTY) {
        res.status = 400;
        res.set_content(response.toString(), "text/plain");
      } else {
        res.status = 200;
        res.set_content("Budget increased successfully", "text/plain");
      }

    } catch (const exception &e) {
      res.status = 500;
      res.set_content("Internal server error", "text/plain");
    }
  });
}

void WebServer::setupRestaurantDetailsRoutes() {
  server_.Get("/api/get/restaurant_detail", [this](const httplib::Request &req,
                                                   httplib::Response &res) {
    try {
      string restaurantName = req.get_param_value("restaurant_name");
      string command =
          "GET restaurant_detail ? restaurant_name \"" + restaurantName + "\"";
      Response response = handler_.handleCommand(command);

      if (!response.getMessage().empty()) {
        res.status = 200;
        res.set_content(response.toString(), "text/plain");
      } else {
        res.status = 400;
        res.set_content("Error: " + response.toString(), "text/plain");
      }

    } catch (const exception &e) {
      res.status = 500;
      res.set_content("Failed to fetch restaurant details", "text/plain");
    }
  });
}

void WebServer::setupGetReservationRoutes() {
  server_.Get("/api/reserves", [this](const httplib::Request &req,
                                      httplib::Response &res) {
    try {
      string cookie = req.get_header_value("Cookie");
      string sessionId = extractSessionId(cookie);

      if (!validateSession(sessionId)) {
        res.status = 401;
        res.set_content("Please login first", "text/plain");
        return;
      }
      string restaurantName = req.get_param_value("restaurant_name");
      string reserveId = req.get_param_value("reserve_id");
      string command = "GET reserves ?";

      if (!restaurantName.empty() && !reserveId.empty()) {
        command += " restaurant_name \"" + restaurantName + "\" reserve_id \"" +
                   reserveId + "\"";
      } else if (!restaurantName.empty()) {
        command += " restaurant_name \"" + restaurantName + "\"";
      } else if (!reserveId.empty()) {
        command += " reserve_id \"" + reserveId + "\"";
      }

      Response response = handler_.handleCommand(command);

      if (response.getMessage() == BAD_REQUEST ||
          response.getMessage() == PERMISSION_DEINIED ||
          response.getMessage() == NOT_FOUND ||
          response.getMessage() == EMPTY) {
        res.status = 400;
        res.set_content(response.toString(), "text/plain");
      } else {
        res.status = 200;
        res.set_content(response.toString(), "text/plain");
      }

    } catch (const exception &e) {
      res.status = 500;
      res.set_content("Internal server error", "text/plain");
    }
  });
}

string WebServer::extractSessionId(const string &cookie) {
  size_t pos = cookie.find("sessionId=");
  if (pos != string::npos) {
    size_t end = cookie.find(";", pos);
    return cookie.substr(pos + 10, end - pos - 10);
  }
  return "";
}

string WebServer::generateSessionId() {
  static random_device rd;
  static mt19937 gen(rd());
  static uniform_int_distribution<> dis(0, 15);
  static const char *hex = "0123456789abcdef";

  string sessionId;
  sessionId.reserve(32);
  for (int i = 0; i < 32; ++i) {
    sessionId += hex[dis(gen)];
  }
  return sessionId;
}

string WebServer::createSession(const string &username) {
  string sessionId = generateSessionId();
  sessions_[sessionId] = username;
  return sessionId;
}

bool WebServer::validateSession(const string &sessionId) {
  auto it = sessions_.find(sessionId);
  if (it == sessions_.end()) {
    return false;
  }
  return true;
}

string WebServer::getUsernameFromSession(const string &sessionId) {
  auto it = sessions_.find(sessionId);
  if (it != sessions_.end()) {
    return it->second;
  }
  return "";
}
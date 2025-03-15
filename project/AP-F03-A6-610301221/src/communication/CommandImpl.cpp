#include "communicationInclude/CommandImpl.hpp"
#include "../systemCore/sysCoreInclude/ReservationSystem.hpp"
#include "communicationInclude/Command.hpp"

Response PostSignupCmd::execute(ReservationSystem &system) {
  unordered_map<string, string> userPsw;
  if (system.isLoggedIn()) {
    return PermissionDeniedResponse();
  }

  if (params_.size() % 2 != 0) {
    return BadRequestResponse();
  }

  for (size_t i = 0; i < params_.size(); i += 2) {
    userPsw[params_[i]] = params_[i + 1];
  }

  if (userPsw.find("username") == userPsw.end() ||
      userPsw.find("password") == userPsw.end()) {
    return BadRequestResponse();
  }

  string username = userPsw["username"];
  string password = userPsw["password"];

  return system.signup(username, password);
}

Response PostLoginCmd::execute(ReservationSystem &system) {
  if (params_.size() < 4) {
    return BadRequestResponse();
  }

  unordered_map<string, string> userPsw;

  for (size_t i = 0; i < params_.size(); i += 2) {
    if (i + 1 < params_.size()) {
      userPsw[params_[i]] = params_[i + 1];
    }
  }

  string username = userPsw["username"];
  string password = userPsw["password"];
  return system.login(username, password);
}

Response PostLogoutCmd::execute(ReservationSystem &system) {
  return system.logout();
}

Response GetDistrictsCmd::execute(ReservationSystem &system) {
  if (!params_.empty()) {
    if (params_.size() < 2) {
      return BadRequestResponse();
    }

    return system.getDistricts(params_[1]);
  }
  return system.getDistricts();
}

Response PutMyDistrictCmd::execute(ReservationSystem &system) {
  if (params_.size() < 2) {
    return BadRequestResponse();
  }
  return system.setUserDistrict(params_[1]);
}

Response GetRestaurantsCmd::execute(ReservationSystem &system) {
  if (params_.empty()) {
    return system.getRestaurants("");
  }

  if (params_.size() > 1 && params_[0] == FOOD_NAME) {
    return system.getRestaurants(params_[1]);
  }

  return BadRequestResponse();
}

Response GetRestaurantDetailCmd::execute(ReservationSystem &system) {
  if (params_.size() % 2 != 0 || params_.size() < 2) {
    return BadRequestResponse();
  }

  unordered_map<string, string> params;
  for (size_t i = 0; i < params_.size(); i += 2) {
    params[params_[i]] = params_[i + 1];
  }

  auto it = params.find("restaurant_name");
  if (it == params.end()) {
    return BadRequestResponse();
  }

  string restName = it->second;
  return system.getRestaurantDetail(restName);
}

Response PostReserveCmd::execute(ReservationSystem &system) {
  if (!system.isLoggedIn()) {
    return PermissionDeniedResponse();
  }

  if (params_.size() < 8) {
    return BadRequestResponse();
  }

  auto [reserveParams, foods] = parseParameters(params_);

  auto [success, params] = extractReservationParams(reserveParams);
  if (!success) {
    return BadRequestResponse();
  }

  auto [restaurantName, tableId, startTime, endTime] = params;

  if (reserveParams.find(RESTAURANT_NAME) == reserveParams.end() ||
      reserveParams[RESTAURANT_NAME].empty()) {
    return BadRequestResponse();
  }

  return system.reserve(restaurantName, tableId, startTime, endTime, foods);
}

Response
GetReservesCmd::handleGetReserves(ReservationSystem &system,
                                  const unordered_map<string, string> &params) {
  if (params.empty()) {
    return system.getReserves();
  }

  auto restIt = params.find(RESTAURANT_NAME);
  if (restIt == params.end()) {
    return BadRequestResponse();
  }

  string restaurantName = restIt->second;

  auto resIdIt = params.find(RESERVE_ID);
  if (resIdIt == params.end()) {
    return system.getReserves(restaurantName);
  }

  try {
    int reservationId = stoi(resIdIt->second);
    return system.getReserves(restaurantName, reservationId);
  } catch (const exception &e) {
    return BadRequestResponse();
  }
}
Response GetReservesCmd::execute(ReservationSystem &system) {
  if (!system.isLoggedIn()) {
    return PermissionDeniedResponse();
  }

  auto [params, _] = parseParameters(params_);
  return handleGetReserves(system, params);
}

Response DeleteReserveCmd::execute(ReservationSystem &system) {
  if (!system.isLoggedIn()) {
    return PermissionDeniedResponse();
  }

  auto [params, _] = parseParameters(params_);

  auto restIt = params.find(RESTAURANT_NAME);
  auto resIdIt = params.find(RESERVE_ID);

  if (restIt == params.end() || resIdIt == params.end()) {
    return BadRequestResponse();
  }

  string restaurantName = restIt->second;
  int reservationId;

  try {
    reservationId = stoi(resIdIt->second);
  } catch (const exception &e) {
    return BadRequestResponse();
  }

  return system.deleteReserve(restaurantName, reservationId);
}
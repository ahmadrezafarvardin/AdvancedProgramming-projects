#include "communicationInclude/CommandFactory.hpp"
#include "../systemCore/sysCoreInclude/ReservationSystem.hpp"
#include "communicationInclude/Command.hpp"

unique_ptr<Command> CommandFactory::createCmd(const string &input) {
  vector<string> tokens = tokenizeCommand(input);
  if (tokens.empty() || tokens.size() < 2) {
    throw BadRequestException(BadRequestException::exceptionMessage());
  }

  string method = tokens[0];
  string action = tokens[1];
  vector<string> params(tokens.begin() + QUESMARK_SKIPPER, tokens.end());
  try {
    if (method == GET) {
      return createGetCmd(action, params);
    } else if (method == POST) {
      return createPostCmd(action, params);
    } else if (method == PUT) {
      return createPutCmd(action, params);
    } else if (method == DELETE) {
      return createDeleteCmd(action, params);
    }
    throw BadRequestException(BadRequestException::exceptionMessage());
  } catch (const NotFoundException &e) {
    throw NotFoundException(NotFoundException::exceptionMessage());
  }
}

vector<string> CommandFactory::tokenizeCommand(const string &commandString) {
  vector<string> tokens;
  istringstream iss(commandString);
  string token;
  bool inQuotes = false;
  string currentToken;

  while (iss >> token) {
    if (isFullyQuotedToken(token)) {
      tokens.push_back(stripQuotes(token));
    } else if (isQuoteStartToken(token)) {
      inQuotes = true;
      currentToken = stripQuotes(token);
    } else if (isQuoteEndToken(token)) {
      inQuotes = false;
      currentToken += " " + stripQuotes(token);
      tokens.push_back(currentToken);
      currentToken.clear();
    } else if (inQuotes) {
      currentToken += " " + token;
    } else {
      tokens.push_back(token);
    }
  }

  return tokens;
}

unique_ptr<Command> CommandFactory::createGetCmd(const string &action,
                                                 const vector<string> &params) {
  if (action == DISTRICTS) {
    return make_unique<GetDistrictsCmd>(params);
  } else if (action == RESTAURANTS) {
    return make_unique<GetRestaurantsCmd>(params);
  } else if (action == RESTAURANT_DETAIL) {
    return make_unique<GetRestaurantDetailCmd>(params);
  } else if (action == RESERVES) {
    return make_unique<GetReservesCmd>(params);
  } else if (action == SHOW_BUDGET) {
    return make_unique<GetShowBudgetCmd>(params);
  }
  throw NotFoundException(NotFoundException::exceptionMessage());
}

unique_ptr<Command>
CommandFactory::createPostCmd(const string &action,
                              const vector<string> &params) {
  if (action == SIGNUP) {
    return make_unique<PostSignupCmd>(params);
  } else if (action == LOGIN) {
    return make_unique<PostLoginCmd>(params);
  } else if (action == LOGOUT) {
    return make_unique<PostLogoutCmd>(params);
  } else if (action == RESERVE) {
    return make_unique<PostReserveCmd>(params);
  } else if (action == INCREASE_BUDGET) {
    return make_unique<PostIncreaseBudgetCmd>(params);
  }
  throw NotFoundException(NotFoundException::exceptionMessage());
}

unique_ptr<Command> CommandFactory::createPutCmd(const string &action,
                                                 const vector<string> &params) {
  if (action == MY_DISTRICT) {
    return make_unique<PutMyDistrictCmd>(params);
  }
  throw NotFoundException(NotFoundException::exceptionMessage());
}

unique_ptr<Command>
CommandFactory::createDeleteCmd(const string &action,
                                const vector<string> &params) {
  if (action == RESERVE) {
    return make_unique<DeleteReserveCmd>(params);
  }
  throw NotFoundException(NotFoundException::exceptionMessage());
}
#ifndef COMMANDIMLP_HPP
#define COMMANDIMLP_HPP

#include "Command.hpp"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class GetCmd : public Command {
public:
  GetCmd(const vector<string> &params) : params_(params) {}
  Response execute(ReservationSystem &system) = 0;

protected:
  vector<string> params_;
};

class PostCmd : public Command {
public:
  PostCmd(const vector<string> &params) : params_(params) {}
  Response execute(ReservationSystem &system) = 0;

protected:
  vector<string> params_;
};

class PutCmd : public Command {
public:
  PutCmd(const vector<string> &params) : params_(params) {}
  Response execute(ReservationSystem &system) = 0;

protected:
  vector<string> params_;
};

class DeleteCmd : public Command {
public:
  DeleteCmd(const vector<string> &params) : params_(params) {}
  Response execute(ReservationSystem &system) = 0;

protected:
  vector<string> params_;
};

class GetDistrictsCmd : public GetCmd {
public:
  GetDistrictsCmd(const vector<string> &params) : GetCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class GetRestaurantDetailCmd : public GetCmd {
public:
  GetRestaurantDetailCmd(const vector<string> &params) : GetCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class GetRestaurantsCmd : public GetCmd {
public:
  GetRestaurantsCmd(const vector<string> &params) : GetCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class GetReservesCmd : public GetCmd {
public:
  GetReservesCmd(const vector<string> &params) : GetCmd(params) {}
  Response execute(ReservationSystem &system) override;

private:
  Response handleGetReserves(ReservationSystem &system,
                             const unordered_map<string, string> &params);
};

class GetShowBudgetCmd : public GetCmd {
public:
  GetShowBudgetCmd(const vector<string> &params) : GetCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class PostSignupCmd : public PostCmd {
public:
  PostSignupCmd(const vector<string> &params) : PostCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class PostLoginCmd : public PostCmd {
public:
  PostLoginCmd(const vector<string> &params) : PostCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class PostLogoutCmd : public PostCmd {
public:
  PostLogoutCmd(const vector<string> &params) : PostCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class PostReserveCmd : public PostCmd {
public:
  PostReserveCmd(const vector<string> &params) : PostCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class PostIncreaseBudgetCmd : public PostCmd {
public:
  PostIncreaseBudgetCmd(const vector<string> &params) : PostCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class PutMyDistrictCmd : public PutCmd {
public:
  PutMyDistrictCmd(const vector<string> &params) : PutCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

class DeleteReserveCmd : public DeleteCmd {
public:
  DeleteReserveCmd(const vector<string> &params) : DeleteCmd(params) {}
  Response execute(ReservationSystem &system) override;
};

#endif
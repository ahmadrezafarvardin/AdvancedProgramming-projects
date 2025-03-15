#ifndef User_HPP
#define User_HPP

#include "District.hpp"
#include "Reservation.hpp"
#include <algorithm>
#include <memory>

class User {
private:
  string uname_;
  string pw_;
  shared_ptr<District> currentDist_;
  vector<Reservation> resis_;

public:
  User(const string &username, const string &password)
      : uname_(username), pw_(password), currentDist_(nullptr) {}

  string getName() { return uname_; }
  string getPsw() { return pw_; }
  void setLocation(shared_ptr<District> district) { currentDist_ = district; }
  shared_ptr<District> getCurrentDistrict() const { return currentDist_; }

  void addReservation(const Reservation &res) { resis_.push_back(res); }
  const vector<Reservation> &getReservations() const { return resis_; }
  void removeReservation(const string &restaurantName, int reserveId) {
    auto it =
        find_if(resis_.begin(), resis_.end(), [&](const Reservation &res) {
          return res.getRestaurantName() == restaurantName &&
                 res.getId() == reserveId;
        });
    if (it != resis_.end()) {
      resis_.erase(it);
    }
  }
};
#endif
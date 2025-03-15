#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <map>
#include <string>
#include <vector>

using namespace std;

class Reservation {
private:
  int resiId_;
  string restName_;
  int tableId_;
  int startT_;
  int endT_;
  vector<pair<string, int>> orderedFoods_;
  double totalPrice_;

public:
  Reservation(int reservationId, const string &restaurantName, int tableId,
              int startTime, int endTime)
      : resiId_(reservationId), restName_(restaurantName), tableId_(tableId),
        startT_(startTime), endT_(endTime), totalPrice_(0) {}

  Reservation(int reservationId, const string &restaurantName, int tableId,
              int startTime, int endTime, const vector<string> &foods)
      : resiId_(reservationId), restName_(restaurantName), tableId_(tableId),
        startT_(startTime), endT_(endTime), totalPrice_(0) {
    map<string, int> foodCount;
    for (const auto &food : foods) {
      foodCount[food]++;
    }

    for (const auto &[food, count] : foodCount) {
      orderedFoods_.push_back({food, count});
    }
  }

  int getEndTime() const { return endT_; }
  int getStartTime() const { return startT_; }
  string getRestaurantName() const { return restName_; }
  int getId() const { return resiId_; }
  int getTableId() const { return tableId_; }
  const vector<pair<string, int>> &getOrderedFoods() const {
    return orderedFoods_;
  }
  double getTotalPrice() const { return totalPrice_; }
  void setTotalPrice(double price) { totalPrice_ = price; }
  void addOrderedFood(const string &foodName, int quantity) {
    orderedFoods_.push_back({foodName, quantity});
  }
};
#endif
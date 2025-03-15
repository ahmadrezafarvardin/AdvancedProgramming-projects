#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct PriceInfo {
  int originalPrice;
  int orderAmountDiscount;
  int itemSpecificDiscount;
  int firstOrderDiscount;
  int totalDiscount;
  int finalPrice;
};
class Reservation {
private:
  int resiId_;
  string restName_;
  int tableId_;
  int startT_;
  int endT_;
  vector<pair<string, int>> orderedFoods_;
  PriceInfo price_;

public:
  Reservation(int reservationId, const string &restaurantName, int tableId,
              int startTime, int endTime)
      : resiId_(reservationId), restName_(restaurantName), tableId_(tableId),
        startT_(startTime), endT_(endTime), price_({0, 0, 0, 0, 0, 0}) {}

  Reservation(int reservationId, const string &restaurantName, int tableId,
              int startTime, int endTime, const vector<string> &foods)
      : resiId_(reservationId), restName_(restaurantName), tableId_(tableId),
        startT_(startTime), endT_(endTime), price_({0, 0, 0, 0, 0, 0}) {
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
  const string getRestaurantName() const { return restName_; }
  int getId() const { return resiId_; }
  int getTableId() const { return tableId_; }
  const vector<pair<string, int>> &getOrderedFoods() const {
    return orderedFoods_;
  }
  void addOrderedFood(const string &foodName, int quantity) {
    orderedFoods_.push_back({foodName, quantity});
  }
  const PriceInfo &getPrice() const { return price_; }
  void setPrice(const PriceInfo &price) { price_ = price; }
};
#endif
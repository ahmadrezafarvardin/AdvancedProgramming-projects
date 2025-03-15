#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "Discount.hpp"
#include "District.hpp"
#include "MenuItem.hpp"
#include "Table.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

class Restaurant {
private:
  string name_;
  shared_ptr<District> district_;
  vector<MenuItem> menu_;
  int openingT_;
  int closingT_;
  vector<Table> tables_;
  int lastReservationId_ = 0;
  vector<DishDiscount> dishDiscounts_;
  unique_ptr<FirstOrderDiscount> firstOrderDiscount_;
  unique_ptr<TotalAmountDiscount> totalAmountDiscount_;

public:
  Restaurant(string name, shared_ptr<District> district_, vector<MenuItem> menu,
             int openingTime, int closingTime, vector<Table> tables);
  const shared_ptr<District> &getDistrict() const { return district_; }
  bool hasFood(const string &foodName) const;
  const string &getName() const { return name_; }
  int getOpeningTime() const { return openingT_; }
  int getClosingTime() const { return closingT_; }
  const vector<MenuItem> &getMenu() const { return menu_; }
  const vector<Table> &getTables() const { return tables_; }
  const MenuItem *findMenuItem(const string &foodName) const;
  Table *findTable(int tableId);
  int getNextReservationId() { return ++lastReservationId_; }
  void addReservation(int tableId, const Reservation &reservation);
  void addDishDiscount(const string &dishName, bool isPercentage, int value) {
    dishDiscounts_.emplace_back(dishName, isPercentage, value);
  }
  void setFirstOrderDiscount(bool isPercentage, int value) {
    firstOrderDiscount_ = make_unique<FirstOrderDiscount>(isPercentage, value);
  }
  void setTotalAmountDiscount(bool isPercentage, int value, int minimumAmount) {
    totalAmountDiscount_ =
        make_unique<TotalAmountDiscount>(isPercentage, value, minimumAmount);
  }
  PriceInfo calculatePrices(const vector<string> &foods,
                            bool isFirstOrder) const;
  const vector<DishDiscount> &getDishDiscounts() const {
    return dishDiscounts_;
  }
  const unique_ptr<FirstOrderDiscount> &getFirstOrderDiscount() const {
    return firstOrderDiscount_;
  }
  const unique_ptr<TotalAmountDiscount> &getTotalAmountDiscount() const {
    return totalAmountDiscount_;
  }
};
#endif
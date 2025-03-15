#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "District.hpp"
#include "MenuItem.hpp"
#include "Table.hpp"
#include <algorithm>
#include <map>
#include <memory>

class Restaurant {
private:
  string name_;
  shared_ptr<District> district_;
  vector<MenuItem> menu_;
  int openingT_;
  int closingT_;
  vector<Table> tables_;
  int lastReservationId_ = 0;

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
};
#endif
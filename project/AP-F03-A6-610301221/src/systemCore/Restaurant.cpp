#include "sysCoreInclude/Restaurant.hpp"

Restaurant::Restaurant(string name, shared_ptr<District> district,
                       vector<MenuItem> menu, int openingTime, int closingTime,
                       vector<Table> tables)
    : name_(std::move(name)), district_(district), menu_(std::move(menu)),
      openingT_(openingTime), closingT_(closingTime),
      tables_(std::move(tables)) {}

bool Restaurant::hasFood(const string &foodName) const {
  return std::any_of(
      menu_.begin(), menu_.end(),
      [&foodName](const MenuItem &item) { return item.getName() == foodName; });
}

const MenuItem *Restaurant::findMenuItem(const string &foodName) const {
  auto it =
      find_if(menu_.begin(), menu_.end(), [&foodName](const MenuItem &item) {
        return item.getName() == foodName;
      });
  return it != menu_.end() ? &(*it) : nullptr;
}

Table *Restaurant::findTable(int tableId) {
  auto it =
      find_if(tables_.begin(), tables_.end(), [tableId](const Table &table) {
        return table.getId() == tableId;
      });
  return it != tables_.end() ? &(*it) : nullptr;
}

void Restaurant::addReservation(int tableId, const Reservation &reservation) {
  auto table = findTable(tableId);
  if (table) {
    table->addReservation(reservation);
  }
}
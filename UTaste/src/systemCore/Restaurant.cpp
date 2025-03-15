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

PriceInfo Restaurant::calculatePrices(const vector<string> &foods,
                                      bool isFirstOrder) const {
  PriceInfo prices = {0, 0, 0, 0, 0, 0};
  unordered_map<string, int> foodCount;
  for (const auto &food : foods) {
    foodCount[food]++;
  }

  for (const auto &food : foodCount) {
    const string &foodName = food.first;
    int quantity = food.second;

    const MenuItem *menuItem = findMenuItem(foodName);
    if (!menuItem) {
      cout << "NOT FOUND IN CALCULATE PRICE: " << foodName << "\n";
      continue;
    }

    int itemOriginalPrice = menuItem->getPrice() * quantity;
    prices.originalPrice += itemOriginalPrice;
    prices.finalPrice += itemOriginalPrice;
    auto dishDiscount =
        std::find_if(dishDiscounts_.begin(), dishDiscounts_.end(),
                     [foodName](const DishDiscount &disc) {
                       return disc.getDishName() == foodName;
                     });

    if (dishDiscount != dishDiscounts_.end()) {
      prices.itemSpecificDiscount +=
          dishDiscount->calcDiscount(itemOriginalPrice, quantity);
      prices.finalPrice -=
          dishDiscount->calcDiscount(itemOriginalPrice, quantity);
    }
  }

  if (isFirstOrder && firstOrderDiscount_) {
    prices.firstOrderDiscount +=
        firstOrderDiscount_->calcDiscount(prices.finalPrice);
    prices.finalPrice -= firstOrderDiscount_->calcDiscount(prices.finalPrice);
  }

  if (totalAmountDiscount_ &&
      totalAmountDiscount_->isApplicable(prices.finalPrice)) {
    prices.orderAmountDiscount +=
        totalAmountDiscount_->calcDiscount(prices.finalPrice);
    prices.finalPrice -= totalAmountDiscount_->calcDiscount(prices.finalPrice);
  }
  prices.totalDiscount = prices.itemSpecificDiscount +
                         prices.firstOrderDiscount + prices.orderAmountDiscount;

  return prices;
}
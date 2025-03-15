#ifndef DISCOUNT_HPP
#define DISCOUNT_HPP

#include <string>

using namespace std;

class Discount {
protected:
  bool isPercentage_;
  int value_;

public:
  Discount(bool isPercentage, int value)
      : isPercentage_(isPercentage), value_(value) {}

  virtual int calcDiscount(int amount, int quantity = 1) const {
    if (isPercentage_) {
      return amount * (value_ / 100.0);
    }
    return value_ * quantity;
  }
  bool isPercentage() const { return isPercentage_; }
  int getValue() const { return value_; }

  virtual ~Discount() = default;
};

class DishDiscount : public Discount {
private:
  string dishName_;

public:
  DishDiscount(const string &dishName, bool isPercentage, int value)
      : Discount(isPercentage, value), dishName_(dishName) {}

  const string &getDishName() const { return dishName_; }
};

class FirstOrderDiscount : public Discount {
public:
  FirstOrderDiscount(bool isPercentage, int value)
      : Discount(isPercentage, value) {}
};

class TotalAmountDiscount : public Discount {
private:
  int minimumAmount_;

public:
  TotalAmountDiscount(bool isPercentage, int value, int minimumAmount)
      : Discount(isPercentage, value), minimumAmount_(minimumAmount) {}

  bool isApplicable(int amount) const { return amount >= minimumAmount_; }
  int getMinimumAmount() const { return minimumAmount_; }
};
#endif
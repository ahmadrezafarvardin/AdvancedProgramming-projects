#ifndef DATALOADER_HPP
#define DATALOADER_HPP

#include "../systemCore/sysCoreInclude/District.hpp"
#include "../systemCore/sysCoreInclude/Restaurant.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>

const char TOKEN_SEPERATOR = ',';
const char FOOD_SEPERATOR = ';';
const char PRICE_IDENTIFIER = ':';
const char DISTRICT_SEPARATOR = ',';
const char NEIGHBOR_SEPARATOR = ';';
const string NONE = "none";
const string PERCENT = "percent";

class DataLoader {
public:
  virtual vector<shared_ptr<Restaurant>> loadRestaurants(
      const string &filename, const string &discountsFile,
      const unordered_map<string, shared_ptr<District>> &districtMap) = 0;
  virtual vector<District> loadDistricts(const string &filename) = 0;
  virtual ~DataLoader() = default;
};

class CSVDataLoader : public DataLoader {
public:
  vector<shared_ptr<Restaurant>> loadRestaurants(
      const string &filename, const string &discountsFile,
      const unordered_map<string, shared_ptr<District>> &districtMap) override;
  vector<District> loadDistricts(const string &filename) override;
  vector<shared_ptr<Restaurant>> loadBasicRestaurantsInfo(
      const string &filename,
      const unordered_map<string, shared_ptr<District>> &districtMap);
  void loadDiscounts(const string &filename,
                     vector<shared_ptr<Restaurant>> &restaurants);

private:
  shared_ptr<Restaurant> parseRestaurantLine(
      const string &line,
      const unordered_map<string, shared_ptr<District>> &districtMap);
  vector<MenuItem> parseMenu(const string &menuStr);
  District parseDistrictLine(const string &line);
  void processTotalAmountDiscount(shared_ptr<Restaurant> &restaurant,
                                  const string &discountStr);
  void processFirstOrderDiscount(shared_ptr<Restaurant> &restaurant,
                                 const string &discountStr);
  void processFoodDiscounts(shared_ptr<Restaurant> &restaurant,
                            const string &discountStr);
  vector<string> splitString(const string &str, char delimiter);
};
#endif
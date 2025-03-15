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

class DataLoader {
public:
  virtual vector<shared_ptr<Restaurant>> loadRestaurants(
      const string &filename,
      const unordered_map<string, shared_ptr<District>> &districtMap) = 0;
  virtual vector<District> loadDistricts(const string &filename) = 0;
  virtual ~DataLoader() = default;
};

class CSVDataLoader : public DataLoader {
public:
  vector<shared_ptr<Restaurant>> loadRestaurants(
      const string &filename,
      const unordered_map<string, shared_ptr<District>> &districtMap) override;
  vector<District> loadDistricts(const string &filename) override;

private:
  shared_ptr<Restaurant> parseRestaurantLine(
      const string &line,
      const unordered_map<string, shared_ptr<District>> &districtMap);
  vector<MenuItem> parseMenu(const string &menuStr);
  District parseDistrictLine(const string &line);
};
#endif
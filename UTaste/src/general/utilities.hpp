#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class District;
class Restaurant;
class MenuItem;
class Table;
class Reservation;
struct PriceInfo;
class Discount;
class DishDiscount;
class FirstOrderDiscount;
class TotalAmountDiscount;

using namespace std;

string formatSingleDistrict(const District &district);
string formatDistrictInfo(const vector<District> &districts);
string formatRestList(const vector<pair<string, string>> &restaurants);
string formatMenuItems(const vector<MenuItem> &menu);
string formatTableResis(const vector<Table> &tables);
string formatRestDetails(const shared_ptr<Restaurant> &restaurant);
vector<string> parseFoodList(const string &foodString);
pair<unordered_map<string, string>, vector<string>>
parseParameters(const vector<string> &params);
pair<bool, tuple<string, int, int, int>>
extractReservationParams(const unordered_map<string, string> &reserveParams);
bool isValidTimeRange(int startTime, int endTime);
bool isWithinRestaurantHours(const shared_ptr<Restaurant> &restaurant,
                             int startTime, int endTime);
pair<bool, PriceInfo>
validateAndCalculateOrder(const shared_ptr<Restaurant> &restaurant,
                          const vector<string> &foods, bool isFirstOrder);
shared_ptr<Table> findTable(const shared_ptr<Restaurant> &restaurant,
                            int tableId);
string formatReservationDetails(const Reservation &reservation);
string formatBasicInfo(const shared_ptr<Restaurant> &restaurant);
string formatDiscounts(const shared_ptr<Restaurant> &restaurant);
string formatDishDiscounts(const vector<DishDiscount> &dishDiscounts);
string formatFirstOrderDiscount(const unique_ptr<FirstOrderDiscount> &discount);
string
formatTotalAmountDiscount(const unique_ptr<TotalAmountDiscount> &discount);
string basicReservationFormat(const Reservation &reservation);
string formatReservationResponse(const Reservation &reservation);
#endif // UTILITIES_HPP
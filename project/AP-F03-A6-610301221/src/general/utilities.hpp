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

pair<bool, int>
validateAndCalculateOrder(const shared_ptr<Restaurant> &restaurant,
                          const vector<string> &foods);

shared_ptr<Table> findTable(const shared_ptr<Restaurant> &restaurant,
                            int tableId);

string formatReservationResponse(int reservationId,
                                 const string &restaurantName, int tableId,
                                 int startTime, int endTime,
                                 const vector<string> &foods, int totalPrice);

string formatReservationDetails(const Reservation &reservation);

#endif // UTILITIES_HPP
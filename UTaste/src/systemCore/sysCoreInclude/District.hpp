#ifndef DISTRICT_HPP
#define DISTRICT_HPP

#include <string>
#include <vector>

using namespace std;
class District {
private:
  string name_;
  vector<string> neighbors_;

public:
  District(string name) : name_(std::move(name)) {}
  void addNeighbor(const string &neighbor);
  const string &getName() const { return name_; }
  const vector<string> &getNeighbors() const { return neighbors_; }
};
#endif
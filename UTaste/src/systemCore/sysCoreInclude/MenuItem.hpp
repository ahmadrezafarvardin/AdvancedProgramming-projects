#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include <map>
#include <string>
#include <vector>

using namespace std;
class MenuItem {
private:
  map<string, int> item_;
  string name_;
  int price_;

public:
  MenuItem(string name, int price) : name_(name), price_(price) {}
  const string &getName() const { return name_; }
  int getPrice() const { return price_; }
};
#endif
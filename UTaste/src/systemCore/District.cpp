#include "sysCoreInclude/District.hpp"

void District::addNeighbor(const string &neighbor) {
  neighbors_.push_back(neighbor);
}
#ifndef TABLE_HPP
#define TABLE_HPP

#include "Reservation.hpp"
#include <algorithm>
#include <vector>

using namespace std;

class Table {
private:
  int tableId_;
  vector<Reservation> reservations_;

public:
  explicit Table(int tableId) : tableId_(tableId) {}
  int getId() const { return tableId_; }
  const vector<Reservation> &getReservations() const { return reservations_; }
  void addReservation(const Reservation &reservation) {
    reservations_.push_back(reservation);
  }
  void removeReservation(int reservationId) {
    auto it = find_if(reservations_.begin(), reservations_.end(),
                      [reservationId](const Reservation &res) {
                        return res.getId() == reservationId;
                      });

    if (it != reservations_.end()) {
      reservations_.erase(it);
    }
  }
};

#endif
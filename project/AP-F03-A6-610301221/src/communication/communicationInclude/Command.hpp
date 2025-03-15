#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../../exceptions/exceptions.hpp"
#include "Response.hpp"

class ReservationSystem;
class Command {
public:
  virtual Response execute(ReservationSystem &system) = 0;
  virtual ~Command() = default;
};

#endif

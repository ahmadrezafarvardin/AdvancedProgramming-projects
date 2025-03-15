#include "communicationInclude/CommandHandler.hpp"

Response CommandHandler::handleCommand(const string &input) {
  try {
    unique_ptr<Command> command = CommandFactory::createCmd(input);
    return command->execute(system_);
  } catch (const BadRequestException &e) {
    return BadRequestResponse();
  } catch (const NotFoundException &e) {
    return NotFoundResponse();
  } catch (const exception &e) {
    return Response(UNEXPECTED_ERROR);
  }
}
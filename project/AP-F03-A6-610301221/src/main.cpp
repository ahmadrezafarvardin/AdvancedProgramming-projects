#include "communication/communicationInclude/CommandHandler.hpp"
#include "dataLoader/DataLoader.hpp"
#include "systemCore/sysCoreInclude/ReservationSystem.hpp"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << USAGE << argv[0] << INPUT_FILES << endl;
    return 1;
  }

  auto system = make_unique<ReservationSystem>(make_unique<CSVDataLoader>());
  system->initialize(argv[1], argv[2]);

  CommandHandler handler(*system);
  string input;

  while (getline(cin, input)) {
    if (input == EXIT)
      break;

    Response response = handler.handleCommand(input);
    cout << response.toString() << endl;
  }

  return 0;
}
#include "communication/communicationInclude/WebServer.hpp"
#include "dataLoader/DataLoader.hpp"
#include "systemCore/sysCoreInclude/ReservationSystem.hpp"

int main(int argc, char *argv[]) {
  if (argc != 5) {
    cerr << USAGE << argv[0] << INPUT_FILES << endl;
    return 1;
  }

  try {
    int port = stoi(argv[1]);
    auto system = make_unique<ReservationSystem>(make_unique<CSVDataLoader>());
    system->initialize(argv[2], argv[3], argv[4]);

    WebServer server(*system);
    server.start(port);

  } catch (const exception &e) {
    cerr << ERROR << e.what() << endl;
    return 1;
  }

  return 0;
}
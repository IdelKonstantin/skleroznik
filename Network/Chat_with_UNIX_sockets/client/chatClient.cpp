#include "clientWorker.h"

int main(int argc, char *argv[]) {

    if (argc < 3) {

        std::cerr << "Wrong args..." << std::endl;
        std::cerr << "Usage: " << argv[0] << " [hostname] [port]" << std::endl;
        return 1;
    }

    auto host = argv[1];
    auto port = argv[2];

    clientWorker cw{host, port};

    if(!cw.init()) {

        return 1;
    }

    cw.mainProcessingLoop();
}
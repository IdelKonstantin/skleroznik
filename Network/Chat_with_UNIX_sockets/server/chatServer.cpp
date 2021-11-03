#include "serverWorker.h"

int main() {

    const char* host{"nevermind..."};
    const char* port{"8080"};

    serverWorker sw{host, port};

    if(!sw.init()) {

        return 1;
    }

    sw.mainProcessingLoop();
}

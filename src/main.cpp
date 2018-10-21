#include "ParseCmdLine.h"
#include "JoinServer.h"
#include "DataBase.h"
#include <iostream>

int main(int argc, char *argv[]) {
    DataBase::getInstance().init();

    ParseCmdLine cmdLine(argc, argv);
    if (!cmdLine.is_valid()) {
        std::cerr << cmdLine.error() << std::endl;
        return 1;
    }

    JoinServer app(cmdLine.port());
    app.run();
    return 0;
}

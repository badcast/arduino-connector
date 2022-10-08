#include <signal.h>

#include <iostream>

#include "connector-pipe.h"
#include "connector.h"

const char defaultUsbTty[] = "/dev/ttyUSB%d";

bool forclyFinishing = false;

void delay(int ms);

void out(int) {
    forclyFinishing = true;
    close_pipe();
    printf("\ndaemon exited\n");
    exit(EXIT_FAILURE);
}

void awake(connector::Connector& con);

int main() {
    int x;
    bool result = false;
    connector::Connector con;

    std::cout << "starting daemon" << std::endl;
    signal(SIGKILL, &out);
    signal(SIGINT, &out);

    if (!create_pipe()) {
        std::cout << "failed" << std::endl;
        out(-1);
    }

    std::cout << "started!" << std::endl;
    std::cout << "Openning usb port..." << std::endl;

    // retrive
    for (x = 0; !result && x < 5; ++x) {
        // find free port, and connect to
        result = con.connect();

        if (con) {
            std::cout << "port: " << con.port().name() << " connected!" << std::endl;

            std::cout << "Memory size: " << con.data().memorySize << std::endl;
            std::cout << "Memory free: " << con.data().memoryFree << std::endl;
            std::cout << "Memory used: " << (con.data().memorySize - con.data().memoryFree) << std::endl;
            std::cout << "LCD Rows: " << con.data().lcdRows << ", Cols: " << con.data().lcdCols << std::endl;

            std::string xx = con.port().fullname();
            xx = con.port().name();
            awake(con);

        } else {
            std::cout << "Error opening port, retrive " << x + 1 << std::endl;
            delay(1000);
        }
    }



    if (!result) {
        // todo: error
        std::cout << "5 retrive opening usb port failed. Exiting." << std::endl;
        out(-1);
    }

    con.disconnect();


    std::cout << "Port disconnected!" << std::endl;
}

void awake(connector::Connector& con) {
    connector::ConnectorUtility util(con);

    std::cout << "Check memory allocs: ";
    con.update();
    int memsz = con.data().memoryFree;
    int memsz0;
    auto mem100b = util.malloc(100);

    con.update();
    memsz0 = con.data().memoryFree;

    std::cout << ((memsz == memsz0) ? "not allocated" : "allocated") << std::endl;

    char str[1024];
    while (!forclyFinishing) {
        int readed = read_message(str, 1024);
        if (readed > 0) {
            util.clear();
            util.print(str);
        }
        if (readed) printf("Data received: %s", str);
        delay(100);
    }
}

#include <iostream>
#include <thread>
#include <csignal>
#include "drive_poller.hpp"

bool quit = false;

void sighandler(int signum) {
    quit = true;
}

int main(int argc, char **argv) {
    std::cout << "starting drive poller" << std::endl;
    drive_poller poller;
    poller.start_polling("", "");

    std::signal(SIGTERM, sighandler);
    std::signal(SIGINT, sighandler);
    while(not quit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}

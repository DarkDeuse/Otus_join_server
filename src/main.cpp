#include <iostream>
#include <stdlib.h>     //for using the function sleep

#include "../libs/Server.h"

int main(/*int argc, char const *argv[]*/){


  /*if(argc != 2) {
    std::cerr << "Wrong number of arguments (expected 2). Usage: bulk_server <port> \n" << std::endl;
    return EXIT_FAILURE;
  }

  auto port = std::strtoll(argv[1], nullptr, 0);*/
  auto port = 9000;
  if((port <= 0) || (port > 65535)) {
    std::cerr << "Port must be an integer between 0 and 65535.\n";
    return EXIT_FAILURE;
  }

    try {
        Server bulk_server{static_cast<uint16_t>(port)};
        bulk_server.start();
    } 
        catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

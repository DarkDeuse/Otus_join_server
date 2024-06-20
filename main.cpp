#include <iostream>
#include <stdlib.h> 

#include "libs/Server.h"

int main(){

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

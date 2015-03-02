// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 15
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <netdb.h>

// Prints the usage of the program.
void PrintUsage(char* exeName) {
  std::cerr << "Usage: ./" << exeName << " [hostname] [port] [local file]"
            << std::endl;
  exit(EXIT_FAILURE);
}

void WriteOut(int outFd, int inFd) {
  off_t offset = 0;
  while (true) {
    ssize_t res = sendfile(outFd, inFd, &offset, 1);
    if (res == 0) {
      return;
    }
    if (res == -1) {
      if (errno == EAGAIN || errno == EINTR) continue;
      break;
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 4) {
    PrintUsage(argv[0]);
  }

  // Prepare local file
  int fd = open(argv[3], O_RDONLY);
  if (fd == -1) {
    std::cerr << "Failed to open file '" << argv[3] << "'";
    return EXIT_FAILURE;
  }

  // Prepare socket
  struct addrinfo* ai;
  struct addrinfo hints = {0};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  int res = getaddrinfo(argv[1], argv[2], &hints, &ai);
  if (res) {
    std::cerr << "Failed to get addr info: " << res;
    return EXIT_FAILURE;
  }

  // Make socket and connect
  int soc = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
  if (soc < 0) {
    std::cerr << "Failed to open socket: " << soc;
    return EXIT_FAILURE;
  }

  if (connect(soc, ai->ai_addr, ai->ai_addrlen) == 0) {

  } else {
    std::cerr << "Faild to connect to server.";
  }

  freeaddrinfo(ai);
  return EXIT_SUCCESS;
}

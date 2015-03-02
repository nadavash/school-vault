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
    std::cerr << "Failed to open file '" << argv[3] << "'" << std::endl;
    return EXIT_FAILURE;
  }

  // Prepare socket
  struct addrinfo* ai, * p;
  struct addrinfo hints = {0};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  int res = getaddrinfo(argv[1], argv[2], &hints, &ai);
  if (res) {
    std::cerr << "Failed to get addr info: " << res << std::endl;
    return EXIT_FAILURE;
  }

  // Make socket and connect

  for (p = ai; p; p = p->ai_next) {
    int soc = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (soc < 0) {
      std::cerr << "Failed to open socket: " << soc << std::endl;
    }
    if (connect(soc, p->ai_addr, p->ai_addrlen) == 0) {
      std::cout << "Sending file to server." << std::endl;
      WriteOut(soc, fd);
      break;
    }
  }

  freeaddrinfo(ai);
  return EXIT_SUCCESS;
}

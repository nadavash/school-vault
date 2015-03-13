/*
 * Copyright 2012 Steven Gribble
 *
 *  This file is part of the UW CSE 333 course project sequence
 *  (333proj).
 *
 *  333proj is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  333proj is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with 333proj.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.
#include <cinttypes>

#include "./ServerSocket.h"

extern "C" {
  #include "libhw1/CSE333.h"
}

namespace hw4 {

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int *listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd".
  const int backlog = 10;
  addrinfo *ai;
  addrinfo hints = {0};
  hints.ai_family = ai_family;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  char port[NI_MAXSERV];
  snprintf(port, sizeof(port), "%" PRIu16, port_);
  int res = getaddrinfo(NULL, port, &hints, &ai);
  if (res) {
    return false;
  }

  int fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
  if (fd < 0
      || setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &res, sizeof(res))
      || bind(fd, ai->ai_addr, ai->ai_addrlen)
      || listen(fd, backlog)) {
    freeaddrinfo(ai);
    close(fd);
    return false;
  }

  sock_family_ = ai_family;
  listen_sock_fd_ = fd;
  *listen_fd = fd;

  freeaddrinfo(ai);
  return true;
}

bool ServerSocket::Accept(int *accepted_fd,
                          std::string *client_addr,
                          uint16_t *client_port,
                          std::string *client_dnsname,
                          std::string *server_addr,
                          std::string *server_dnsname) {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.
  sockaddr_storage clt_addr;
  socklen_t clt_addrlen = sizeof(clt_addr);
  char clt_dns[NI_MAXHOST], clt_ip[NI_MAXHOST], clt_port[NI_MAXSERV];

  sockaddr_storage srv_addr;
  socklen_t srv_addrlen = sizeof(srv_addr);
  char srv_dns[NI_MAXHOST], srv_ip[NI_MAXHOST];

  int newfd = accept(listen_sock_fd_, reinterpret_cast<sockaddr *>(&clt_addr),
                     &clt_addrlen);
  if (newfd < 0) {
    return false;
  }
  *accepted_fd = newfd;

  getnameinfo(reinterpret_cast<sockaddr *>(&clt_addr), clt_addrlen, clt_dns,
              sizeof(clt_dns), NULL, 0, 0);
  getnameinfo(reinterpret_cast<sockaddr *>(&clt_addr), clt_addrlen, clt_ip,
              sizeof(clt_ip), clt_port, sizeof(clt_port),
              NI_NUMERICHOST | NI_NUMERICSERV);
  *client_addr = clt_ip;
  *client_port = atoi(clt_port);
  *client_dnsname = clt_dns;

  getsockname(newfd, reinterpret_cast<sockaddr *>(&srv_addr), &srv_addrlen);
  getnameinfo(reinterpret_cast<sockaddr *>(&srv_addr), srv_addrlen, srv_dns,
              sizeof(srv_dns), NULL, 0, 0);
  getnameinfo(reinterpret_cast<sockaddr *>(&srv_addr), srv_addrlen, srv_ip,
              sizeof(srv_ip), NULL, 0, NI_NUMERICHOST);
  *server_addr = srv_ip;
  *server_dnsname = srv_dns;

  return true;
}

}  // namespace hw4

#include "socket.hpp"

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <utils/log.hpp>

int CreateListener(const std::string &port) {
  struct addrinfo hint{};
  hint.ai_flags = AI_PASSIVE;
  hint.ai_family = AF_INET6;
  hint.ai_socktype = SOCK_STREAM;
  struct addrinfo *res = NULL;

  int gai_err;
  if ((gai_err = getaddrinfo(NULL, port.c_str(), &hint, &res))) {
    LOG_ERROR << "Gai error: " << gai_strerror(gai_err);
    return -1;
  }
  int sock = -1;
  for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
    sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sock < 0) {
      LOG_ERROR << "Socket error: " << strerror(errno);
      continue;
    }
    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
      LOG_ERROR << "Setsockeopt error: " << strerror(errno);
      close(sock);
      sock = -1;
      continue;
    }
    if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
      LOG_ERROR << "Bind error: " << strerror(errno);
      close(sock);
      sock = -1;
      continue;
    }
    if (listen(sock, 1024) < 0) {
      LOG_ERROR << "Listen error: " << strerror(errno);
      close(sock);
      sock = -1;
      continue;
    }
    break;
  }
  freeaddrinfo(res);
  return sock;
}

void MakeSocketNonBlocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    LOG_ERROR << "fcntl F_GETFL";
    exit(1);
  }

  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
    LOG_ERROR << "fcntl F_SETFL O_NONBLOCK";
    exit(1);
  }
}

int AcceptConnection(int sock) {
  struct sockaddr_storage client;
  socklen_t len = sizeof(client);
  int connection = accept(sock, (struct sockaddr *)&client, &len);
  if (connection < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      LOG_WARNING << "Accept returned EAGAIN or EWOULDBLOCK\n";
    } else {
      LOG_ERROR << "Accept error: " << strerror(errno);
    }
    return -1;
  }
  MakeSocketNonBlocking(sock);
  char host[NI_MAXHOST], port[NI_MAXSERV];
  getnameinfo((struct sockaddr*)&client, len, host, sizeof(host), port, sizeof(port),
              NI_NUMERICHOST | NI_NUMERICSERV);
  LOG_INFO << "Accepted client: address = " << host << " port = " << port;
  return connection;
}
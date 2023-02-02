#include "epoll_wrapper.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <socket/socket.hpp>
#include <utils/log.hpp>

Epoll::Epoll() {
  epollfd_ = epoll_create1(0);
  if (epollfd_ == -1) {
    LOG_ERROR << "epoll_create1 error " << strerror(errno);
    exit(1);
  }
  events_ = std::vector<epoll_event>(max_events_);
}

Epoll::Epoll(int listener) : listener_(listener) {
  epollfd_ = epoll_create1(0);
  if (epollfd_ == -1) {
    LOG_ERROR << "epoll_create1 error " << strerror(errno);
    exit(1);
  }
  AddListener(listener_);
  events_ = std::vector<epoll_event>(max_events_);
}

int Epoll::Push(int fd, int flags) {
  struct epoll_event server_event;
  server_event.data.fd = fd;
  server_event.events = flags;
  if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &server_event) < 0) {
    LOG_ERROR << "epoll_ctl error: add server socket " << strerror(errno);
    return 1;
  }
  return 0;
}

int Epoll::Pop(int fd) {
  if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, NULL) < 0) {
    LOG_ERROR << "epoll_ctl EPOLL_CTL_DEL";
    return 1;
  }
  return 0;
}

void Epoll::AddListener(int listener, int flags) {
  listener_ = listener;
  Push(listener_, flags);
}

int Epoll::GetEventsNum() {
  return epoll_wait(epollfd_, events_.data(), max_events_, -1);
}

bool Epoll::IsNewConnection(int fd) {
  return fd == listener_;
}

int Epoll::GetConnection(size_t i) const {
  return events_[i].data.fd;
}

int Epoll::Accept() {
  int connection = AcceptConnection(listener_);
  return connection;
}

EventType Epoll::GetEventType(size_t i, bool check_new_connection) {
  if (events_[i].events & (EPOLLERR | EPOLLHUP)) {
    int fd = events_[i].data.fd;
    LOG_ERROR << "socket error: " << fd;
    shutdown(fd, 1);
    close(fd);
    Pop(fd);
    return EventType::ERROR;
  }
  if (check_new_connection && IsNewConnection(events_[i].data.fd)) {
    return EventType::NEW;
  } else if (events_[i].events & EPOLLIN) {
    return EventType::READY;
  } else if (events_[i].events & EPOLLRDHUP) {
    int fd = events_[i].data.fd;
    LOG_ERROR << "socket closing: " << fd;
    shutdown(fd, 1);
    close(fd);
    Pop(fd);
    return EventType::CLOSE;
  }
  return EventType::ERROR;
}
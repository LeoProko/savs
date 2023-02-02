#pragma once

#include <sys/epoll.h>

#include <vector>
// #include <mutex>

enum class EventType {
  ERROR,
  NEW,
  READY,
  CLOSE,
};

class Epoll {
public:
  Epoll();

  Epoll(int listener);

  int Push(int fd, int flags);

  int Pop(int fd);

  void AddListener(int listener, int flags = EPOLLIN);

  int GetEventsNum();

  EventType GetEventType(size_t i, bool check_new_connection);

  int GetConnection(size_t i) const;

  int Accept();

private:
  bool IsNewConnection(int fd);

private:
  int listener_ = -1;
  int max_events_ = 16 * 1024;
  int epollfd_;
  std::vector<struct epoll_event> events_;
  // std::mutex m;
};

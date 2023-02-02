#pragma once

#include <memory>

#include <yaml-cpp/yaml.h>

#include <server/epoll_wrapper.hpp>
#include <ssl/ssl.hpp>

class HTTPSServer {
public:
  HTTPSServer(const std::string& config_path);

  void Run();

  private:
  void Worker(Epoll& epoll);

  void AcceptConnections(Epoll& epoll, std::vector<Epoll>& workers_fds);

private:
  YAML::Node config_;
  std::unique_ptr<MySSL> ssl_;
  int server_sockfd_;
};
#include "https_server.hpp"

#include <thread>
#include <vector>

#include <errno.h>
#include <string.h>
#include <sys/epoll.h>

#include <server/epoll_wrapper.hpp>
#include <server/http.hpp>
#include <socket/socket.hpp>
#include <ssl/ssl.hpp>
#include <utils/log.hpp>

const int THREADS_NUM = 8;

HTTPSServer::HTTPSServer(const std::string& config_path) : config_(YAML::LoadFile(config_path)) {
  std::string port = config_["port"].as<std::string>();
  std::string certificate_path = config_["https"]["certificate_path"].as<std::string>();
  std::string private_key_path = config_["https"]["private_key_path"].as<std::string>();

  LOG_INFO << "HTTPS server configuration: "
           << "port: " << port << ", "
           << "certificate_path: " << certificate_path << ", "
           << "private_key_path : " << private_key_path;

  ssl_ = std::make_unique<MySSL>(std::move(certificate_path), std::move(private_key_path));

  server_sockfd_ = CreateListener(port);
  if (server_sockfd_ < 0) {
    LOG_ERROR << "Couldn't create listen socket";
    exit(1);
  }
}

void HTTPSServer::Worker(Epoll& epoll) {
  for (;;) {
    int events_num = epoll.GetEventsNum();
    LOG_INFO << events_num << " new events in Worker";
    for (int i = 0; i < events_num; ++i) {
      EventType event_type = epoll.GetEventType(i, false);
      int connection = epoll.GetConnection(i);
      if (event_type == EventType::ERROR) {
        LOG_ERROR << "EPOLERR " << strerror(errno);
        continue;
      } else if (event_type == EventType::READY) {
        LOG_INFO << "Worker ready. Connection = " << connection;
        SSLHandler ssl_handler(ssl_->GetCtx(), connection);
        if (SendHtml(ssl_handler.GetSSL(), connection, config_) != 0) {
          LOG_ERROR << "Http serve error";
        }
      } else if (event_type == EventType::CLOSE) {
        LOG_INFO << "Worker close";
        // todo
        continue;
      }
      close(connection);
      epoll.Pop(connection);
    }
  }
}

void HTTPSServer::AcceptConnections(Epoll& epoll, std::vector<Epoll>& workers_fds) {
  for (;;) {
    int events_num = epoll.GetEventsNum();
    LOG_INFO << events_num << " new events in AcceptConnections";
    for (int i = 0; i < events_num; ++i) {
      EventType event_type = epoll.GetEventType(i, true);
      if (event_type == EventType::ERROR) {
        LOG_ERROR << "EPOLERR " << strerror(errno);
      } else if (event_type == EventType::NEW) {
        int connection = epoll.Accept();
        int worker_num = rand() % THREADS_NUM;
        workers_fds[worker_num].AddListener(connection, EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET | EPOLLRDHUP);
        LOG_INFO << "Add to " << worker_num << "-th worker new connection " << connection;
      }
    }
  }
}

void HTTPSServer::Run() {
  std::vector<Epoll> workers_fds(THREADS_NUM);
  std::vector<std::jthread> threads(THREADS_NUM);
  for (int i = 0; i < THREADS_NUM; ++i) {
    workers_fds[i] = Epoll();
    threads[i] = std::jthread(&HTTPSServer::Worker, this, std::ref(workers_fds[i]));
  }

  Epoll server_epoll(server_sockfd_);
  std::jthread listen_thread(&HTTPSServer::AcceptConnections, this, std::ref(server_epoll),
                             std::ref(workers_fds));
}
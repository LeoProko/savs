#include "http.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <ssl/ssl.hpp>
#include <utils/log.hpp>

enum {
  BUF_SIZE = 4096,
};

int SendHtml(SSL* ssl, int connecion, const YAML::Node& config) {
  char data[BUF_SIZE] = {0};
  int ssl_read_result = SSL_read(ssl, data, BUF_SIZE - 1);
  if (ssl_read_result < 0) {
    LOG_ERROR << "SSL error: " << SSL_get_error(ssl, ssl_read_result);
    return 1;
  }

  LOG_INFO << "Received text size: " << ssl_read_result;
  LOG_INFO << "Received text: " << data;

  std::string tmp(data);
  std::stringstream ss;
  ss << tmp;
  std::string method;
  std::string path;
  std::string protocol;
  ss >> method >> path >> protocol;

  LOG_INFO << "Meteod: " << method << ", path: " << path << ", protocol: " << protocol;

  std::string reply =
      "HTTP/1.1 200 OK\r\n"
      "Server: Leo Server\r\n\r\n";
  std::string page_path;
  if (config["paths"][path].IsDefined()) {
    page_path = config["paths"][path]["template"].as<std::string>();
  } else {
    LOG_ERROR << "Wrong path in SendHtml: " << path;
    page_path = "404.html";
  }
  std::ifstream file("/home/leoproko/savs/templates/" + page_path);
  std::ostringstream sstr;
  sstr << file.rdbuf();
  reply += sstr.str();

  LOG_INFO << "Send reply " << reply;

  int ssl_write_result = SSL_write(ssl, reply.c_str(), strlen(reply.c_str()));

  if (ssl_write_result <= 0) {
    LOG_ERROR << "Write error: " << ssl_write_result;
    return 1;
  } else {
    LOG_INFO << "Write ok: " << ssl_write_result;
  }

  // shutdown(connecion, 1);
  close(connecion);

  return 0;
}
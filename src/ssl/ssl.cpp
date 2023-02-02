#include "ssl.hpp"

#include <error.h>
#include <openssl/ssl.h>
#include <string.h>

#include <utils/log.hpp>

void MySSL::InitializeCtx() {
  ctx_ = SSL_CTX_new(TLS_server_method());
  if (ctx_ == NULL) {
    LOG_ERROR << "Unable to create SSL context" << strerror(errno);
    ERR_print_errors_fp(stderr);
    exit(1);
  }
}

void MySSL::ConfigureServerContext(const std::string& certificate_path,
                                   const std::string& private_key_path) {
  if (SSL_CTX_use_certificate_file(ctx_, certificate_path.c_str(), SSL_FILETYPE_PEM) <= 0) {
    LOG_ERROR << "Failed to upload certificate";
    exit(1);
  }

  if (SSL_CTX_use_PrivateKey_file(ctx_, private_key_path.c_str(), SSL_FILETYPE_PEM) <= 0) {
    LOG_ERROR << "Failed to upload key";
    exit(1);
  }
}

void SSLHandler::HandleSSL(int connection) {
  if (connection < 0) {
    LOG_ERROR << "Bad connection";
    return;
  }
  ssl_ = SSL_new(ctx_);
  SSL_set_fd(ssl_, connection);
  int r = SSL_accept(ssl_);
  if (r <= 0) {
    LOG_ERROR << "SSL accept error code " << SSL_get_error(ssl_, r);
  } else {
    LOG_INFO << "SSL accept " << r;
  }
}
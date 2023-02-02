#pragma once

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <string>

class MySSL {
public:
  MySSL(const std::string& certificate_path, const std::string& private_key_path) {
    InitializeCtx();
    ConfigureServerContext(certificate_path, private_key_path);
  }

  SSL_CTX* GetCtx() const {
    return ctx_;
  }

  ~MySSL() {
    SSL_CTX_free(ctx_);
  }

private:
  SSL_CTX* ctx_ = nullptr;

  void InitializeCtx();

  void ConfigureServerContext(const std::string& certificate_path,
                              const std::string& private_key_path);
};

class SSLHandler {
public:
  SSLHandler(SSL_CTX* ctx, int connection) : ctx_(ctx) {
    HandleSSL(connection);
  }

  SSL* GetSSL() const {
    return ssl_;
  }

  ~SSLHandler() {
    SSL_shutdown(ssl_);
    SSL_free(ssl_);
  }

private:
  SSL* ssl_ = nullptr;
  SSL_CTX* ctx_ = nullptr;

  void HandleSSL(int connection);
};
#pragma once

#include <yaml-cpp/yaml.h>

#include <openssl/ssl.h>

#include <string>

int SendHtml(SSL* ssl, int s, const YAML::Node& config);

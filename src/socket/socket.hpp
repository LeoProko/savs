#pragma once

#include <string>

int CreateListener(const std::string& port);

int AcceptConnection(int sock);
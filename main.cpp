#include <sys/types.h>
#include <unistd.h>

#include <server/https_server.hpp>
#include <utils/log.hpp>

int main(int /* argc */, char** argv) {
  InitLogger(argv[0]);
  LOG_INFO << "main init";

  HTTPSServer server("/home/leoproko/savs/api.yaml");
  server.Run();
}

#pragma once

#include <glog/logging.h>

#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)

inline void InitLogger(char* name) {
  google::InitGoogleLogging(name);
  google::SetLogDestination(google::GLOG_INFO, "/home/leoproko/savs/logs/log.kek");
  FLAGS_timestamp_in_logfile_name = false;
  FLAGS_alsologtostderr = true;
}
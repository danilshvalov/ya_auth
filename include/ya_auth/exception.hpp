#pragma once

#include <stdexcept>
#include <string>

namespace ya_auth {

class Exception : public std::exception {
 public:
  explicit Exception(std::string msg) : msg_(std::move(msg)) {}

  const char* what() const noexcept final { return msg_.c_str(); }

 private:
  std::string msg_;
};

class ParseException : public Exception {
 public:
  using Exception::Exception;
};

class InvalidArgumentException : public Exception {
 public:
  using Exception::Exception;
};

}  // namespace ya_auth

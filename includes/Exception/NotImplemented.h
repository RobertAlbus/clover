#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error {
public:
  NotImplementedException() : logic_error("Function not implemented.") {}
  NotImplementedException(const std::string &functionName)
      : logic_error("Function not implemented: " + functionName) {}
};
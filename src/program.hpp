#pragma once

#include <expected>


#include "helpers.hpp"

class Program {
public:
  static void execute(std::string &project_name);

private:
  static std::expected<void, std::string_view>
  remove_underscores(std::string &project_name);
};
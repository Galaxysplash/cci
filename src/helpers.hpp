#pragma once

#include <expected>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

class Helpers {
public:
  static std::expected<std::string, std::string> check_args(const int argc,
                                                            const char *argv[]);

  static void create_file(const std::string &project_dir,
                          const std::string &file_name,
                          const std::string &content = "");

  static void
  make_project_directory(const std::stringstream &project_directory);

  static void
  make_directories(const std::stringstream &project_directory,
                   const std::initializer_list<std::string> &directory_names);

  static void make_directory(const std::stringstream &project_directory,
                             const std::string &directory_name);

  static std::stringstream
  concat_project_directory(const std::string &project_name);

  static std::stringstream
  concat_src_directory(const std::stringstream &project_directory);

  static std::stringstream
  concat_include_directory(const std::stringstream &project_directory);

  static std::stringstream
  concat_new_directory(const std::stringstream &project_directory,
                       const std::string_view &&name);
};
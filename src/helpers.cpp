#include "helpers.hpp"

#include <iostream>
#include <sstream>

bool Helpers::check_args(const int argc) {
  std::stringstream error_message;

  switch (argc) {
  case 2:
    return true;
  case 1:
    error_message
        << "sorry. no project name given.\n"
        << "An example would be 'hello_world'\n"
        << "note, that between 'hello' and 'world' are no spaces only a "
           "underscore.\n";
    break;
  default:
    error_message << "too much arguments"
                  << "please provide only a project_name\n"
                  << "where also no spaces exist between the words,\n"
                  << "only underscores.\n";
    break;
  }

  return false;
}

void Helpers::make_project_directory(
    const std::stringstream &project_directory) {
  std::filesystem::create_directory(project_directory.str());
}

void Helpers::create_file(const std::string &project_dir,
                          const std::string &file_name,
                          const std::string &content) {
  std::stringstream file_path;
  std::ofstream output_filestream;
  file_path << project_dir << "/" << file_name;
  output_filestream.open(file_path.str());
  output_filestream << content;
  output_filestream.close();
}

void Helpers::make_directories(
    const std::stringstream &project_directory,
    const std::initializer_list<std::string> &directory_names) {
  for (const std::string &directory_name : directory_names) {
    make_directory(project_directory, directory_name);
  }
}

void Helpers::make_directory(const std::stringstream &project_directory,
                             const std::string &directory_name) {
  std::stringstream new_folder_directory;
  new_folder_directory << project_directory.str() << "/" << directory_name;
  std::filesystem::create_directory(new_folder_directory.str());
}

std::stringstream
Helpers::concat_project_directory(const std::string &project_name) {
  std::stringstream project_directory;
  project_directory << std::filesystem::current_path().c_str() << "/"
                    << project_name;

  return std::move(project_directory);
}

std::stringstream
Helpers::concat_src_directory(const std::stringstream &project_directory) {
  return concat_new_directory(project_directory, "src");
}

std::stringstream
Helpers::concat_include_directory(const std::stringstream &project_directory) {
  return concat_new_directory(project_directory, "include");
}

std::stringstream
Helpers::concat_new_directory(const std::stringstream &project_directory,
                              const std::string_view &&name) {
  std::stringstream new_directory;

  new_directory << project_directory.str() << "/" << name;

  return std::move(new_directory);
}

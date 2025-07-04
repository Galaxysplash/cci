#include "program.hpp"
#include "file.hpp"

#include <cci/symbol_replacements.hpp>

#include <iostream>

void Program::execute(std::string &project_name) {

  const std::expected<void, std::string_view> &message =
      remove_underscores(project_name);

  if (!message) {
    std::cout << message.error() << "\n";
  }

  const std::stringstream &project_directory =
      Helpers::concat_project_directory(project_name);

  std::filesystem::remove_all(project_directory.str());

  const std::stringstream &src_directory =
      Helpers::concat_src_directory(project_directory);

  std::stringstream cmakelists_content, readme_file_content,
      dot_gitignore_content, vcpkg_content;

  std::stringstream main_cpp_content;

  {
    cmakelists_content
        << "cmake_minimum_required(VERSION 3.20)\nproject(" << project_name
        << ")\n\nset(CMAKE_CXX_STANDARD 23)\nset(CMAKE_CXX_STANDARD_REQUIRED "
        << "ON)\n\ninclude_directories(include)"
        << "\n\nfile(GLOB_RECURSE SOURCES "
        << "src/*.cpp)\nadd_executable(${PROJECT_NAME} ${SOURCES})\n";

    main_cpp_content << "#include <iostream>\n\n"
                     << "int main(int argc, const char *argv[]) {\n"
                     << "  std::cout << \"Hello World\\n\";\n"
                     << "}";

    readme_file_content << "# " << project_name << "\n\n## prerequisites\n"
                        << "->make<br>\n"
                        << "->vcpkg<br>\n"
                        << "->cmake<br>\n\n";

    dot_gitignore_content << "build\n";

    {
      vcpkg_content << "{\n  \"name\": \"" << project_name
                    << "\",\n  \"version\": \"0.1.0\",\n  \"dependencies\": "
                       "[\n\n  ]\n}";
    }
  }

  File global_files[] = {{"CMakeLists.txt", cmakelists_content.str()},
                         {".gitignore", dot_gitignore_content.str()},
                         {"README.md", readme_file_content.str()},
                         {"vcpkg.json", vcpkg_content.str()}};

  Helpers::make_project_directory(project_directory);

  Helpers::make_directories(project_directory, {"build", "src", "include"});

  Helpers::make_directory(Helpers::concat_include_directory(project_directory),
                          project_name);

  for (File &global_file : global_files) {
    Helpers::create_file(project_directory.str(), global_file.get_name(),
                         global_file.get_content());
  }

  File main_cpp_file = {"main.cpp", main_cpp_content.str()};

  Helpers::create_file(Helpers::concat_src_directory(project_directory).str(),
                       main_cpp_file.get_name(), main_cpp_file.get_content());
}

std::expected<void, std::string_view>
Program::remove_underscores(std::string &project_name) {
  bool replacement_was_needed = false;

  for (char &uncecked_char : project_name) {
    for (const auto &[check, replace] :
         SymbolReplacements::checks_and_replaces) {
      if (uncecked_char == check) {
        uncecked_char = replace;
        replacement_was_needed = true;
      }
    }
  }

  if (replacement_was_needed) {
    return std::unexpected("cci replaced underscore(s) with minus(es)");
  }
}
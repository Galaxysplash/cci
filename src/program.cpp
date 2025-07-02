#include "program.hpp"
#include "file.hpp"

#include <iostream>

void Program::execute(const std::string &project_name) {
  const std::stringstream &project_directory =
      Helpers::concat_project_directory(project_name);

  setup(project_name);
}

void Program::setup(const std::string &project_name) {
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
        << ")\n\nset(CMAKE_CXX_STANDARD 20)\nset(CMAKE_CXX_STANDARD_REQUIRED "
        << "ON)\n\ninclude_directories(include)"
        << "\n\nfile(GLOB_RECURSE SOURCES "
        << "src/*.cpp)\nadd_executable(${PROJECT_NAME} ${SOURCES})\n";

    main_cpp_content << "#include <iostream>\n\n"
                     << "int main(int argc, const char *argv[]) {\n"
                     << "  std::cout << \"Hello World\\n\";\n"
                     << "}";

    readme_file_content << "# " << project_name << "\n\n## "
                        << "prerequisites\n"
                        << "->make<br>\n"
                        << "->vcpkg<br>\n"
                        << "->cmake<br>\n\n";

    dot_gitignore_content << "build\n";

    vcpkg_content << "{\n  \"name\": \"" << project_name
                  << "\",\n  \"version\": \"0.1.0\",\n  \"dependencies\": "
                     "[\n\n  ]\n}";
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
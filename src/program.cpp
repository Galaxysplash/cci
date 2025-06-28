#include "program.hpp"
#include "file.hpp"

#include <iostream>

auto Program::execute(
    const std::string &project_name) -> void
{
    const std::stringstream &project_directory = Helpers::concat_project_directory(project_name);

    pre_setup(project_directory);
    setup(project_name, project_directory);
    post_setup(project_name, project_directory);
}

auto Program::pre_setup(
    const std::stringstream &project_directory) -> void
{
    std::filesystem::remove_all(project_directory.str());
}

auto Program::setup(
    const std::string &project_name,
    const std::stringstream &project_directory) -> void
{
    const std::stringstream &src_directory = Helpers::concat_src_directory(project_directory);
    const std::stringstream &include_directory = Helpers::concat_include_directory(project_directory);

    std::stringstream
        cmakelists_content,
        python_build_file_content,
        readme_file_content,
        dot_gitignore_content,
        vcpkg_content,
        foundation_header;

    std::stringstream
        main_cpp_content;

    std::stringstream
        foundation_header_content;
    
    {
        cmakelists_content << "cmake_minimum_required(VERSION 3.20)\nproject(" << project_name << ")\n\nset(CMAKE_CXX_STANDARD 23)\nset(CMAKE_CXX_STANDARD_REQUIRED ON)\n\ninclude_directories(include include/foundation)\n\nfile(GLOB_RECURSE SOURCES src/*.cpp)\nadd_executable(${PROJECT_NAME}_exec ${SOURCES})\n";
        main_cpp_content
            << "#include <foundation.hpp>\n\n"
            << "fn main(const i32 argc, const cstr argv[]) -> i32 {\n"
            << "    std::println(\"Hello World!\");\n"
            << "}";

        python_build_file_content << "import os\nimport platform\nfrom subprocess import run, PIPE\n\n# how your executable should be named\nproject_name: str = \"" << project_name << "\"\n\n# arguments you wonna use for testing\ntesting_arguments = \"\"\n\n\n# build code (don't touch it, unless you know what your doing)\nwindows: bool = platform.system() == \"Windows\"\n\nif windows:\n\trun(\"rmdir /S /Q build\", shell = True)\nelse:\n\trun(\"rm -rf build\", shell = True)\n\nif not os.path.exists(\"build\"):\n\trun(\"mkdir build\", shell=True)\n\ndef execute_cmd(cmd: str, description: str):\n\tvcpkg_result = run(\n\t\tcmd,\n\t\tshell=True,\n\t\tstdout=PIPE,\n\t\tstderr=PIPE\n\t)\n\n\tif vcpkg_result.returncode == 0:\n\t\tprint(description + \" was successful!!!\\n\\n\\n\")\n\telse:\n\t\tprint(\"---DETAIL ZONE---\")\n\t\tif vcpkg_result.stdout:\n\t\t\tprint(vcpkg_result.stdout)\n\t\tif vcpkg_result.stderr:\n\t\t\tprint(vcpkg_result.stderr)\n\t\tprint(\"---END DETAIL ZONE---\")\n\t\t\n\t\tprint(\"\\n\\n\\n\" + description + \" has failed! (details are above)\")\n\t\texit(1)\n\n\n\ncmds = [\n\t\t\t(\n\t\t\t\t\"vcpkg install\",\n\t\t\t\t\"vcpkg installing packages\"\n\t\t\t),\n\t\t\t(\n\t\t\t\t\"cmake . -B build\",\n\t\t\t\t\"CMake pre generating the stuff necessary for building your project\"\n\t\t\t),\n\t\t\t(\n\t\t\t\t\"make -C build\",\n\t\t\t\t\"Make trying to build your project form the cmake generated stuff\"\n\t\t\t)\n\t\t]\n\nfor cmd in cmds:\n\texecute_cmd(cmd[0], cmd[1])\n\nif windows:\n\tif os.path.exists(f\".\\\\build\\\\{project_name}_exec.exe\"):\n\t\trun([f\".\\\\build\\\\{project_name}_exec.exe {testing_arguments}\"], shell=True, check = True)\nelse:\n\tif os.path.exists(f\"./build/{project_name}_exec\"):\n\t\trun([f\"./build/{project_name}_exec {testing_arguments}\"], shell=True, check = True)\n# end building code";

        readme_file_content
            << "# " << project_name << "\n\n## prerequisites\n->python3<br>\n->git<br>\n->make<br>\n->vcpkg<br>\n->cmake<br>\n->vscode<br>\n\n## build & run\n-> On Windows: 'python build.py'<br>\n-> On unix-like systems like (macOS, Linux, ...):'python3 build.py'<br>\n";

        dot_gitignore_content
            << ".vscode\nbuild\n";

        vcpkg_content << "{\n    \"name\": \"" << project_name << "\",\n    \"version\": \"0.1.0\",\n    \"dependencies\": [\n\n    ]\n}";

        foundation_header_content << "#pragma once\n\n#include <print>\n#include <cstdlib>\n#include <expected>\n#include <string_view>\n#include <vector>\n\ntemplate <typename T, typename Err>\nusing Result = std::expected<T, Err>;\n\nusing i8 = int_fast8_t;\nusing i16 = int_fast16_t;\nusing i32 = int_fast32_t;\nusing i64 = int_fast64_t;\n\nusing f32 = float;\nusing f64 = double;\n\nusing u8 = uint_fast8_t;\nusing u16 = uint_fast16_t;\nusing u32 = uint_fast32_t;\nusing u64 = uint_fast64_t;\n\nusing usize = size_t;\n\nusing str = std::string_view;\n\ntemplate <typename T>\nusing Vec = std::vector<T>;\n\nusing String = std::string;\n\nusing cstr = char *;\n\n#define let const auto\n#define var auto\n#define fn auto\n#define impl auto\n#define def const auto\n#define block struct\n";
    }
    File global_files[] = {
        {"CMakeLists.txt", cmakelists_content.str()},
        {"build.py", python_build_file_content.str()},
        {".gitignore", dot_gitignore_content.str()},
        {"README.md", readme_file_content.str()},
        {"vcpkg.json", vcpkg_content.str()}};

    File src_file =
        {"main.cpp", main_cpp_content.str()};

    File includes [] = {
        {"foundation.hpp", foundation_header_content.str()}
    };

    Helpers::make_project_directory(project_directory);

    Helpers::make_directories(project_directory, {"build", "src", "include", "testing", "scripts"});
    Helpers::make_directory(include_directory, project_name);

    for (File &global_file : global_files)
    {
        Helpers::create_file(project_directory.str(), global_file.get_name(), global_file.get_content());
    }

    const auto &foundation_directory = Helpers::concat_new_directory(include_directory, "foundation");

    Helpers::make_directory(include_directory, "foundation");

    for (File &include : includes)
    {
        Helpers::create_file(foundation_directory.str(), include.get_name(), include.get_content());
    }

    Helpers::create_file(src_directory.str(), src_file.get_name(), src_file.get_content());
}

auto Program::post_setup(
    const std::string &project_name,
    const std::stringstream &project_directory) -> void
{
    std::stringstream open_cmd;

    open_cmd << "code " << project_name << "\n";
    system(open_cmd.str().c_str());
    system("exit");
}

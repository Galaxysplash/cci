#include "program.hpp"
#include "file.hpp"

#include <iostream>
#include <cci/files.hpp>

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

    GENERATE_FILES(project_name)

    File global_files[] = {
        {"CMakeLists.txt", cmakelists_content.str()},
        {"build.py", python_build_file_content.str()},
        {".gitignore", dot_gitignore_content.str()},
        {"README.md", readme_file_content.str()},
        {"conanfile.txt", conanfile_txt_content.str()}};

    File src_file =
        {"main.cpp", main_cpp_content.str()};

    Helpers::make_project_directory(project_directory);

    Helpers::make_directories(project_directory, {"build", "src", "include", "testing", "scripts"});
    Helpers::make_directory(include_directory, project_name);

    for (File &global_file : global_files)
    {
        Helpers::create_file(project_directory.str(), global_file.get_name(), global_file.get_content());
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

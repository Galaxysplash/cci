#include "helpers.hpp"

#include <iostream>
#include <sstream>

auto Helpers::check_args(const int argc) -> bool
{
    std::stringstream err_msg;

    switch (argc)
    {
    case 2:
        return true;
    case 1:
        err_msg
            << "sorry. no project name given.\n"
            << "An example would be 'hello_world'\n"
            << "note, that between 'hello' and 'world' are no spaces only a underscore.\n";
        break;
    default:
        err_msg
            << "too much arguments"
            << "please provide only a project_name\n"
            << "where also no spaces exist between the words,\n"
            << "only underscores.\n";
        break;
    }

    return false;
}

auto Helpers::make_project_directory(const std::stringstream &project_directory) -> void
{
    std::filesystem::create_directory(project_directory.str());
}

auto Helpers::create_file(
    const std::string &project_dir,
    const std::string &file_name,
    const std::string &content) -> void
{
    std::stringstream file_path;
    std::ofstream file_creator;
    file_path << project_dir << "/" << file_name;
    file_creator.open(file_path.str());
    file_creator << content;
    file_creator.close();
}

auto Helpers::make_directories(
    const std::stringstream &project_directory,
    const std::initializer_list<std::string> &directory_names) -> void
{
    for (const std::string &directory_name : directory_names)
    {
        make_directory(project_directory, directory_name);
    }
}

auto Helpers::make_directory(
    const std::stringstream &project_directory,
    const std::string &directory_name) -> void
{
    std::stringstream new_folder_directory;
    new_folder_directory << project_directory.str() << "/" << directory_name;
    std::filesystem::create_directory(new_folder_directory.str());
}

auto Helpers::concat_project_directory(const std::string &project_name) -> std::stringstream
{
    std::stringstream project_directory;
    project_directory << std::filesystem::current_path().c_str() << "/" << project_name;

    return std::move(project_directory);
}

auto Helpers::concat_src_directory(const std::stringstream &project_directory) -> std::stringstream
{
    return concat_new_directory(project_directory, "src");
}

auto Helpers::concat_include_directory(const std::stringstream &project_directory) -> std::stringstream
{
    return concat_new_directory(project_directory, "include");
}

auto Helpers::concat_new_directory(
    const std::stringstream &project_directory,
    const std::string_view &&name
) -> std::stringstream
{
    std::stringstream new_directory;

    new_directory << project_directory.str() << "/" << name;

    return std::move(new_directory);
}

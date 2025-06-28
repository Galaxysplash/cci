#pragma once

#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>

struct Helpers final
{
    static auto check_args(const int argc) -> bool;

    static auto create_file(
        const std::string &project_dir,
        const std::string &file_name,
        const std::string &content = ""
    ) -> void;

    static auto make_project_directory(const std::stringstream &project_directory) -> void;
    static auto make_directories(
        const std::stringstream &project_directory,
        const std::initializer_list<std::string> &directory_names) -> void;

    static auto make_directory(
        const std::stringstream &project_directory,
        const std::string &directory_name
    ) -> void;

    static auto concat_project_directory(
        const std::string &project_name
    ) -> std::stringstream;

    static auto concat_src_directory(
        const std::stringstream &project_directory
    ) -> std::stringstream;

    static auto concat_include_directory(
        const std::stringstream &project_directory
    ) -> std::stringstream;

    static auto concat_new_directory(
        const std::stringstream &project_directory,
        const std::string_view &&name
    ) -> std::stringstream;
};
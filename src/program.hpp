#pragma once

#include "helpers.hpp"

struct Program final
{
    static auto execute(
        const std::string &project_name) -> void;

    static auto pre_setup(
        const std::stringstream &project_directory) -> void;

    static auto setup(
        const std::string &project_name,
        const std::stringstream &project_directory) -> void;

    static auto post_setup(
        const std::string &project_name,
        const std::stringstream &project_directory) -> void;
};
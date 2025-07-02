#include "file.hpp"

File::File(const std::string &&name, const std::string &&content)
    : m_name(std::move(name)), m_content(std::move(content)) {}

const std::string &File::get_name() const { return m_name; }

const std::string &File::get_content() const { return m_content; }

#pragma once

#include <string>
#include <string_view>

struct File {
public:
  File(const std::string &&name, const std::string &&content);

  const std::string &get_name() const;
  const std::string &get_content() const;

private:
  const std::string m_name;
  const std::string m_content;
};
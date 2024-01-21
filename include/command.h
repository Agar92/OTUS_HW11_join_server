#pragma once

#include <ostream>
#include <string> 
#include <vector>

struct Command
{
    Command(std::string&& name, std::vector<std::string>&& args) :
        m_name(name), m_arguments(args) {}
    std::string m_name;
    std::vector<std::string> m_arguments;
};

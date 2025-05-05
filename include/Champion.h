#pragma once
#include <string>

class Champion {
    std::string name_;
    std::string imageURL_;
public:
    Champion(std::string name, std::string imageURL) noexcept;
    const std::string& name() const noexcept;
};

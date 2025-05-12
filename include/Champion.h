#pragma once
#include <string>
#include <iostream>

class Champion {
    std::string name_;
    std::string imageURL_;
public:
    Champion(std::string name, std::string imageURL) noexcept;
    const std::string& name() const noexcept;
    const std::string& imageURL()  const noexcept;
};

inline std::ostream& operator<<(std::ostream& os, Champion const& c) {
    os << c.name() << " [img: " << c.imageURL() << "]";
    return os;
}

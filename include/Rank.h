#pragma once
#include <string>

class Rank {
    std::string tier_;
    int points_;
public:
    Rank(std::string tier, int points) noexcept;
    const std::string& tier()   const noexcept;
    int                 points() const noexcept;
    bool operator==(Rank const& o) const noexcept;
    bool operator!=(Rank const& o) const noexcept;
};

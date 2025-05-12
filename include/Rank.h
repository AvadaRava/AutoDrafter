#pragma once
#include <string>
#include <iostream>
#include <ostream>

class Rank {
    std::string tier_;
    int points_;
public:
    Rank(std::string tier, int points) noexcept;
    const std::string& tier()   const noexcept;
    int                 points() const noexcept;
    bool operator==(Rank const& o) const noexcept;
    bool operator!=(Rank const& o) const noexcept;
    static bool compare(Rank const& a, Rank const& b) noexcept;
};

inline std::ostream& operator<<(std::ostream& os, Rank const& r) {
    os << r.tier() << " (" << r.points() << " pts)";
    return os;
}

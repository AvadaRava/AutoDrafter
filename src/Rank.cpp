#include "Rank.h"
#include <utility>
#include <ostream>

Rank::Rank(std::string tier, int points) noexcept
  : tier_{std::move(tier)}, points_{points}
{}

const std::string& Rank::tier() const noexcept {
    return tier_;
}

int Rank::points() const noexcept {
    return points_;
}

bool Rank::operator==(Rank const& o) const noexcept {
    return tier_ == o.tier_ && points_ == o.points_;
}

bool Rank::operator!=(Rank const& o) const noexcept {
    return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, Rank const& r) {
    os << r.tier() << " (" << r.points() << " pts)";
    return os;
}

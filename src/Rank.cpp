#include "Rank.h"
#include <utility>


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

bool Rank::compare(Rank const& a, Rank const& b) noexcept {
  return a.points_ < b.points_;
}


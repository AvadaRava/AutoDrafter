#include "Player.h"
#include <utility>
#include <ostream>

Player::Player(std::string name, Champion champ, Rank rank) noexcept
  : name_{std::move(name)}
  , champ_{std::move(champ)}
  , rank_{std::move(rank)}
{}

Player::Player(Player const& o) noexcept
  : name_{o.name_}
  , champ_{o.champ_}
  , rank_{o.rank_}
{}

Player& Player::operator=(Player o) noexcept {
    swap(*this, o);
    return *this;
}

void swap(Player& a, Player& b) noexcept {
    using std::swap;
    swap(a.name_, b.name_);
    swap(a.champ_, b.champ_);
    swap(a.rank_, b.rank_);
}

const std::string& Player::name() const noexcept {
    return name_;
}

const Champion& Player::champion() const noexcept {
    return champ_;
}

const Rank& Player::rank() const noexcept {
    return rank_;
}

void Player::setChampion(Champion c) noexcept {
    champ_ = std::move(c);
}

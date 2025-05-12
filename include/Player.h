#pragma once
#include <string>
#include "Champion.h"
#include "Rank.h"
#include <ostream>

class Player {
    std::string name_;
    Champion     champ_;
    Rank         rank_;
public:
    Player(std::string name, Champion champ, Rank rank) noexcept;
    Player(Player const& o) noexcept;
    Player& operator=(Player o) noexcept;
    ~Player() = default;

    const std::string& name()     const noexcept;
    const Champion&     champion() const noexcept;
    const Rank&         rank()     const noexcept;

    void setChampion(Champion c) noexcept;

    friend void swap(Player& a, Player& b) noexcept;
};

inline std::ostream& operator<<(std::ostream& os, Player const& p) {
    os << "Player: "      << p.name()
       << " | Champ: "    << p.champion().name()
       << " | Rank: "     << p.rank();
    return os;
}

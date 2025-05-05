#pragma once
#include <string>
#include "Champion.h"
#include "Rank.h"

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

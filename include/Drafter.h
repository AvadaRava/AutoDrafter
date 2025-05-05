#pragma once
#include <memory>
#include "Champion.h"

class Server;  // forward

class Drafter {
public:
    virtual ~Drafter() noexcept = default;
    virtual Champion draftChampion(Server const& s) const = 0;
    virtual std::unique_ptr<Drafter> clone() const = 0;
};

class RandomDrafter : public Drafter {
public:
    Champion draftChampion(Server const& s) const override;
    std::unique_ptr<Drafter> clone() const override;
};

class HighWinRateDrafter : public Drafter {
public:
    Champion draftChampion(Server const& s) const override;
    std::unique_ptr<Drafter> clone() const override;
    double threshold() const noexcept;
};

class BalancedDrafter : public Drafter {
public:
    Champion draftChampion(Server const& s) const override;
    std::unique_ptr<Drafter> clone() const override;
};

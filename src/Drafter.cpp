#include "Drafter.h"
#include "Server.h"
#include "Exceptions.h"
#include <random>
#include <algorithm>

std::unique_ptr<Drafter> RandomDrafter::clone() const {
    return std::make_unique<RandomDrafter>(*this);
}

Champion RandomDrafter::draftChampion(Server const& s) const {
    auto const& ps = s.getPlayers();
    if(ps.empty()) throw NoPlayersException();
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dist(0, int(ps.size())-1);
    return ps[dist(gen)].champion();
}

std::unique_ptr<Drafter> HighWinRateDrafter::clone() const {
    return std::make_unique<HighWinRateDrafter>(*this);
}

double HighWinRateDrafter::threshold() const noexcept {
    return 0.60;
}

Champion HighWinRateDrafter::draftChampion(Server const& s) const {
    return s.getMostPopularChampion();
}

std::unique_ptr<Drafter> BalancedDrafter::clone() const {
    return std::make_unique<BalancedDrafter>(*this);
}

Champion BalancedDrafter::draftChampion(Server const& s) const {
    return s.getMedianChampion();
}

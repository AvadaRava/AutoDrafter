#include "Server.h"
#include "Exceptions.h"
#include <algorithm>
#include <unordered_map>

Server::Server(std::string name, Rank rank, std::unique_ptr<Drafter> dr)
  : name_{std::move(name)}
  , rank_{std::move(rank)}
  , drafter_{std::move(dr)}
{}

Server::Server(Server const& o)
  : name_{o.name_}
  , rank_{o.rank_}
  , players_{o.players_}
  , drafter_{o.drafter_ ? o.drafter_->clone() : nullptr}
{}

Server& Server::operator=(Server o) {
    swap(*this, o);
    return *this;
}

void swap(Server& a, Server& b) noexcept {
    using std::swap;
    swap(a.name_, b.name_);
    swap(a.rank_, b.rank_);
    swap(a.players_, b.players_);
    swap(a.drafter_, b.drafter_);
}

void Server::addPlayer(Player p) {
    if(p.rank() != rank_) throw InvalidRankException();
    players_.push_back(std::move(p));
}

bool Server::removePlayer(std::string const& name) {
    auto it = std::remove_if(players_.begin(), players_.end(),
        [&](auto const& pl){ return pl.name() == name; });
    if(it == players_.end()) return false;
    players_.erase(it, players_.end());
    return true;
}

void Server::setDrafter(std::unique_ptr<Drafter> dr) {
    drafter_ = std::move(dr);
}

Champion Server::getMostPopularChampion() const {
    if(players_.empty()) throw NoPlayersException();
    std::unordered_map<std::string,int> cnt;
    for(auto const& p : players_) cnt[p.champion().name()]++;
    auto it = std::max_element(cnt.begin(), cnt.end(),
        [](auto const& a, auto const& b){ return a.second < b.second; });
    return Champion(it->first, "");
}

Champion Server::getMedianChampion() const {
    if(players_.empty()) throw NoPlayersException();
    std::vector<std::string> names;
    for(auto const& p : players_) names.push_back(p.champion().name());
    std::sort(names.begin(), names.end());
    return Champion(names[names.size()/2], "");
}

Champion Server::performDraft() const {
    if(!drafter_) throw DrafterException();
    // exemplu de dynamic_cast
    if (auto const* hw = dynamic_cast<HighWinRateDrafter const*>(drafter_.get())) {
        (void)hw->threshold();  
    }
    return drafter_->draftChampion(*this);
}

const std::vector<Player>& Server::getPlayers() const noexcept {
    return players_;
}
std::ostream& operator<<(std::ostream& os, Server const& s) {
    os << "Server: " << s.name_ 
       << "\nRank server: " << s.rank_ 
       << "\nJucători:\n";
    for (auto const& p : s.players_) {
        os << "  - " << p << "\n";
    }
    return os;
}

// File: Exceptions.h
#pragma once
#include <stdexcept>
#include <string>

// Bază pentru excepții proprii
class LoLException : public std::exception {
public:
    explicit LoLException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};

// Erori specifice
class InvalidRankException : public LoLException {
public:
    InvalidRankException() : LoLException("Invalid rank for operation") {}
};

class NoPlayersException : public LoLException {
public:
    NoPlayersException() : LoLException("No players available on server") {}
};

class DrafterException : public LoLException {
public:
    DrafterException() : LoLException("Drafter failed to draft") {}
};


// File: Rank.h
#pragma once
#include <string>
#include <utility>
#include <algorithm>

class Rank {
    std::string tier_;
    int points_;
public:
    Rank(std::string tier, int points);
    const std::string& tier() const noexcept;
    int points() const noexcept;
    bool operator==(Rank const& other) const noexcept;
    bool operator!=(Rank const& other) const noexcept;
    static bool compare(Rank const& a, Rank const& b) noexcept;
};

// File: Rank.cpp
#include "Rank.h"
Rank::Rank(std::string tier, int points) : tier_(std::move(tier)), points_(points) {}
const std::string& Rank::tier() const noexcept { return tier_; }
int Rank::points() const noexcept { return points_; }
bool Rank::operator==(Rank const& other) const noexcept {
    return tier_ == other.tier_ && points_ == other.points_;
}
bool Rank::operator!=(Rank const& other) const noexcept { return !(*this == other); }
bool Rank::compare(Rank const& a, Rank const& b) noexcept {
    return a.points_ < b.points_;
}

// File: Champion.h
#pragma once
#include <string>

class Champion {
    std::string name_;
    std::string imageURL_;
public:
    Champion(std::string name, std::string imageURL);
    const std::string& name() const noexcept;
};

// File: Champion.cpp
#include "Champion.h"
Champion::Champion(std::string name, std::string imageURL)
    : name_(std::move(name)), imageURL_(std::move(imageURL)) {}
const std::string& Champion::name() const noexcept { return name_; }

// File: Player.h
#pragma once
#include <string>
#include "Champion.h"
#include "Rank.h"

class Player {
    std::string name_;
    Champion champ_;
    Rank rank_;
public:
    Player(std::string name, Champion champ, Rank rank);
    Player(Player const& other);
    Player& operator=(Player other);
    ~Player() = default;
    const Rank& rank() const noexcept;
    const std::string& name() const noexcept;
    void setChampion(Champion c);
    const Champion& champion() const noexcept;
    friend void swap(Player& a, Player& b) noexcept;
};

// File: Player.cpp
#include "Player.h"
#include <utility>
Player::Player(std::string name, Champion champ, Rank rank)
    : name_(std::move(name)), champ_(std::move(champ)), rank_(std::move(rank)) {}
Player::Player(Player const& other)
    : name_(other.name_), champ_(other.champ_), rank_(other.rank_) {}
Player& Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}
void swap(Player& a, Player& b) noexcept {
    using std::swap;
    swap(a.name_, b.name_);
    swap(a.champ_, b.champ_);
    swap(a.rank_, b.rank_);
}
const Rank& Player::rank() const noexcept { return rank_; }
const std::string& Player::name() const noexcept { return name_; }
void Player::setChampion(Champion c) { champ_ = std::move(c); }
const Champion& Player::champion() const noexcept { return champ_; }

// File: Drafter.h
#pragma once
#include <memory>
#include "Champion.h"
class Server;

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
    // funcție specifică
    double threshold() const noexcept;
};

class BalancedDrafter : public Drafter {
public:
    Champion draftChampion(Server const& s) const override;
    std::unique_ptr<Drafter> clone() const override;
};

// File: Drafter.cpp
#include "Drafter.h"
#include "Server.h"
#include "NoPlayersException.h" // ensure exception definitions
#include <random>

std::unique_ptr<Drafter> RandomDrafter::clone() const {
    return std::make_unique<RandomDrafter>(*this);
}
Champion RandomDrafter::draftChampion(Server const& s) const {
    auto players = s.getPlayers();
    if(players.empty()) throw NoPlayersException();
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dist(0, players.size()-1);
    return players[dist(gen)].champion();
}

std::unique_ptr<Drafter> HighWinRateDrafter::clone() const {
    return std::make_unique<HighWinRateDrafter>(*this);
}
double HighWinRateDrafter::threshold() const noexcept { return 0.6; }
Champion HighWinRateDrafter::draftChampion(Server const& s) const {
    // exemplu dummy: return most popular
    return s.getMostPopularChampion();
}

std::unique_ptr<Drafter> BalancedDrafter::clone() const {
    return std::make_unique<BalancedDrafter>(*this);
}
Champion BalancedDrafter::draftChampion(Server const& s) const {
    // echilibrare: pick median player champion
    return s.getMedianChampion();
}

// File: Server.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Player.h"
#include "Rank.h"
#include "Drafter.h"

class Server {
    std::string name_;
    Rank rank_;
    std::vector<Player> players_;
    std::unique_ptr<Drafter> drafter_;
public:
    Server(std::string name, Rank r, std::unique_ptr<Drafter> d);
    Server(Server const& other);
    Server& operator=(Server other);
    ~Server() = default;
    void addPlayer(Player p);
    bool removePlayer(const std::string& name);
    void setDrafter(std::unique_ptr<Drafter> d);
    Champion performDraft() const;
    Champion getMostPopularChampion() const;
    Champion getMedianChampion() const;
    const std::vector<Player>& getPlayers() const noexcept;
    friend void swap(Server& a, Server& b) noexcept;
};

// File: Server.cpp
#include "Server.h"
#include "InvalidRankException.h"
#include "NoPlayersException.h"
#include <algorithm>

Server::Server(std::string name, Rank r, std::unique_ptr<Drafter> d)
    : name_(std::move(name)), rank_(std::move(r)), drafter_(std::move(d)) {}

Server::Server(Server const& other)
    : name_(other.name_), rank_(other.rank_),
      players_(other.players_),
      drafter_(other.drafter_ ? other.drafter_->clone() : nullptr) {}

Server& Server::operator=(Server other) {
    swap(*this, other);
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

bool Server::removePlayer(const std::string& name) {
    auto it = std::remove_if(players_.begin(), players_.end(),
        [&](Player const& pl){ return pl.name() == name; });
    if(it == players_.end()) return false;
    players_.erase(it, players_.end());
    return true;
}

void Server::setDrafter(std::unique_ptr<Drafter> d) {
    drafter_ = std::move(d);
}

Champion Server::getMostPopularChampion() const {
    if(players_.empty()) throw NoPlayersException();
    std::unordered_map<std::string,int> count;
    for(auto const& pl: players_) count[pl.champion().name()]++;
    auto best = std::max_element(count.begin(), count.end(),
        [](auto const& a, auto const& b){ return a.second < b.second; });
    return Champion(best->first, "");
}

Champion Server::getMedianChampion() const {
    if(players_.empty()) throw NoPlayersException();
    std::vector<std::string> names;
    for(auto const& pl: players_) names.push_back(pl.champion().name());
    std::sort(names.begin(), names.end());
    return Champion(names[names.size()/2], "");
}

Champion Server::performDraft() const {
    if(!drafter_) throw DrafterException();
    if(players_.empty()) throw NoPlayersException();
    // demonstrație dynamic_cast
    if(auto* ptr = dynamic_cast<HighWinRateDrafter*>(drafter_.get())) {
        double th = ptr->threshold(); (void)th; // folosit threshold
    }
    return drafter_->draftChampion(*this);
}

const std::vector<Player>& Server::getPlayers() const noexcept { return players_; }

// File: main.cpp
#include <iostream>
#include <memory>
#include "Server.h"
#include "Drafter.h"
#include "Rank.h"
#include "Champion.h"
#include "Player.h"
#include "Exceptions.h"

int main() {
    try {
        auto drafter = std::make_unique<HighWinRateDrafter>();
        Server srv("EUW", {"Gold",2500}, std::move(drafter));
        srv.addPlayer({"Alice", {"Lux", "url"}, {"Gold",2500}});
        srv.addPlayer({"Bob",   {"Garen","url"}, {"Gold",2500}});
        Champion pick = srv.performDraft();
        std::cout << "Drafted: " << pick.name() << '\n';
    } catch(const LoLException& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}

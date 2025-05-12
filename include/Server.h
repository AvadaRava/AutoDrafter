#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Player.h"
#include "Rank.h"
#include "Drafter.h"
#include <ostream> 

class Server {
    std::string                name_;
    Rank                       rank_;
    std::vector<Player>        players_;
    std::unique_ptr<Drafter>   drafter_;
//helper intern pt operator<<
std::string playersToString() const;

public:
    Server(std::string name, Rank rank, std::unique_ptr<Drafter> dr);
    Server(Server const& o);
    Server& operator=(Server o);
    ~Server() = default;

    void addPlayer(Player p);
    bool removePlayer(std::string const& name);
    void setDrafter(std::unique_ptr<Drafter> dr);

    Champion performDraft() const;
    Champion getMostPopularChampion() const;
    Champion getMedianChampion() const;
    const std::vector<Player>& getPlayers() const noexcept;
    const std::string& getName()    const noexcept { return name_; }
    const Rank&       getRank()     const noexcept { return rank_; }

     // swap prieten, pentru copy,swap
     friend void swap(Server& a, Server& b) noexcept;

      // output-stream
    friend std::ostream& operator<<(std::ostream& os, Server const& s);
};

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>


class Rank {
private:
    std::string tier;  
    int points;

public:
    
    Rank(const std::string& tier, int points)
        : tier(tier), points(points) {}

    
    const std::string& getTier() const { return tier; }
    int getPoints() const { return points; }

    // Op== pentru comparare
    
    bool operator==(const Rank& other) const {
        return (tier == other.tier && points == other.points);
    }

    // Op<< pentru afisare
    
    friend std::ostream& operator<<(std::ostream& os, const Rank& rank) {
        os << rank.tier << " (" << rank.points << " pts)";
        return os;
    }
};



class Champion {
private:
    std::string name;
    std::string imageURL;

public:
    // Cons de init
    
    Champion(const std::string& name, const std::string& imageURL)
        : name(name), imageURL(imageURL) {}

    
    const std::string& getName() const { return name; }
    const std::string& getImageURL() const { return imageURL; }

    // Op<< pentru afisare
    
    friend std::ostream& operator<<(std::ostream& os, const Champion& champ) {
        os << "Champion: " << champ.name << " [Image: " << champ.imageURL << "]";
        return os;
    }
};


class Player {
private:
    std::string playerName;
    Champion champion; 
    Rank rank;        

public:
    // Cons de init    
    Player(const std::string& playerName, const Champion& champion, const Rank& rank)
        : playerName(playerName), champion(champion), rank(rank) {}

    
    const std::string& getPlayerName() const { return playerName; }
    const Champion& getChampion() const { return champion; }
    const Rank& getRank() const { return rank; }

  
    // mod campion favorit  
    void setChampion(const Champion& newChampion) {
        champion = newChampion;
    }

    // Op << pt afisare 
    
    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
        os << "Player: " << player.playerName << "\n  " << player.champion 
           << "\n  Rank: " << player.rank;
        return os;
    }
};


class Server {
private:
    std::string serverName;
    Rank serverRank;                  
    std::vector<Player> players;       

   // repr a jucatorilor
 
    std::string playersToString() const {
        std::ostringstream oss;
        for (const auto& player : players) {
            oss << player << "\n";
        }
        return oss.str();
    }

public:
  
    Server(const std::string& serverName, const Rank& serverRank, const std::vector<Player>& players)
        : serverName(serverName), serverRank(serverRank), players(players) {}

    // Cons de init
  
    Server(const std::string& serverName, const Rank& serverRank)
        : serverName(serverName), serverRank(serverRank) {}

    // cons de cop
  
    Server(const Server& other)
        : serverName(other.serverName), serverRank(other.serverRank), players(other.players) {}

    // Op de cop
  
    Server& operator=(const Server& other) {
        if (this != &other) {
            serverName = other.serverName;
            serverRank = other.serverRank;
            players = other.players;
        }
        return *this;
    }

    // Destructor
    ~Server() {
    }

   
    bool addPlayer(const Player& player) {
        if (player.getRank() == serverRank) {
            players.push_back(player);
            return true;
        }
        std::cerr << "Eroare: Jucătorul " << player.getPlayerName() 
                  << " are rank diferit de server (" << serverRank << ").\n";
        return false;
    }

   
    bool removePlayer(const std::string& playerName) {
        auto it = std::remove_if(players.begin(), players.end(),
            [&playerName](const Player& p) { return p.getPlayerName() == playerName; });
        if (it != players.end()) {
            players.erase(it, players.end());
            return true;
        }
        return false;
    }
   
    bool updatePlayerChampion(const std::string& playerName, const Champion& newChampion) {
        for (auto& player : players) {
            if (player.getPlayerName() == playerName) {
                player.setChampion(newChampion);
                return true;
            }
        }
        return false;
    }
   
    Champion getMostPopularChampion() const {
        std::unordered_map<std::string, int> champCount;
        
        for (const auto& player : players) {
            ++champCount[player.getChampion().getName()];
        }
        
        std::string popularName;
        int maxCount = 0;
        for (const auto& pair : champCount) {
            if (pair.second > maxCount) {
                maxCount = pair.second;
                popularName = pair.first;
            }
        }
       
        
        return Champion(popularName, "unknown");
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Server& server) {
        os << "Server: " << server.serverName 
           << "\nRank server: " << server.serverRank
           << "\nJucători:\n" << server.playersToString();
        return os;
    }
};

int main() {
    

    Rank bronze("Bronze", 800);
    Rank silver("Silver", 1500);
    Rank gold("Gold", 2500);
    
    Champion champAshe("Ashe", "http://imagelink.com/ashe.jpg");
    Champion champGaren("Garen", "http://imagelink.com/garen.jpg");
    Champion champLux("Lux", "http://imagelink.com/lux.jpg");
    Champion champAnnie("Annie", "http://imagelink.com/annie.jpg");

    
    Player player1("PlayerOne", champAshe, bronze);
    Player player2("PlayerTwo", champGaren, bronze);
    Player player3("PlayerThree", champLux, bronze);
    
    Player player4("PlayerFour", champAnnie, silver); // va fi refuzat de serverul Bronze

    
    Server serverBronze("Server-Bronze", bronze);
    
    serverBronze.addPlayer(player1);
    serverBronze.addPlayer(player2);
    serverBronze.addPlayer(player3);
    
    // Adaugare jucator cu rang diferit (va fi refuzat)
    
    serverBronze.addPlayer(player4);
    
    std::cout << "=== Afisare Server Bronze ===\n" << serverBronze << "\n";

    // schimbare campion pt un jucator
    
    std::cout << "\nActualizare campion pentru PlayerTwo...\n";
    serverBronze.updatePlayerChampion("PlayerTwo", champLux);
    std::cout << serverBronze << "\n";

    // Cel mai popular campion 
    
    Champion popular = serverBronze.getMostPopularChampion();
    std::cout << "\nCampionul cel mai popular de pe server: " << popular << "\n";

    
    std::cout << "\nDemonstratie constructor de copiere si operator=:\n";
    Server copyServer = serverBronze;  
    Server assignServer("Temp", bronze);
    
    assignServer = serverBronze;
    std::cout << "CopyServer:\n" << copyServer << "\n";
    std::cout << "AssignServer:\n" << assignServer << "\n";

    return 0;
}

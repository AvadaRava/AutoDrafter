#include <iostream>
#include <memory>
#include "Exceptions.h"
#include "Rank.h"
#include "Champion.h"
#include "Player.h"
#include "Drafter.h"
#include "Server.h"

int main() {
    try {
        // Strategia inițială
        auto drafter = std::make_unique<HighWinRateDrafter>();
        Server srv("EUW", Rank{"Gold",2500}, std::move(drafter));

        // Adăugăm doi jucători
        srv.addPlayer(Player{"Alice", Champion{"Lux","url"}, Rank{"Gold",2500}});
        srv.addPlayer(Player{"Bob",   Champion{"Garen","url"}, Rank{"Gold",2500}});

        // Draft
        Champion pick = srv.performDraft();
        std::cout << "Drafted champion: " << pick.name() << "\n";

        // Schimbăm strategia la Random
        srv.setDrafter(std::make_unique<RandomDrafter>());
        pick = srv.performDraft();
        std::cout << "Next drafted (random): " << pick.name() << "\n";

    } catch (LoLException const& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}

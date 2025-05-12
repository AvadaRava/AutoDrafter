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
        // 1. Construim serverul cu strategia inițială
        auto drafter = std::make_unique<HighWinRateDrafter>();
        Server srv("EUW", Rank{"Gold",2500}, std::move(drafter));

        // 2. Creăm doi jucători și îi adăugăm pe server
        Player alice("Alice", Champion{"Lux","url"}, Rank{"Gold",2500});
        Player bob  ("Bob",   Champion{"Garen","url"}, Rank{"Gold",2500});

        srv.addPlayer(alice);
        srv.addPlayer(bob);

        // 3. Facem un draft
        Champion pick = srv.performDraft();
        std::cout << "Drafted champion: " << pick.name() << "\n";

        // ─────────────────────────────────────────────────────────────────────
        // 4. Folosim Player::setChampion() pe Alice
        std::cout << "\n[TEST] schimb champion pentru Alice...\n";
        alice.setChampion(Champion{"Annie","url"});
        std::cout << "Alice joacă acum cu: " 
                  << alice.champion().name() << "\n";

        // 5. Folosim Rank::tier() și Rank::points()
        std::cout << "\n[TEST] info rank Alice:\n"
                  << " Tier:   " << alice.rank().tier()  << "\n"
                  << " Puncte: " << alice.rank().points() << "\n";

        // 6. Folosim Server::removePlayer() pentru Bob
        std::cout << "\n[TEST] ștergem Bob de pe server...\n";
        bool removed = srv.removePlayer("Bob");
        std::cout << (removed 
                      ? "→ Bob a fost eliminat cu succes\n"
                      : "→ Bob nu a fost găsit\n");
        std::cout << "\nServer după ștergere:\n" 
                  << srv << "\n";

        // ─────────────────────────────────────────────────────────────────────
        // 7. Schimbăm strategia la Random și re-draft
        srv.setDrafter(std::make_unique<RandomDrafter>());
        pick = srv.performDraft();
        std::cout << "Next drafted (random): " << pick.name() << "\n";

    } catch (LoLException const& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}

#include <iostream>
#include <memory>
#include "Exceptions.h"
#include "Rank.h"
#include "Champion.h"
#include "Player.h"
#include "Drafter.h"
#include "Server.h"
#include <vector>
#include <algorithm>

int main() {
    try {
        // ─────────────────────────────────────────────────────────────────────
        // 1. Construim serverul cu strategia initiala
        auto drafter = std::make_unique<HighWinRateDrafter>();
        Server srv("EUW", Rank{"Gold",2500}, std::move(drafter));
        // ─────────────────────────────────────────────────────────────────────
        // 2. Cream doi jucatori si ii adaugam pe server
        Player alice("Alice", Champion{"Lux","url"}, Rank{"Gold",2500});
        Player bob  ("Bob",   Champion{"Garen","url"}, Rank{"Gold",2500});
        
        srv.addPlayer(alice);
        srv.addPlayer(bob);
        // ─────────────────────────────────────────────────────────────────────
        // 3. Facem un draft
        Champion pick = srv.performDraft();
        std::cout << "Drafted champion: " << pick.name() << "\n";

        // ─────────────────────────────────────────────────────────────────────
        // 4. Folosim Player::setChampion() pe Alice
        std::cout << "\n[TEST] schimb champion pentru Alice...\n";
        alice.setChampion(Champion{"Annie","url"});
        std::cout << "Alice joacă acum cu: " 
                  << alice.champion().name() << "\n";
        // ─────────────────────────────────────────────────────────────────────
        // 5. Folosim Rank::tier() si Rank::points()
        std::cout << "\n[TEST] info rank Alice:\n"
                  << " Tier:   " << alice.rank().tier()  << "\n"
                  << " Puncte: " << alice.rank().points() << "\n";
        // ─────────────────────────────────────────────────────────────────────
        // 6. Folosim Server::removePlayer() pentru Bob
        std::cout << "\n[TEST] stergem Bob de pe server...\n";
        bool removed = srv.removePlayer("Bob");
        std::cout << (removed 
                      ? "→ Bob a fost eliminat cu succes\n"
                      : "→ Bob nu a fost gasit\n");
        std::cout << "\nServer dupa stergere:\n" 
                  << srv << "\n";

        // ─────────────────────────────────────────────────────────────────────
        // 7. Schimbam strategia la Random si re-draft
        srv.setDrafter(std::make_unique<RandomDrafter>());
        pick = srv.performDraft();
        std::cout << "Next drafted (random): " << pick.name() << "\n";
        // ─────────────────────────────────────────────────────────────────────
        // 8. Demonstratie utilizare Rank::compare
        std::cout << "\n[TEST] sortare ranguri cu Rank::compare...\n";
        std::vector<Rank> ranks{
            alice.rank(),
            bob.rank(),
            Rank{"Platinum", 3000}
        };
        std::sort(ranks.begin(), ranks.end(), Rank::compare);
        for (auto const& r : ranks) {
            std::cout << r << "  ";
        }
        std::cout << "\n";


    } catch (LoLException const& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}

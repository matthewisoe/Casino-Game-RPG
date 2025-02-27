#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Card.h"
#include "Deck.h"

class blackjackGame {
private:
    Deck deck;
    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;
    bool playerTurn;
    int playerBalance;
    int currentBet;
    bool dealerSecondCardFaceUp;
    int balance;

public:
    blackjackGame();

    int GetBalance();
    void setBalance(int newBalance);
    void StartNewGame();
    void PlayerHit();
    void PlayerStand();
    void DealerPlay();
    int CalculateHandValue(const std::vector<Card>& hand) const;
    bool IsGameOver() const;
    std::string GetWinner() const;
    const std::vector<Card>& GetPlayerHand() const;
    const std::vector<Card>& GetDealerHand() const;
    bool PlaceBet(int amount);
    void ResolveBet();
    void FlipDealerSecondCard();
};

#endif // GAME_H

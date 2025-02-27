#include "blackjackGame.h"
#include <algorithm>
#include <iostream>

blackjackGame::blackjackGame() : playerTurn(true), dealerSecondCardFaceUp(false), playerBalance(500), currentBet(0) {
    StartNewGame();
}

int blackjackGame::GetBalance() {
    return playerBalance;
}

void blackjackGame::setBalance(int newBalance) {
    playerBalance = newBalance;
}

void blackjackGame::StartNewGame() {
    deck = Deck();
    playerHand.clear();
    dealerHand.clear();
    playerHand.push_back(deck.DrawCard());
    playerHand.push_back(deck.DrawCard());
    dealerHand.push_back(deck.DrawCard());
    dealerHand.push_back(deck.DrawCard());
    dealerSecondCardFaceUp = false;
    playerTurn = true;
    currentBet = 0;
}

void blackjackGame::FlipDealerSecondCard() {
    dealerSecondCardFaceUp = true;
}

bool blackjackGame::PlaceBet(int amount) {
    if (amount <= 0) {
        std::cout << "Bet must be greater than 0." << std::endl;
        return false;
    }

    if (amount > playerBalance) {
        std::cout << "Not enough balance to place bet." << std::endl;
        return false;
    }

    currentBet = amount;
    playerBalance -= amount;
    return true;
}

void blackjackGame::ResolveBet() {
    std::string result = GetWinner();
    int playerScore = CalculateHandValue(playerHand);
    int dealerScore = CalculateHandValue(dealerHand);

    if (result == "Player Wins!") {
        std::cout << "Player wins the bet!" << std::endl;
        if (playerScore == 21 && playerHand.size() == 2) {
            // Blackjack bonus (typically 3:2 payout)
            playerBalance += static_cast<int>(currentBet * 2.5);  // 2.5x the bet for a blackjack
        } else {
            playerBalance += currentBet * 2;  // 2x the bet for a win
        }
    } else if (result == "Dealer Wins!") {
        std::cout << "Dealer wins the bet!" << std::endl;
    } else {
        std::cout << "It's a draw! The bet is returned." << std::endl;
        playerBalance += currentBet;  // Bet is returned on a draw
    }

    // Reset the current bet after resolving
    currentBet = 0;
    setBalance(playerBalance);
}

void blackjackGame::PlayerHit() {
    if (playerTurn) {
        playerHand.push_back(deck.DrawCard());
        int playerHandValue = CalculateHandValue(playerHand);
        if (playerHandValue > 21) {
            std::cout << "Player busts!" << std::endl;
            playerTurn = false; // Player busted
        }
    }
}

void blackjackGame::PlayerStand() {
    playerTurn = false;
}

void blackjackGame::DealerPlay() {
    // Dealer follows the rule to hit if their score is less than 17
    while (CalculateHandValue(dealerHand) < 17) {
        dealerHand.push_back(deck.DrawCard());
    }
}

int blackjackGame::CalculateHandValue(const std::vector<Card>& hand) const {
    int total = 0, aceCount = 0;
    for (const auto& card : hand) {
        total += card.value;
        if (card.rank == "ace") aceCount++;
    }
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    return total;
}

bool blackjackGame::IsGameOver() const {
    // Game is over if either player or dealer goes bust or if both players have finished their turns
    return !playerTurn || CalculateHandValue(playerHand) > 21 || CalculateHandValue(dealerHand) > 21;
}

std::string blackjackGame::GetWinner() const {
    int playerScore = CalculateHandValue(playerHand);
    int dealerScore = CalculateHandValue(dealerHand);

    if (playerScore > 21) return "Dealer Wins!";  // Player busts
    if (dealerScore > 21) return "Player Wins!";  // Dealer busts
    if (playerScore > dealerScore) return "Player Wins!";
    if (dealerScore > playerScore) return "Dealer Wins!";
    return "It's a Draw!";
}

const std::vector<Card>& blackjackGame::GetPlayerHand() const {
    return playerHand;
}

const std::vector<Card>& blackjackGame::GetDealerHand() const {
    return dealerHand;
}

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <memory>
#include "GameMessage.hpp"
#include "GameServer.hpp"

class tcp_connection;

class Player {
public:
    enum State {
        LOBBY,
        IN_GAME,
        READY,
        BETTING,
        PLAYING
    };

    Player(int id, std::shared_ptr<tcp_connection> conn)
        : id_(id), connection_(conn), state_(LOBBY), balance_(1000) {}

    int getId() const { return id_; }
    State getState() const { return state_; }
    int getBalance() const { return balance_; }
    int getCurrentBet() const { return current_bet_; }
    int getTableId() const { return table_id_; }
    std::shared_ptr<tcp_connection> getConnection() const { return connection_; }

    void setState(State state) { state_ = state; }
    void setTableId(int id) { table_id_ = id; }
    void placeBet(int amount) {
        if (amount <= balance_) {
            balance_ -= amount;
            current_bet_ = amount;
        }
    }

    void addWinnings(int amount) {
        balance_ += amount;
        current_bet_ = 0;
    }

    void resetBet() {
        current_bet_ = 0;
    }

    void sendMessage(const std::string& msg);

private:
    int id_;
    std::shared_ptr<tcp_connection> connection_;
    State state_;
    int balance_;
    int current_bet_ = 0;
    int table_id_ = -1;
};

#endif 
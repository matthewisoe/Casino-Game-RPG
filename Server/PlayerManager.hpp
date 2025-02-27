#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include <map>
#include <vector>
#include "../Src/Player.h"
#include "GameServer.hpp"

class PlayerManager {
public:
    static PlayerManager& getInstance() {
        static PlayerManager instance;
        return instance;
    }

    int addPlayer(std::shared_ptr<tcp_connection> conn) {
        int new_id = next_player_id_++;
        players_[new_id] = std::make_shared<Player>(new_id, conn);
        players_[new_id]->setState(Player::LOGIN);
        return new_id;
    }

    void updatePlayerInfo(int player_id, const std::string& name, int skinId, float xPos, float yPos) {
        if (players_.find(player_id) != players_.end()) {
            players_[player_id]->setName(name);
            players_[player_id]->setSkinId(skinId);
            players_[player_id]->setXPos(xPos);
            players_[player_id]->setYPos(yPos);
        }
    }
    void updatePlayerPosition(int player_id, float xPos, float yPos){
        players_[player_id]->setXPos(xPos);
        players_[player_id]->setYPos(yPos);

    }

    void removePlayer(int id) {
        players_.erase(id);
    }

    std::shared_ptr<Player> getPlayer(int id) {
        auto it = players_.find(id);
        return it != players_.end() ? it->second : nullptr;
    }

    std::map<int, std::shared_ptr<Player>> getPlayers(){
        return players_;
    }

    void broadcastByState(Player::State target_state, const GameMessage& msg) {
        std::string serialized = msg.serialize() + "\n";
        for (const auto& [id, player] : players_) {
            if (player->getState() == target_state) {
                if (auto conn = player->getConnection()) {
                    conn->send(serialized);
                }
            }
        }
    }

    void broadcastByStateExcept(Player::State target_state, int exclude_player_id, const GameMessage& msg) {
        std::string serialized = msg.serialize() + "\n";
        for (const auto& [id, player] : players_) {
            if (id != exclude_player_id && player->getState() == target_state) {
                if (auto conn = player->getConnection()) {
                    conn->send(serialized);
                }
            }
        }
    }
private:
    PlayerManager() : next_player_id_(1) {}
    std::map<int, std::shared_ptr<Player>> players_;
    int next_player_id_;
};

#endif 
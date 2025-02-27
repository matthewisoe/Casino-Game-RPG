#ifndef GAME_MESSAGE_HPP
#define GAME_MESSAGE_HPP

#include <string>
#include <sstream>
#include <vector>
#include <map>

struct GameMessage {
    enum Type {
        LOGIN = 1,
        GAME_START = 2,
        GAME_ACTION = 3,
        CHAT = 4,
        LOGOUT = 5,
        JOIN_TABLE = 6,
        LEAVE_TABLE = 7,
        PLACE_BET = 8,
        GAME_STATE = 9,
        JOIN_LOBBY = 10,
        PLAYER_MOVE = 11
    };
    
    Type type;
    int player_id;
    std::string data;
    

    std::string serialize() const {
        std::stringstream ss;
        ss << static_cast<int>(type) << "|"
           << player_id << "|"
           << data;
        return ss.str();
    }
    

    static GameMessage deserialize(const std::string& msg) {
        GameMessage result;
        std::stringstream ss(msg);
        std::string item;
        

        std::getline(ss, item, '|');
        result.type = static_cast<Type>(std::stoi(item));
        

        std::getline(ss, item, '|');
        result.player_id = std::stoi(item);
        

        std::getline(ss, result.data);
        
        return result;
    }
};


struct GameState {
    int table_id;
    std::vector<int> players;
    std::map<int, int> bets;     // player_id -> bet_amount
    std::string game_phase;       // "waiting", "betting", "playing", "ended"
    
    std::string toString() const {
        std::stringstream ss;
        ss << "Table " << table_id << " [" << game_phase << "] Players: ";
        for (int player : players) {
            ss << player << " ";
        }
        ss << "Bets: ";
        for (const auto& bet : bets) {
            ss << "Player" << bet.first << ":" << bet.second << " ";
        }
        return ss.str();
    }
};

#endif 
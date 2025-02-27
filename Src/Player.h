#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include "../Server/GameMessage.hpp"
#include "../Server/GameServer.hpp"

class tcp_connection;

class Player {
public:
    enum State {
        LOGIN,
        LOBBY,
        IN_GAME,
        READY,
        BETTING,
        PLAYING
    };
    Player() = default;
    Player(int id, std::shared_ptr<tcp_connection> conn)
            : id(id), connection(conn), state(LOBBY), balance(1000) {}
    Player(const int id, const std::string& name, const int skinId) : id(id), name(name), skinId(skinId), balance(500){}

    //Constructors
    void setXPos(float x){
        this->x = x;
    }
    void setYPos(float y){
        this->y = y;
    }
    void setName(std::string name){
        this->name = name;
    }
    void setSkinId(int skinId){
        this->skinId = skinId;
    }
    void setBalance(int balance){
        this->balance = balance;
    }
    void setState(State new_state) {
        state = new_state;
    }

    //Getter
    int getId() const { return id; }
    std::string getName() const {return name;}
    int getSkinId() const { return skinId; }
    State getState() const { return state; }
    int getBalance() const { return balance; }
    std::shared_ptr<tcp_connection> getConnection() const { return connection; }
    //Constructors
    float getXPos(){
        return x;
    }
    float getYPos(){
        return y;
    }


private:
    int id = 0;
    std::string name;
    float x = 0.0f, y = 0.0f;
    int skinId = 0;
    int balance;
    std::shared_ptr<tcp_connection> connection;
    State state;
};



#endif // PLAYER_H

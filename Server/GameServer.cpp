#include "GameServer.hpp"
#include "PlayerManager.hpp"
#include "GameMessage.hpp"
#include <boost/asio.hpp>
#include <iostream>

void tcp_connection::start() {
    auto self = shared_from_this();

    int player_id = PlayerManager::getInstance().addPlayer(self);

    // send welcome message to the new player
    GameMessage welcome_msg;
    welcome_msg.type = GameMessage::LOGIN;
    welcome_msg.player_id = player_id;
    welcome_msg.data = "Welcome to the CUHKSZ Casino Server!" + std::to_string(player_id);

    std::string serialized = welcome_msg.serialize() + "\n";
    boost::asio::async_write(socket_, boost::asio::buffer(serialized),
                             [self](const boost::system::error_code& ec, std::size_t) {
                                 if (ec) {
                                     std::cerr << "Error sending login confirmation: " << ec.message() << std::endl;
                                 } else {
                                     std::cout << "Login confirmation sent to client." << std::endl;
                                 }
                             });



    start_read();
}

void tcp_connection::start_read() {
    auto self = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [self](const boost::system::error_code& ec, std::size_t length) {
                                if (!ec) {
                                    std::string received_data(self->data_, length);
                                    std::cout << "Received data: " << received_data << std::endl;

                                    try {
                                        GameMessage received_msg = GameMessage::deserialize(received_data);

                                        switch (received_msg.type) {
                                            case GameMessage::JOIN_LOBBY: {
                                                // Confirm the player has joined the lobby
                                                auto player = PlayerManager::getInstance().getPlayer(received_msg.player_id);
                                                if (player) {
                                                    player->setState(Player::State::LOBBY);
                                                    // Send existing players' information to the new player
                                                    for (auto& [id, existing_player] : PlayerManager::getInstance().getPlayers()) {
                                                        if (id != received_msg.player_id && existing_player->getState() == Player::State::LOBBY) {
                                                            GameMessage existing_player_msg;
                                                            existing_player_msg.type = GameMessage::JOIN_LOBBY;
                                                            existing_player_msg.player_id = id;
                                                            existing_player_msg.data = existing_player->getName() + " " +
                                                                                       std::to_string(existing_player->getSkinId()) + " " +
                                                                                       std::to_string(existing_player->getXPos()) + " " +
                                                                                       std::to_string(existing_player->getYPos());


                                                            std::cout << "Sending player data to new player: " << existing_player_msg.data << std::endl; // Debug log

                                                            self->send(existing_player_msg.serialize() + "\n");
                                                        }
                                                    }

                                                    std::istringstream iss(received_msg.data);
                                                    std::string name;
                                                    int skinId;
                                                    float xPos, yPos;
                                                    iss >> name >> skinId >> xPos >> yPos;

                                                    PlayerManager::getInstance().updatePlayerInfo(received_msg.player_id, name, skinId, xPos, yPos);

                                                    // Broadcast to all players that a new player has joined
                                                    GameMessage join_msg;
                                                    join_msg.type = GameMessage::JOIN_LOBBY;
                                                    join_msg.player_id = received_msg.player_id;
                                                    join_msg.data = received_msg.data; // Contains player name and skinId

                                                    PlayerManager::getInstance().broadcastByStateExcept(Player::State::LOBBY, join_msg.player_id, join_msg);

                                                    std::cout << "Broadcasted that player " << received_msg.player_id << " has joined the lobby." << std::endl;
                                                } else {
                                                    std::cerr << "Player with ID " << received_msg.player_id << " not found." << std::endl;
                                                }
                                                break;
                                            }
                                            case GameMessage::PLAYER_MOVE:{
                                                // Confirm the player has joined the lobby
                                                auto player = PlayerManager::getInstance().getPlayer(received_msg.player_id);
                                                if (player) {
                                                    // Broadcast to all players that a new player has joined
                                                    GameMessage move_msg;
                                                    move_msg.type = GameMessage::PLAYER_MOVE;
                                                    move_msg.player_id = received_msg.player_id;
                                                    move_msg.data = received_msg.data; // Contains direction and position

                                                    std::istringstream iss(move_msg.data);

                                                    std::string direction;
                                                    std::string name;
                                                    int skinId;
                                                    float x, y;
                                                    iss >> name >> skinId >> direction >> x >> y;
                                                    PlayerManager::getInstance().updatePlayerPosition(move_msg.player_id, x, y);

                                                    std::cout << "Updated player " << move_msg.player_id << " position" << "(" << x << "," << y << ")";

                                                    PlayerManager::getInstance().broadcastByStateExcept(Player::State::LOBBY, move_msg.player_id, move_msg);

                                                    std::cout << "Broadcasted that player " << received_msg.player_id << " has joined the lobby." << std::endl;
                                                } else {
                                                    std::cerr << "Player with ID " << received_msg.player_id << " not found." << std::endl;
                                                }
                                                break;
                                            }

                                            default:
                                                std::cerr << "Unknown message type received: " << received_msg.type << std::endl;
                                                break;
                                        }
                                    } catch (const std::exception& e) {
                                        std::cerr << "Error processing message: " << e.what() << std::endl;
                                    }

                                    self->start_read(); // Use `self` to call member functions
                                } else {
                                    std::cerr << "Error reading from client: " << ec.message() << std::endl;
                                }
                            });
}

void tcp_connection::send(const std::string& message) {
    auto self = shared_from_this();
    boost::asio::async_write(socket_, boost::asio::buffer(message),
        [self](const boost::system::error_code& ec, std::size_t) {
            if (ec) {
                std::cerr << "Error sending message: " << ec.message() << std::endl;
            }
        });
}

void tcp_server::start_accept() {
    auto new_connection = tcp_connection::create(io_context_);
    acceptor_.async_accept(new_connection->socket(),
        [this, new_connection](const boost::system::error_code& error) {
            if (!error) {
                std::cout << "New connection established!" << std::endl;
                new_connection->start();
            } else {
                std::cerr << "Error during connection: " << error.message() << std::endl;
            }
            start_accept();
        });

    std::cout << "Welcome to the Casino Server, Goodluck!" << std::endl;
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
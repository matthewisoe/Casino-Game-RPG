
#ifndef GAME_GAMECLIENT_H
#define GAME_GAMECLIENT_H

#include <iostream>
#include <boost/asio.hpp>
#include "../Server/GameMessage.hpp"

using boost::asio::ip::tcp;
class GameClient {
public:
    GameClient(boost::asio::io_context& io_context)
            : socket_(io_context), player_id_(-1) {
    }
    ~GameClient() {
        if (socket_.is_open()) {
            send_logout();
        }
    }

    void connect(const std::string& host, const std::string& port) {
        try {
            tcp::resolver resolver(socket_.get_executor());
            auto endpoints = resolver.resolve(host, port);

            std::cout << "Connecting to server..." << std::endl;
            boost::asio::connect(socket_, endpoints);
            std::cout << "Connected!" << std::endl;


            boost::asio::socket_base::keep_alive option(true);
            socket_.set_option(option);
        }
        catch (std::exception& e) {
            std::cerr << "Connection error: " << e.what() << std::endl;
            throw;
        }
    }

    void run() {
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == "quit") break;

            try {
                input = input.substr(input.find_first_not_of(" \t"));

                if (input.substr(0, 5) == "game ") {
                    std::string command = input.substr(5);
                    if (command.find("join") == 0) {
                        GameMessage msg;
                        msg.type = GameMessage::JOIN_TABLE;
                        msg.player_id = player_id_;
                        msg.data = command.substr(5);
                        send_message(msg);


                        receive_response();
                    }
                    else if (command.find("bet") == 0) {
                        GameMessage msg;
                        msg.type = GameMessage::PLACE_BET;
                        msg.player_id = player_id_;
                        msg.data = command.substr(4);
                        send_message(msg);
                        receive_response();
                    }
                    else if (command.find("leave") == 0) {
                        GameMessage msg;
                        msg.type = GameMessage::LEAVE_TABLE;
                        msg.player_id = player_id_;
                        send_message(msg);
                        receive_response();
                    }
                    else {
                        GameMessage msg;
                        msg.type = GameMessage::GAME_ACTION;
                        msg.player_id = player_id_;
                        msg.data = command;
                        send_message(msg);
                        receive_response();
                    }
                }
                else {

                    GameMessage msg;
                    msg.type = GameMessage::CHAT;
                    msg.player_id = player_id_;
                    msg.data = input;
                    send_message(msg);
                    receive_response();
                }

            } catch (const boost::system::system_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                break;
            }
        }


        send_logout();
    }

    int send_login() {
        GameMessage msg;
        msg.type = GameMessage::LOGIN;
        msg.player_id = -1;
        msg.data = "new_player";
        send_message(msg);


        try {
            boost::asio::streambuf buf;
            boost::asio::read_until(socket_, buf, "\n");

            std::string data;
            std::istream is(&buf);
            std::getline(is, data);

            std::cout << "Raw login response: " << data << std::endl;

            GameMessage response = GameMessage::deserialize(data);
            if (response.type == GameMessage::LOGIN) {
                player_id_ = response.player_id;
                std::cout << "Login successful. Player ID: " << player_id_ << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Login error: " << e.what() << std::endl;
            throw;
        }
        return player_id_;
    }

    void send_logout() {
        if (player_id_ != -1) {
            GameMessage msg;
            msg.type = GameMessage::LOGOUT;
            msg.player_id = player_id_;
            send_message(msg);
        }

        socket_.shutdown(tcp::socket::shutdown_both);
        socket_.close();
    }

    void send_message(const GameMessage& msg) {
        std::string serialized = msg.serialize() + "\n";
        boost::asio::write(socket_, boost::asio::buffer(serialized));
    }

    GameMessage receive_response() {
        boost::asio::streambuf buf;
        boost::asio::read_until(socket_, buf, "\n");

        std::string data;
        std::istream is(&buf);
        std::getline(is, data);

        std::cout << "Raw response: " << data << std::endl;

        GameMessage msg = GameMessage::deserialize(data);

        return msg;
    }

    bool has_response() {
        try {
            return socket_.available() > 0;
        } catch (const boost::system::system_error& e) {
            std::cerr << "Error checking response availability: " << e.what() << std::endl;
            return false;
        }
    }

private:
    tcp::socket socket_;
    int player_id_;
};

#endif //GAME_GAMECLIENT_H

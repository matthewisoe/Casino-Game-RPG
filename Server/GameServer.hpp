#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <iostream>

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    using pointer = std::shared_ptr<tcp_connection>;

    static pointer create(boost::asio::io_context& io_context) {
        return pointer(new tcp_connection(io_context));
    }

    boost::asio::ip::tcp::socket& socket() { return socket_; }

    void start();
    void start_read();  // Start reading data from the client
    void send(const std::string& message);

private:
    tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::string message_;
};

class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context)
        : io_context_(io_context),
          acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string("172.16.217.236"), 12902)) {
        start_accept();
    }

private:
    void start_accept();
    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error);

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif

#pragma once

#include "boost/asio.hpp"
#include <functional>
#include "QueryParser.h"

namespace ba = boost::asio;

struct JoinServer;


struct ClientSession : std::enable_shared_from_this<ClientSession> {
    using socket_t  =  ba::ip::tcp::socket;

    ClientSession(const ClientSession &) = delete;

    void operator=(const ClientSession &) = delete;

    explicit ClientSession(ba::io_service &service, JoinServer &server);

    ~ClientSession() { std::cout << "session terminate\n"; }

    socket_t &socket();

    void start();

    void stop();

private:
    void do_start();
    void on_read(const boost::system::error_code &err, size_t data_size);

    socket_t m_socket;
    static const size_t m_read_buffer_size{10};
    char m_read_buffer[m_read_buffer_size];
    bool m_started;
    JoinServer &m_server;
    QueryParser m_query_parser;
};

#pragma once

#include "boost/asio.hpp"
#include <functional>
#include <list>

namespace ba = boost::asio;

struct ClientSession;

struct JoinServer {
    using port_t = uint16_t;
    using client_t = std::shared_ptr<ClientSession>;

    explicit JoinServer(port_t port);

    JoinServer(const JoinServer &) = delete;
    void operator=(const JoinServer &) = delete;

    void run();

    void remove_client_session(const client_t &client_session);

private:
    void do_accept(client_t client_session, const boost::system::error_code &err);
    void do_stop(int signal_number);

    ba::io_service m_service;
    ba::ip::tcp::acceptor m_acceptor;
    port_t m_port;
    std::list<client_t> m_client_sessions;
};


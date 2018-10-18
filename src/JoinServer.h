#pragma once

#include "boost/asio.hpp"
#include <functional>

namespace ba = boost::asio;

struct JoinServer {
    using port_t = uint16_t;

    JoinServer(const JoinServer &) = delete;
    void operator=(const JoinServer &) = delete;

    explicit JoinServer(port_t port);

    void run();

private:
    ba::io_service        m_service;
    ba::ip::tcp::acceptor m_acceptor;
    port_t                m_port;
};


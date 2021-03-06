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

    explicit ClientSession(socket_t socket);

    ~ClientSession();

    void start();

    void stop();

private:
    using future_result_t = query_parser::future_result_t;

    void do_read();
    void do_check_result();
    void do_write(std::string result);

    void on_read(const boost::system::error_code &err, size_t data_size);
    void on_check_result();

    bool                           m_started;
    ba::streambuf                  m_read_buffer;
    future_result_t                m_future_result;
    socket_t                       m_socket;
    std::shared_ptr<ClientSession> m_self;
};

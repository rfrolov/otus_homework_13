#include "ClientSession.h"
#include "JoinServer.h"


ClientSession::ClientSession(ba::io_service &service, JoinServer &server) : m_socket{service}, m_started{false},
                                                                            m_server{server}, m_query_parser{} {
    std::cout << "session create\n";
}

ClientSession::socket_t &ClientSession::socket() {
    return m_socket;
}

void ClientSession::start() {
    m_started = true;
    do_start();
}

void ClientSession::stop() {
    if (!m_started) return;
    m_started = false;
    m_socket.close();
    m_server.remove_client_session(shared_from_this());

}

void ClientSession::do_start() {
    m_socket.async_read_some(ba::buffer(m_read_buffer), [this](const boost::system::error_code &err, size_t bytes) {
        on_read(err, bytes);
    });
}

void ClientSession::on_read(const boost::system::error_code &err, size_t data_size) {
    if (!m_started) {
        return;
    }

    if (err) {
        stop();
        return;
    }

    m_query_parser.add(std::move(std::string(&m_read_buffer[0], &m_read_buffer[data_size])));

    do_start();
}
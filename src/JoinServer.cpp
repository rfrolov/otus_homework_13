#include "JoinServer.h"
#include "ClientSession.h"

JoinServer::JoinServer(port_t port) :
        m_service{}
        , m_acceptor{m_service, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port)}
        , m_port{port}
        , m_client_sessions{} {
}

void JoinServer::run() {
    m_client_sessions.emplace_back(std::make_shared<ClientSession>(m_service, *this));
    {
        auto client_session = m_client_sessions.back();
        m_acceptor.async_accept(client_session->socket(), [this, client_session](const boost::system::error_code &err) {
            do_accept(client_session, err);
        });
    }
    ba::signal_set signals(m_service, SIGINT, SIGTERM);
    signals.async_wait(
            [this, &signals](const boost::system::error_code &error, int signal_number) { do_stop(signal_number); });
    m_service.run();
}


void JoinServer::do_stop(int signal_number) {
    m_service.stop();
    for (const auto &it:m_client_sessions) {
        it->stop();
    }
    m_client_sessions.clear();
}

void JoinServer::do_accept(client_t client_session, const boost::system::error_code &err) {
    if (!err) {
        client_session->start();
    }

    m_client_sessions.emplace_back(std::make_shared<ClientSession>(m_service, *this));

    auto new_client_session = m_client_sessions.back();
    m_acceptor.async_accept(new_client_session->socket(),
                            [this, new_client_session](const boost::system::error_code &err) {
                                do_accept(new_client_session, err);
                            });

}

void JoinServer::remove_client_session(const client_t &client_session) {
    auto it = std::find(m_client_sessions.begin(), m_client_sessions.end(), client_session);
    m_client_sessions.erase(it);
}
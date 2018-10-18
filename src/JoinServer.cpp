#include "JoinServer.h"

JoinServer::JoinServer(port_t port) : m_service{}, m_acceptor{m_service, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port)}, m_port{port} {
}

void JoinServer::run() {
//    m_clients.emplace_back(std::make_shared<Client>(*this, m_service));
//    auto client = m_clients.back();
//    m_acceptor.async_accept(client->socket(), [this, &client](const boost::system::error_code &err) { do_accept(client, err); });
//    m_service.run();
}


void JoinServer::stop() {
//    m_service.stop();
}

//void JoinServer::do_accept(const client_t &client, const boost::system::error_code &err) {
//    if (!err) {
//        client->start();
//    }
//
//    m_clients.emplace_back(std::make_shared<Client>(*this, m_service));
//    auto &new_client = m_clients.back();
//    m_acceptor.async_accept(new_client->socket(), [this, &new_client](const boost::system::error_code &err) {
//        handle_accept(new_client, err);
//    });
//}

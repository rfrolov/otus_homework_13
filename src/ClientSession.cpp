#include "ClientSession.h"
#include "JoinServer.h"

ClientSession::ClientSession(ba::io_service &service, JoinServer &server) :
        m_socket{service}
        , m_started{false}
        , m_server{server}
        , m_read_buffer{} {
    std::cout << "session create\n";
}

ClientSession::socket_t &ClientSession::socket() {
    return m_socket;
}

void ClientSession::start() {
    m_started = true;
    do_read();
}

void ClientSession::stop() {
    if (!m_started) return;
    m_started = false;
    m_socket.close();
    m_server.remove_client_session(shared_from_this());

}

void ClientSession::do_read() {
    ba::async_read_until(m_socket, m_read_buffer, '\n', [this](const boost::system::error_code &err, size_t bytes) {
        on_read(err, bytes);
    });
}

void ClientSession::on_read(const boost::system::error_code &err, size_t /*data_size*/) {
    if (!m_started) {
        return;
    }

    if (err) {
        stop();
        return;
    }

    std::istream is(&m_read_buffer);
    std::string  data;
    std::getline(is, data);

    query_parser::future_result_t db_future_result{};

    auto parser_result = query_parser::parse(data, db_future_result);
    if (!parser_result.empty()) {
        std::cout << "ERR " << parser_result << std::endl;
    } else {
        // TODO: Запилить асинхронное ожидание результата.
        bool        is_error;
        std::string result;
        std::tie(is_error, result) = db_future_result.get();
        std::cout << (is_error ? "ERR" : "OK") << (result.empty() ? "" : " " + result) << std::endl;
    }
    do_read(); // TODO: Сначала дождаться результата.
}
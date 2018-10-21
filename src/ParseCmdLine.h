#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>

struct ParseCmdLine {
    using port_t = uint16_t;

    explicit ParseCmdLine(int argc, char *argv[]) {
        long long port{0};

        if (argc != 2) {
            m_error = "Неверное число аргументов";
            return;
        }

        port = strtoll(argv[1], nullptr, 0);
        if (port < 1 || port > 65535) {
            m_error = "Неверный аргумент: port";
            return;
        }

        m_port     = static_cast<port_t>(port);
        m_is_valid = true;
    }

    bool is_valid() const { return m_is_valid; }
    port_t port() const { return m_port; }
    const std::string &error() const { return m_error; };

    ParseCmdLine() = delete;

private:
    bool        m_is_valid{};
    port_t      m_port{};
    std::string m_error{};
};
//
// Created by by Ricardo Barragán Cátedra, Oct-2022
//

#pragma once

#include <boost/asio.hpp>
#include <arcus-chat-network/server/tcp_connection.h>
#include <functional>
#include <optional>
#include <unordered_set>

namespace Arcus {
    namespace asio = boost::asio;

    enum class IPV {
        V4,
        V6
    };

    class TCPServer {
        using OnJoinHandler = std::function<void(TCPConnection::pointer)>;
        using OnLeaveHandler = std::function<void(TCPConnection::pointer)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        TCPServer(IPV ipv, int port);

        int Run();
        void Broadcast(const std::string& message);
    private:
        void startAccept();

    public:
        OnJoinHandler OnJoin;
        OnLeaveHandler OnLeave;
        OnClientMessageHandler OnClientMessage;

    private:
        IPV _ipVersion;
        int _port;

        asio::io_context _ioContext;
        asio::ip::tcp::acceptor  _acceptor;
        std::optional<asio::ip::tcp::socket> _socket;
        std::unordered_set<TCPConnection::pointer> _connections {};
    };
}



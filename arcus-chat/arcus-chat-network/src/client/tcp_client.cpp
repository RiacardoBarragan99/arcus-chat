//
// Created by by Ricardo Barragán Cátedra, Oct-2022
//
#include "arcus-chat-network/client/tcp_client.h"

namespace Arcus {

    TCPClient::TCPClient(const std::string &address, int port) : _socket(_ioContext) {
        asio::ip::tcp::resolver resolver {_ioContext};
        _endpoints = resolver.resolve(address, std::to_string(port));
    }

    void TCPClient::Run() {
        asio::async_connect(_socket, _endpoints, [this](boost::system::error_code ec, asio::ip::tcp::endpoint ep) {
            if (!ec)
                asyncRead();
        });

        _ioContext.run();
    }

    void TCPClient::Stop() {
        boost::system::error_code ec;
        _socket.close(ec);

        if (ec) {
            // process error
        }
    }

    void TCPClient::Post(const std::string &message) {
        bool queueIdle = _outgoingMessages.empty();
        _outgoingMessages.push(message);

        if (queueIdle) {
            asyncWrite();
        }
    }

    void TCPClient::asyncRead() {
        asio::async_read_until(_socket, _streamBuf, "\n", [this](boost::system::error_code ec, size_t bytesTransferred) {
           onRead(ec, bytesTransferred);
        });
    }

    void TCPClient::onRead(boost::system::error_code ec, size_t bytesTransferred) {
        if (ec) {
            Stop();
            return;
        }

        std::stringstream message;
        message << std::istream{&_streamBuf}.rdbuf();
        OnMessage(message.str());
        asyncRead();
    }

    void TCPClient::asyncWrite() {
        asio::async_write(_socket, asio::buffer(_outgoingMessages.front()), [this](boost::system::error_code ec, size_t bytesTransferred) {
            onWrite(ec, bytesTransferred);
        });
    }

    void TCPClient::onWrite(boost::system::error_code ec, size_t bytesTransferred) {
        if (ec) {
            Stop();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty()) {
            asyncWrite();
        }
    }
}
//
// Created by by Ricardo Barragán Cátedra, Oct-2022
//

#pragma once
#include <boost/asio.hpp>
#include <queue>

namespace Arcus {
    namespace asio = boost::asio;
    using MessageHandler = std::function<void(std::string)>;

    class TCPClient {
    public:
        TCPClient(const std::string& address, int port);

        void Run();
        void Stop();
        void Post(const std::string& message);

    private:
        void asyncRead();
        void onRead(boost::system::error_code ec, size_t bytesTransferred);
        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    public:
        MessageHandler OnMessage;

    private:
        asio::io_context _ioContext {};
        asio::ip::tcp::socket _socket;

        asio::ip::tcp::resolver::results_type _endpoints;

        asio::streambuf _streamBuf{65536};
        std::queue<std::string> _outgoingMessages {};
    };
}



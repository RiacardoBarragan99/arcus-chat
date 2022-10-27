//
// Created by by Ricardo Barragán Cátedra, Oct-2022
//

#include <arcus-chat-network/server/tcp_server.h>
#include <iostream>

int main(int argc, char* argv[]) {

    Arcus::TCPServer server {Arcus::IPV::V4, 1337};

    server.OnJoin = [](Arcus::TCPConnection::pointer server) {
        std::cout << "User has joined the server: " << server->GetUsername() << std::endl;
    };

    server.OnLeave = [](Arcus::TCPConnection::pointer server) {
        std::cout << "User has left the server: " << server->GetUsername() << std::endl;
    };

    server.OnClientMessage = [&server](const std::string& message) {
        // Parse the message
        // Do game server things

        // Send message to client
        server.Broadcast(message);
    };

    server.Run();

    return 0;
}
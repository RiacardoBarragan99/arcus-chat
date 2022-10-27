//
// Created by by Ricardo Barragán Cátedra, Oct-2022
//
#include <arcus-chat-network/client/tcp_client.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <filesystem>



int main(int argc, char* argv[]) {
    Arcus::TCPClient client {"localhost", 1337};

    client.OnMessage = [](const std::string& message) {
        std::string prefix("\\f:");
        if (!message.compare(0, prefix.size(), prefix)) {
            std::cout << "Receiving file.." << std::endl;
        }
        else {
            std::cout << message;
        }
    };

    std::thread t{[&client] () { client.Run(); }};

    while(true) {
        std::string message;
        getline(std::cin, message);

        if (message == "\\q") break;

        std::string prefix("\\f:");
        if (!message.compare(0, prefix.size(), prefix)) {
            std::filesystem::path filePath = { message.substr(3) };
            if (!std::filesystem::exists(filePath))
            {
                std::cerr << "File don't exist!" << std::endl;
                return 1;
            }
            std::cout << "Sending file to server..." << std::endl;
            std::ifstream input(message.substr(3), std::ios::binary);
            std::string buffer(std::istreambuf_iterator<char>(input), {});
            client.Post(message + buffer);
            input.close();
            std::cout << "File sent." << std::endl;
        }

        message += "\n";

        client.Post(message);
    }

    client.Stop();
    t.join();
    return 0;
}

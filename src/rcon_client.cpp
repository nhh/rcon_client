//
// Created by Niklas Hanft on 10.03.21.
//

#include <iostream>
#include "rcon_client.h"

void RconClient::Connect(std::string address) {
    connected_ = true;
    std::cout << "Connected..." << std::endl;
}

void RconClient::Disconnect() {
    std::cout << "Disconnected..." << std::endl;
}

void RconClient::Authenticate(std::string username, std::string password) {
    if(!connected_) {
        std::cout << "Not connected" << std::endl;
        return;
    }
    username = username;
    password = password;

    // Todo implement me
}

int main() {
    auto client = new RconClient();
    client->Connect("localhost");

    auto command = new RconCommand(std::string("Hallo"));
    auto responseValue = new RconResponseValue(std::string("Hallo"));
    auto auth = new RconAuth(std::string("Hallo"));
    auto authResponse = new RconAuthResponse(std::string("Hallo"));
    auto execCommand = new RconExecCommand(std::string("Hallo"));

    client->Authenticate(std::string("nhh"), std::string("test"));
    client->Disconnect();
};

RconCommand::RconCommand(const std::string &body) {
    // Timestamp as id
    auto p0 = std::chrono::system_clock::now().time_since_epoch();
    id_ = std::chrono::duration_cast<std::chrono::milliseconds>(p0).count();
    body_ = body;
    size_ = body.size() + 10; // Only the body is variable, the rest is always 10 bytes.
}
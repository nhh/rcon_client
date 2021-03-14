//
// Created by Niklas Hanft on 10.03.21.
//

#ifndef RCON_CLIENT_RCON_CLIENT_H
#define RCON_CLIENT_RCON_CLIENT_H

#include <string>
#include <ostream>

// Request and Responste structure:

// Field	Type	Value
// Size	32-bit little-endian Signed Integer	Varies, see below.
// ID	32-bit little-endian Signed Integer	Varies, see below.
// Type	32-bit little-endian Signed Integer	Varies, see below.
// Body	Null-terminated ASCII String	Varies, see below.
// Empty String	Null-terminated ASCII String	0x00

// Packet Size
// The packet size field is a 32-bit little endian integer, representing the length of the request in bytes.
// Note that the packet size field itself is not included when determining the size of the packet, so the value of this field is always 4 less than the packet's actual length. The minimum possible value for packet size is 10:

// Size	Containing
// 4 Bytes	ID Field
// 4 Bytes	Type Field
// At least 1 Byte	Packet body (potentially empty)
// 1 Bytes	Empty string terminator
// Since the only one of these values that can change in length is the body, an easy way to calculate the size of a packet is to find the byte-length of the packet body, then add 10 to it.
//
//The maximum possible value of packet size is 4096. If the response is too large to fit into one packet, it will be split and sent as multiple packets. See "Multiple-packet Responses" below for more information on how to determine when a packet is split.

// Packet ID
// The packet id field is a 32-bit little endian integer chosen by the client for each request.
// It may be set to any positive integer. When the server responds to the request, the response packet will have the same packet id as the original request
// (unless it is a failed SERVERDATA_AUTH_RESPONSE packet - see below.)
// It need not be unique, but if a unique packet id is assigned, it can be used to match incoming responses to their corresponding requests.

// # Packet Type
// The packet type field is a 32-bit little endian integer, which indicates the purpose of the packet. Its value will always be either 0, 2, or 3, depending on which of the following request/response types the packet represents:

// Empty String
// The end of a Source RCON Protocol packet is marked by an empty ASCIIZ string - essentially just 8 bits of 0. In languages that do not null-terminate strings automatically,
// you can simply write 16 bits of 0 immediately after the packet body (8 to terminate the packet body and 8 for the empty string afterwards).

// Usage: rcon_client << rcon_command

enum RconPacketType { SERVERDATA_AUTH = 0, SERVERDATA_AUTH_RESPONSE = 2, SERVERDATA_EXECCOMMAND = 2, SERVERDATA_RESPONSE_VALUE = 0 };

class RconCommand {

public:
    explicit RconCommand(const std::string &body);

private:
    int32_t size_; // Is this being calculated by the setter?
    int32_t id_; // This is also set automatically
    RconPacketType type_;
    std::string body_;
    std::string terminator_;
};

class RconAuth : public RconCommand { using RconCommand::RconCommand;

private:
    RconPacketType type_ = SERVERDATA_AUTH;
};

class RconAuthResponse : public RconCommand { using RconCommand::RconCommand;

private:
    RconPacketType type_ = SERVERDATA_AUTH_RESPONSE;
};

class RconExecCommand : public RconCommand { using RconCommand::RconCommand;

private:
    RconPacketType type_ = SERVERDATA_EXECCOMMAND;
};

class RconResponseValue : public RconCommand { using RconCommand::RconCommand;

private:
    RconPacketType type_ = SERVERDATA_RESPONSE_VALUE;
};

class RconClient {

public:
    void Connect(std::string address);
    void Disconnect();
    void Authenticate(std::string username, std::string password);

    // friend RconCommand &operator<<(RconCommand &command, RconClient &client);

private:
    bool connected_;
    std::string address;
    std::string username;
    std::string password;
};


#endif //RCON_CLIENT_RCON_CLIENT_H

#include "ntp.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "swmod.h"

#pragma comment(lib, "Ws2_32.lib")

#define NTP_SERVER "216.239.35.8"  // IP address of time.google.com
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define NTP_TIMESTAMP_DELTA 2208988800ull

time_t getNTPTime() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        cleanup();
    }

    SOCKET sockfd;
    sockaddr_in serv_addr;
    uint8_t buffer[NTP_PACKET_SIZE] = { 0 };

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        cleanup();
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NTP_PORT);

    if (InetPton(AF_INET, TEXT(NTP_SERVER), &serv_addr.sin_addr) != 1) {
        std::cerr << "Invalid address" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        cleanup();
    }

    // Set the first byte of the buffer to the NTP request mode
    buffer[0] = 0x1B;

    // Send the NTP packet
    if (sendto(sockfd, reinterpret_cast<const char*>(buffer), NTP_PACKET_SIZE, 0, reinterpret_cast<SOCKADDR*>(&serv_addr), sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Error sending request" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        cleanup();
    }

    // Receive the NTP response
    int bytes_received = recv(sockfd, reinterpret_cast<char*>(buffer), NTP_PACKET_SIZE, 0);
    if (bytes_received < NTP_PACKET_SIZE) {
        std::cerr << "Error receiving response" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        cleanup();
    }

    closesocket(sockfd);
    WSACleanup();

    // Extract the NTP timestamp (seconds since 1900)
    uint64_t ntpTime = ((uint64_t)buffer[40] << 24 | (uint64_t)buffer[41] << 16 |
        (uint64_t)buffer[42] << 8 | (uint64_t)buffer[43]) - NTP_TIMESTAMP_DELTA;

    // Convert NTP timestamp to UNIX timestamp (seconds since 1970)
    return (time_t)ntpTime;
}
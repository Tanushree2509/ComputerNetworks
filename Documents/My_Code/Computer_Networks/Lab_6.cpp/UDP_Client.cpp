#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsa;
    SOCKET clientSocket;
    sockaddr_in serverAddress;

    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    // Create UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (clientSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed." << endl;

        WSACleanup();
        return 1;
    }

    // Clear server address
    memset(&serverAddress, 0, sizeof(serverAddress));

    // Server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Server IP address
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout << "========================================" << endl;
    cout << "           UDP SOCKET CLIENT" << endl;
    cout << "========================================" << endl;

    cout << "UDP socket created successfully." << endl;
    cout << "Server IP: 127.0.0.1" << endl;
    cout << "Server Port: " << PORT << endl;
    cout << endl;

    // Input message
    string message;

    cout << "Enter a string in UPPERCASE: ";
    getline(cin, message);

    // Send message to server
    int bytesSent = sendto(
        clientSocket,
        message.c_str(),
        message.length(),
        0,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    );

    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Error sending message." << endl;

        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }

    cout << endl;
    cout << "Message sent to server: "
         << message << endl;

    // Receive response from server
    int serverAddressSize = sizeof(serverAddress);

    memset(buffer, 0, BUFFER_SIZE);

    int bytesReceived = recvfrom(
        clientSocket,
        buffer,
        BUFFER_SIZE - 1,
        0,
        (sockaddr*)&serverAddress,
        &serverAddressSize
    );

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "Error receiving response from server." << endl;

        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }

    buffer[bytesReceived] = '\0';

    cout << "Converted string received from server: "
         << buffer << endl;

    // Close socket
    closesocket(clientSocket);
    WSACleanup();

    cout << endl;
    cout << "Client socket closed." << endl;

    return 0;
}
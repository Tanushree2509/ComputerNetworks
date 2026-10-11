#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsa;
    SOCKET serverSocket;
    sockaddr_in serverAddress, clientAddress;

    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    // Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }

    // Clear address structures
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    // Set server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket,
             (sockaddr*)&serverAddress,
             sizeof(serverAddress)) == SOCKET_ERROR)
    {
        cout << "Bind failed." << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    cout << "========================================" << endl;
    cout << "          UDP SOCKET SERVER" << endl;
    cout << "========================================" << endl;

    cout << "Server started successfully." << endl;
    cout << "Waiting for message from client..." << endl;
    cout << "Port: " << PORT << endl;
    cout << endl;

    // Receive message from client
    int clientAddressSize = sizeof(clientAddress);

    int bytesReceived = recvfrom(
        serverSocket,
        buffer,
        BUFFER_SIZE - 1,
        0,
        (sockaddr*)&clientAddress,
        &clientAddressSize
    );

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "Error receiving message." << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    buffer[bytesReceived] = '\0';

    cout << "Message received from client: "
         << buffer << endl;

    // Convert uppercase to lowercase
    string message = buffer;

    for (char& ch : message)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            ch = ch + ('a' - 'A');
        }
    }

    cout << "Converted message: "
         << message << endl;

    // Send converted message back to client
    int bytesSent = sendto(
        serverSocket,
        message.c_str(),
        message.length(),
        0,
        (sockaddr*)&clientAddress,
        clientAddressSize
    );

    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Error sending response." << endl;
    }
    else
    {
        cout << "Converted message sent to client." << endl;
    }

    // Close server socket
    closesocket(serverSocket);
    WSACleanup();

    cout << endl;
    cout << "Server socket closed." << endl;

    return 0;
}
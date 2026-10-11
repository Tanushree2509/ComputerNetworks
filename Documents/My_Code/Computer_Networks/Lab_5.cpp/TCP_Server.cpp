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
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }

    // Clear address structures
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    // Server address
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

    // Listen for client
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        cout << "Listen failed." << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    cout << "========================================" << endl;
    cout << "          TCP CHAT SERVER" << endl;
    cout << "========================================" << endl;

    cout << "Server started successfully." << endl;
    cout << "Waiting for client connection..." << endl;
    cout << "Port: " << PORT << endl;
    cout << endl;

    // Accept client connection
    int clientAddressSize = sizeof(clientAddress);

    clientSocket = accept(
        serverSocket,
        (sockaddr*)&clientAddress,
        &clientAddressSize
    );

    if (clientSocket == INVALID_SOCKET)
    {
        cout << "Client connection failed." << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    cout << "Client connected successfully." << endl;
    cout << endl;

    // Chat loop
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        int bytesReceived = recv(
            clientSocket,
            buffer,
            BUFFER_SIZE - 1,
            0
        );

        if (bytesReceived <= 0)
        {
            cout << "Client disconnected." << endl;
            break;
        }

        buffer[bytesReceived] = '\0';

        cout << "Client: " << buffer << endl;

        // Check for exit
        if (strcmp(buffer, "bye") == 0)
        {
            cout << "Client ended the chat." << endl;
            break;
        }

        // Enter server reply
        string reply;

        cout << "Server: ";
        getline(cin, reply);

        // Send reply to client
        send(
            clientSocket,
            reply.c_str(),
            reply.length(),
            0
        );

        // Check for exit
        if (reply == "bye")
        {
            cout << "Chat ended." << endl;
            break;
        }
    }

    // Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    cout << endl;
    cout << "Server sockets closed." << endl;

    return 0;
}
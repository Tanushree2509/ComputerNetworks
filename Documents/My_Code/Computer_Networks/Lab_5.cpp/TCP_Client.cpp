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
    SOCKET clientSocket;
    sockaddr_in serverAddress;

    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    // Create TCP socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

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
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout << "========================================" << endl;
    cout << "          TCP CHAT CLIENT" << endl;
    cout << "========================================" << endl;

    cout << "TCP socket created successfully." << endl;
    cout << "Connecting to server..." << endl;

    // Connect to server
    if (connect(
            clientSocket,
            (sockaddr*)&serverAddress,
            sizeof(serverAddress)
        ) == SOCKET_ERROR)
    {
        cout << "Connection failed." << endl;

        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }

    cout << "Connected to server successfully." << endl;
    cout << endl;

    // Chat loop
    while (true)
    {
        string message;

        // Enter client message
        cout << "Client: ";
        getline(cin, message);

        // Send message to server
        send(
            clientSocket,
            message.c_str(),
            message.length(),
            0
        );

        // Check for exit
        if (message == "bye")
        {
            cout << "Chat ended." << endl;
            break;
        }

        // Receive server response
        memset(buffer, 0, BUFFER_SIZE);

        int bytesReceived = recv(
            clientSocket,
            buffer,
            BUFFER_SIZE - 1,
            0
        );

        if (bytesReceived <= 0)
        {
            cout << "Server disconnected." << endl;
            break;
        }

        buffer[bytesReceived] = '\0';

        cout << "Server: " << buffer << endl;

        // Check for exit
        if (strcmp(buffer, "bye") == 0)
        {
            cout << "Server ended the chat." << endl;
            break;
        }
    }

    // Close socket
    closesocket(clientSocket);
    WSACleanup();

    cout << endl;
    cout << "Client socket closed." << endl;

    return 0;
}
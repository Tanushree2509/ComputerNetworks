#include <iostream>
#include <string>
#include <sstream>
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

    // Start Winsock
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

    // Server address
    memset(&serverAddress, 0, sizeof(serverAddress));

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

    // Listen
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        cout << "Listen failed." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "========================================" << endl;
    cout << "       TCP MATH OPERATIONS SERVER" << endl;
    cout << "========================================" << endl;
    cout << "Server started successfully." << endl;
    cout << "Waiting for client connection..." << endl;
    cout << "Port: " << PORT << endl;
    cout << endl;

    // Accept client
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

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data
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

        string data = buffer;

        // Stop if client sends bye
        if (data == "bye")
        {
            cout << "Client ended the connection." << endl;
            break;
        }

        cout << "Received from client: " << data << endl;

        // Extract numbers and operator
        stringstream ss(data);

        double num1, num2;
        char operation;

        ss >> num1 >> operation >> num2;

        double result;
        bool valid = true;

        // Perform operation
        switch (operation)
        {
            case '+':
                result = num1 + num2;
                break;

            case '-':
                result = num1 - num2;
                break;

            case '*':
                result = num1 * num2;
                break;

            case '/':
                if (num2 == 0)
                {
                    valid = false;
                }
                else
                {
                    result = num1 / num2;
                }
                break;

            default:
                valid = false;
        }

        string response;

        if (valid)
        {
            response = "Result = " + to_string(result);
        }
        else
        {
            response = "Invalid operation or division by zero.";
        }

        cout << "Sending to client: " << response << endl;

        // Send result
        send(
            clientSocket,
            response.c_str(),
            response.length(),
            0
        );

        cout << endl;
    }

    // Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    cout << "Server sockets closed." << endl;

    return 0;
}
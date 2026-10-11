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

    // Start Winsock
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

    // Server address
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    cout << "========================================" << endl;
    cout << "       TCP MATH OPERATIONS CLIENT" << endl;
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

    while (true)
    {
        double num1, num2;
        char operation;

        cout << "Enter first number: ";
        cin >> num1;

        cout << "Enter operation (+, -, *, /): ";
        cin >> operation;

        cout << "Enter second number: ";
        cin >> num2;

        // Create message
        string message =
            to_string(num1) + " " +
            operation + " " +
            to_string(num2);

        // Send to server
        send(
            clientSocket,
            message.c_str(),
            message.length(),
            0
        );

        cout << endl;
        cout << "Sent to server: "
             << num1 << " "
             << operation << " "
             << num2 << endl;

        // Receive result
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

        cout << "Result received from server: "
             << buffer << endl;

        cout << endl;

        char choice;

        cout << "Do you want to perform another operation? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N')
        {
            string bye = "bye";

            send(
                clientSocket,
                bye.c_str(),
                bye.length(),
                0
            );

            break;
        }

        cout << endl;
    }

    // Close socket
    closesocket(clientSocket);
    WSACleanup();

    cout << endl;
    cout << "Client socket closed." << endl;

    return 0;
}
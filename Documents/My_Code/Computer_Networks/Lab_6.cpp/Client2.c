
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#define PORT 8080
#define MAXLINE 1024

int main(void)
{
    SOCKET sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    int n;

#ifdef _WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return EXIT_FAILURE;
    }
#endif

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");

#ifdef _WIN32
        WSACleanup();
#endif

        return EXIT_FAILURE;
    }

    // Configure server address
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get string from user
    printf("Enter a string: ");

    if (fgets(buffer, MAXLINE, stdin) == NULL)
    {
        printf("Input failed.\n");
        closesocket(sockfd);

#ifdef _WIN32
        WSACleanup();
#endif

        return EXIT_FAILURE;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    // Send string to server
    if (sendto(sockfd, buffer, (int)strlen(buffer), 0,
               (struct sockaddr *)&servaddr,
               sizeof(servaddr)) == SOCKET_ERROR)
    {
        printf("Send failed.\n");
        closesocket(sockfd);

#ifdef _WIN32
        WSACleanup();
#endif

        return EXIT_FAILURE;
    }

    printf("String sent to server.\n");

    // Receive reversed string
#ifdef _WIN32
    int len = sizeof(servaddr);
#else
    socklen_t len = sizeof(servaddr);
#endif

    n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                 (struct sockaddr *)&servaddr, &len);

    if (n == SOCKET_ERROR)
    {
        printf("Receive failed.\n");
        closesocket(sockfd);

#ifdef _WIN32
        WSACleanup();
#endif

        return EXIT_FAILURE;
    }

    buffer[n] = '\0';

    printf("Reversed string from server: %s\n", buffer);

    // Close socket
    closesocket(sockfd);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}

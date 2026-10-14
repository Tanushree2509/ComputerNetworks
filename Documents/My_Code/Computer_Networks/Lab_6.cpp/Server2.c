
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define MAXLINE 1024

int main(void)
{
    SOCKET sockfd;
    char buffer[MAXLINE];
    char temp;

    struct sockaddr_in servaddr, cliaddr;

    int len;
    int n, i, j;
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed.\n");
        return EXIT_FAILURE;
    }

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == INVALID_SOCKET)
    {
        fprintf(stderr, "Socket creation failed: %d\n",
                WSAGetLastError());

        WSACleanup();
        return EXIT_FAILURE;
    }

    // Initialize address structures
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&servaddr,
             sizeof(servaddr)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Bind failed: %d\n",
                WSAGetLastError());

        closesocket(sockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("Server is waiting for client...\n");

    // Receive string from client
    len = sizeof(cliaddr);

    n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                 (struct sockaddr *)&cliaddr, &len);

    if (n == SOCKET_ERROR)
    {
        fprintf(stderr, "Receive failed: %d\n",
                WSAGetLastError());

        closesocket(sockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }

    buffer[n] = '\0';

    printf("Original string: %s\n", buffer);

    // Reverse the string
    i = 0;
    j = n - 1;

    while (i < j)
    {
        temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;

        i++;
        j--;
    }

    printf("Reversed string: %s\n", buffer);

    // Send reversed string to client
    if (sendto(sockfd, buffer, n, 0,
               (struct sockaddr *)&cliaddr, len) == SOCKET_ERROR)
    {
        fprintf(stderr, "Send failed: %d\n",
                WSAGetLastError());

        closesocket(sockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("Reversed string sent to client.\n");

    // Close socket and clean up Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}

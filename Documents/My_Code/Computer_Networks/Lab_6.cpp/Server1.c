#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int sockfd;
    char buffer[MAX];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("UDP Server is running...\n");
    printf("Waiting for client message...\n");

    // Receive string from client
    int n = recvfrom(sockfd, buffer, MAX - 1, 0,
                     (struct sockaddr *)&client_addr,
                     &client_len);

    if (n < 0)
    {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';

    printf("Original string: %s\n", buffer);

    // Reverse the string
    int start = 0;
    int end = strlen(buffer) - 1;

    while (start < end)
    {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;

        start++;
        end--;
    }

    // Send reversed string to client
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&client_addr, client_len);

    printf("Reversed string: %s\n", buffer);

    close(sockfd);

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #ifdef _WIN32
// #include <winsock2.h>
// #include <ws2tcpip.h>
// //#pragma comment(lib, "ws2_32.lib")
// #else
// #include <arpa/inet.h>
// #include <unistd.h>
// #endif

// #define PORT 8081
// #define BUFFER_SIZE 1024

// int main() {
// #ifdef _WIN32
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         printf("Failed to initialize Winsock.\n");
//         return 1;
//     }
// #endif

//     int udp_socket;
//     struct sockaddr_in server_addr;
//     char buffer[BUFFER_SIZE];
//     char response[BUFFER_SIZE];
//     socklen_t addr_len = sizeof(server_addr);

//     if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//         perror("UDP socket failed");
//         return -1;
//     }

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(PORT);

//     if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
//         perror("Invalid address");
//         return -1;
//     }

//     printf("Connected to the UDP server. Enter your commands : \n");

//     while (1) {
//         printf("> ");
//         fgets(buffer, BUFFER_SIZE, stdin);
//         sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);

//         int len = recvfrom(udp_socket, response, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
//         if (len > 0) {
//             response[len] = '\0';
//             printf("Response : %s\n", response);
//         }
//     }

// #ifdef _WIN32
//     closesocket(udp_socket);
//     WSACleanup();
// #else
//     close(udp_socket);
// #endif

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
#endif

    int udp_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket failed.");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    printf("Connected to the UDP server. Enter your commands :\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprime '\n' de fgets
        sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);

        int len = recvfrom(udp_socket, response, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (len > 0) {
            response[len] = '\0';
            printf("Response : %s\n", response);
        }
    }

#ifdef _WIN32
    closesocket(udp_socket);
    WSACleanup();
#else
    close(udp_socket);
#endif

    return 0;
}

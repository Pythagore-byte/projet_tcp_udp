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

// typedef struct {
//     int id_compte;
//     char password[50];
//     float solde;
//     char operations[10][100];
//     int operation_index;
// } Compte;

// Compte comptes[10];
// int compte_count = 0;

// void init_comptes() {
//     comptes[0].id_compte = 1;
//     strcpy(comptes[0].password, "pass123");
//     comptes[0].solde = 1000.0;
//     comptes[0].operation_index = 0;

//     comptes[1].id_compte = 2;
//     strcpy(comptes[1].password, "pass456");
//     comptes[1].solde = 2000.0;
//     comptes[1].operation_index = 0;

//     compte_count = 2;
// }

// int verifier_compte(int id_compte, char *password) {
//     for (int i = 0; i < compte_count; i++) {
//         if (comptes[i].id_compte == id_compte && strcmp(comptes[i].password, password) == 0) {
//             return i;
//         }
//     }
//     return -1;
// }

// void traiter_commande(const char *buffer, char *response) {
//     char command[10], password[50];
//     int id_compte;
//     float somme;

//     if (sscanf(buffer, "%s %d %s %f", command, &id_compte, password, &somme) >= 3) {
//         int compte_index = verifier_compte(id_compte, password);
//         if (compte_index == -1) {
//             strcpy(response, "KO");
//         } else {
//             Compte *compte = &comptes[compte_index];
//             if (strcmp(command, "AJOUT") == 0) {
//                 compte->solde += somme;
//                 strcpy(response, "OK");
//             } else if (strcmp(command, "RETRAIT") == 0) {
//                 if (compte->solde >= somme) {
//                     compte->solde -= somme;
//                     strcpy(response, "OK");
//                 } else {
//                     strcpy(response, "KO");
//                 }
//             } else if (strcmp(command, "SOLDE") == 0) {
//                 snprintf(response, BUFFER_SIZE, "RES_SOLDE %.2f", compte->solde);
//             } else {
//                 strcpy(response, "KO");
//             }
//         }
//     } else {
//         strcpy(response, "KO");
//     }
// }

// int main() {
// #ifdef _WIN32
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         printf("Failed to initialize Winsock.\n");
//         return 1;
//     }
// #endif

//     int udp_socket;
//     struct sockaddr_in server_addr, client_addr;
//     char buffer[BUFFER_SIZE];
//     char response[BUFFER_SIZE];
//     socklen_t addr_len = sizeof(client_addr);

//     init_comptes();

//     if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//         perror("UDP socket failed");
//         return -1;
//     }

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(PORT);

//     if (bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
//         perror("Bind failed");
//         return -1;
//     }

//     printf("UDP server listening on port %d\n", PORT);

//     while (1) {
//         int len = recvfrom(udp_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
//         if (len > 0) {
//             buffer[len] = '\0';
//             printf("Request received: %s\n", buffer);
//             traiter_commande(buffer, response);
//             sendto(udp_socket, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
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

typedef struct {
    int id_compte;
    char password[50];
    float solde;
    char operations[10][100];
    int operation_index;
} Compte;

Compte comptes[10];
int compte_count = 0;

void init_comptes() {
    comptes[0].id_compte = 1;
    strcpy(comptes[0].password, "pass123");
    comptes[0].solde = 1000.0;
    comptes[0].operation_index = 0;

    comptes[1].id_compte = 2;
    strcpy(comptes[1].password, "pass456");
    comptes[1].solde = 2000.0;
    comptes[1].operation_index = 0;

    compte_count = 2;
}

int verifier_compte(int id_compte, const char *password) {
    for (int i = 0; i < compte_count; i++) {
        if (comptes[i].id_compte == id_compte && strcmp(comptes[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

void ajouter_operation(Compte *compte, const char *operation) {
    snprintf(compte->operations[compte->operation_index], 100, "%s", operation);
    compte->operation_index = (compte->operation_index + 1) % 10;
}

void traiter_commande(const char *buffer, char *response) {
    char command[20], password[50];
    int id_compte;
    float somme;

    printf("Command received: %s\n", buffer);

    if (sscanf(buffer, "%s %d %s %f", command, &id_compte, password, &somme) >= 3 ||
        sscanf(buffer, "%s %d %s", command, &id_compte, password) >= 3) {
        int compte_index = verifier_compte(id_compte, password);
        if (compte_index == -1) {
            strcpy(response, "KO");
            printf("Error: Invalid account or password.\n");
        } else {
            Compte *compte = &comptes[compte_index];
            if (strcmp(command, "AJOUT") == 0) {
                compte->solde += somme;
                char operation[100];
                snprintf(operation, 100, "AJOUT %.2f", somme);
                ajouter_operation(compte, operation);
                snprintf(response, BUFFER_SIZE, "OK");
            } else if (strcmp(command, "RETRAIT") == 0) {
                if (compte->solde >= somme) {
                    compte->solde -= somme;
                    char operation[100];
                    snprintf(operation, 100, "RETRAIT %.2f", somme);
                    ajouter_operation(compte, operation);
                    snprintf(response, BUFFER_SIZE, "OK");
                } else {
                    strcpy(response, "KO");
                }
            } else if (strcmp(command, "SOLDE") == 0) {
                snprintf(response, BUFFER_SIZE, "RES_SOLDE %.2f", compte->solde);
            } else if (strcmp(command, "OPERATIONS") == 0) {
                snprintf(response, BUFFER_SIZE, "RES_OPERATIONS\n");
                int found = 0;
                for (int i = 0; i < 10; i++) {
                    if (strlen(compte->operations[i]) > 0) {
                        strcat(response, compte->operations[i]);
                        strcat(response, "\n");
                        found = 1;
                    }
                }
                if (!found) {
                    strcat(response, "No operation recorded.\n");
                }
            } else {
                strcpy(response, "KO");
            }
        }
    } else {
        strcpy(response, "KO");
        printf("Error: Malformed command.\n");
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
#endif

    int udp_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    init_comptes();

    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed.");
        return -1;
    }

    printf("UDP server listening on port %d\n", PORT);

    while (1) {
        int len = recvfrom(udp_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (len > 0) {
            buffer[len] = '\0';
            traiter_commande(buffer, response);
            sendto(udp_socket, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
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

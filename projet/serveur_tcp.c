#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <pthread.h>
#endif

#define PORT 8080
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

int verifier_compte(int id_compte, char *password) {
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

#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID socket_desc) {
#else
void *handle_client(void *socket_desc) {
#endif
    int sock = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Request received : %s\n", buffer);

        char command[10], password[50];
        int id_compte;
        float somme;

        if (sscanf(buffer, "%s %d %s %f", command, &id_compte, password, &somme) >= 3) {
            int compte_index = verifier_compte(id_compte, password);
            if (compte_index == -1) {
                send(sock, "KO", strlen("KO"), 0);
                continue;
            }

            Compte *compte = &comptes[compte_index];
            if (strcmp(command, "AJOUT") == 0) {
                compte->solde += somme;
                char operation[100];
                snprintf(operation, 100, "AJOUT %.2f", somme);
                ajouter_operation(compte, operation);
                send(sock, "OK", strlen("OK"), 0);
            } else if (strcmp(command, "RETRAIT") == 0) {
                if (compte->solde >= somme) {
                    compte->solde -= somme;
                    char operation[100];
                    snprintf(operation, 100, "RETRAIT %.2f", somme);
                    ajouter_operation(compte, operation);
                    send(sock, "OK", strlen("OK"), 0);
                } else {
                    send(sock, "KO", strlen("KO"), 0);
                }
            } else if (strcmp(command, "SOLDE") == 0) {
                char response[BUFFER_SIZE];
                snprintf(response, BUFFER_SIZE, "RES_SOLDE %.2f", compte->solde);
                send(sock, response, strlen(response), 0);
            } else if (strcmp(command, "OPERATIONS") == 0) {
                char response[BUFFER_SIZE] = "RES_OPERATIONS\n";
                for (int i = 0; i < 10; i++) {
                    if (strlen(compte->operations[i]) > 0) {
                        strcat(response, compte->operations[i]);
                        strcat(response, "\n");
                    }
                }
                send(sock, response, strlen(response), 0);
            } else {
                send(sock, "KO", strlen("KO"), 0);
            }
        } else {
            send(sock, "KO", strlen("KO"), 0);
        }
    }

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    free(socket_desc);
#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }
#endif

    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    init_comptes();

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP server listening on port %d\n", PORT);

    while ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len))) {
        int *new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
#ifdef _WIN32
        HANDLE thread_id = CreateThread(NULL, 0, handle_client, (void *)new_sock, 0, NULL);
        if (thread_id == NULL) {
            perror("Thread failed.");
            free(new_sock);
        }
#else
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) < 0) {
            perror("Thread failed");
            free(new_sock);
        }
#endif
    }

    if (new_socket < 0) {
        perror("Accept failed.");
        exit(EXIT_FAILURE);
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}

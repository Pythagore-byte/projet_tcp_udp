# # Définitions générales
# CC = gcc
# CFLAGS = -Wall -Wextra
# TARGETS = serveur client

# # Drapeaux spécifiques selon le système d'exploitation
# ifeq ($(OS),Windows_NT)
#     LIBS = -lws2_32
# else
#     LIBS = -pthread
# endif

# all: $(TARGETS)

# serveur: serveur.c
# 	$(CC) $(CFLAGS) serveur.c -o serveur $(LIBS)

# client: client.c
# 	$(CC) $(CFLAGS) client.c -o client $(LIBS)

# clean:
# 	rm -f $(TARGETS)


# Définitions générales
CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = serveur_tcp client_tcp serveur_udp client_udp

# Bibliothèques spécifiques au système d'exploitation
ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
	RM = del /Q
else
    LIBS = -pthread
	RM = rm -f
endif

all: $(TARGETS)

serveur_tcp: serveur_tcp.c
	$(CC) $(CFLAGS) serveur_tcp.c -o serveur_tcp $(LIBS)

client_tcp: client_tcp.c
	$(CC) $(CFLAGS) client_tcp.c -o client_tcp $(LIBS)

serveur_udp: serveur_udp.c
	$(CC) $(CFLAGS) serveur_udp.c -o serveur_udp $(LIBS)

client_udp: client_udp.c
	$(CC) $(CFLAGS) client_udp.c -o client_udp $(LIBS)

clean:
	$(RM) $(RM) serveur_tcp.exe client_tcp.exe serveur_udp.exe client_udp.exe

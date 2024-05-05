#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Color
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

#define SERVER_IP "127.0.0.1"  // Assuming server is localhost
#define SERVER_PORT1 12460
#define SERVER_PORT2 22460
#define BUFFER_SIZE 1024

int main() {
	int client_socket1, client_socket2;
	struct sockaddr_in server_address1, server_address2;
	char buffer[BUFFER_SIZE];
	int bytes_sent, bytes_received;

	// Create socket for Port 10000
	client_socket1 = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket1 == -1) {
		perror(RED"Error creating socket 1");
		exit(EXIT_FAILURE);
	}

	// Initialize server address structure for Port 10000
	memset(&server_address1, 0, sizeof(server_address1));
	server_address1.sin_family = AF_INET;
	server_address1.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_address1.sin_port = htons(SERVER_PORT1);

	// Create socket for Port 20000
	client_socket2 = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket2 == -1) {
		perror(RED"Error creating socket 2");
		exit(EXIT_FAILURE);
	}

	// Initialize server address structure for Port 20000
	memset(&server_address2, 0, sizeof(server_address2));
	server_address2.sin_family = AF_INET;
	server_address2.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_address2.sin_port = htons(SERVER_PORT2);

	// Connect to server for Port 10000
	if (connect(client_socket1, (struct sockaddr*)&server_address1, sizeof(server_address1)) == -1) {
		perror(RED"Error connecting to server 1");
		exit(EXIT_FAILURE);
	}

	// Connect to server for Port 20000
	if (connect(client_socket2, (struct sockaddr*)&server_address2, sizeof(server_address2)) == -1) {
		perror(RED"Error connecting to server 2");
		exit(EXIT_FAILURE);
	}

	printf(GREEN"Connected to servers\n");

	// Receive name and surname from user
	char name_surname[BUFFER_SIZE];
	printf(BLUE"Enter your name and surname: ");
	fgets(name_surname, BUFFER_SIZE, stdin);
	// Remove newline character at the end
	name_surname[strcspn(name_surname, "\n")] = 0;

	// Send name and surname to server for Port 10000
	bytes_sent = send(client_socket1, name_surname, strlen(name_surname), 0);
	if (bytes_sent == -1) {
		perror(RED"Error sending data 1");
		exit(EXIT_FAILURE);
	}

	printf(BLUE"Name and surname sent to server\n");

	// Receive date of birth from user
	char dob[BUFFER_SIZE];
	printf(BLUE"Enter your date of birth (DDMMYYYY): ");
	fgets(dob, BUFFER_SIZE, stdin);
	// Remove newline character at the end
	dob[strcspn(dob, "\n")] = 0;

	// Send date of birth to server for Port 10000
	bytes_sent = send(client_socket1, dob, strlen(dob), 0);
	if (bytes_sent == -1) {
		perror(RED"Error sending data 1");
		exit(EXIT_FAILURE);
	}

	printf("Date of birth sent to server\n");

	bytes_received = recv(client_socket1, buffer, BUFFER_SIZE, 0);
	if (bytes_received == -1) {
		perror(RED"Error receiving data 1");
		exit(EXIT_FAILURE);
	}
	buffer[bytes_received] = '\0';  // Null-terminate the received data
	printf(GREEN"Server says: %s\n", buffer);

	// Receive "Bye" from client via Port 20000
	bytes_received = recv(client_socket2, buffer, BUFFER_SIZE, 0);
	if (bytes_received == -1) {
		perror(RED"Error receiving data 2");
		exit(EXIT_FAILURE);
	}
	buffer[bytes_received] = '\0';  // Null-terminate the received data
	printf(GREEN"Received Conveted Year from client via port %d: %s\n", SERVER_PORT2, buffer);

	// Send "Bye" to server via Port 20000
	bytes_sent = send(client_socket2, "Bye", strlen("Bye"), 0);
	if (bytes_sent == -1) {
		perror(RED"Error sending data 2");
		exit(EXIT_FAILURE);
	}

	printf("Sent" GREEN"\"Bye\"" "to server via port %d\n", SERVER_PORT2);

	// Close sockets
	close(client_socket1);
	close(client_socket2);

	return 0;
}

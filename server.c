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

#define SERVER_PORT1 12460
#define SERVER_PORT2 22460
#define BUFFER_SIZE 1024

int main() {
	int server_socket1, server_socket2;
	struct sockaddr_in server_address1, server_address2, client_address;
	socklen_t client_address_len;
	char buffer[BUFFER_SIZE];
	int bytes_received, bytes_sent;

	// Create socket for Port 10000
	server_socket1 = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket1 == -1) {
		perror(RED"Error creating socket 1");
		exit(EXIT_FAILURE);
	}

	// Initialize server address structure for Port 10000
	memset(&server_address1, 0, sizeof(server_address1));
	server_address1.sin_family = AF_INET;
	server_address1.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address1.sin_port = htons(SERVER_PORT1);

	// Create socket for Port 20000
	server_socket2 = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket2 == -1) {
		perror(RED"Error creating socket 2");
		exit(EXIT_FAILURE);
	}

	// Initialize server address structure for Port 20000
	memset(&server_address2, 0, sizeof(server_address2));
	server_address2.sin_family = AF_INET;
	server_address2.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address2.sin_port = htons(SERVER_PORT2);

	// Bind sockets to the addresses and ports
	if (bind(server_socket1, (struct sockaddr*)&server_address1, sizeof(server_address1)) == -1) {
		perror(RED"Error binding 1");
		exit(EXIT_FAILURE);
	}

	if (bind(server_socket2, (struct sockaddr*)&server_address2, sizeof(server_address2)) == -1) {
		perror(RED"Error binding 2");
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections for both sockets
	if (listen(server_socket1, 5) == -1) {
		perror(RED"Error listening 1");
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket2, 5) == -1) {
		perror(RED"Error listening 2");
		exit(EXIT_FAILURE);
	}

	printf(GREEN"Server listening on port %d and %d\n", SERVER_PORT1, SERVER_PORT2);

	// Accept incoming connection for Port 10000
	int client_socket1;
	client_address_len = sizeof(client_address);
	client_socket1 = accept(server_socket1, (struct sockaddr*)&client_address, &client_address_len);
	if (client_socket1 == -1) {
		perror(RED"Error accepting connection 1");
		exit(EXIT_FAILURE);
	}

	// Accept incoming connection for Port 20000
	int client_socket2;
	client_address_len = sizeof(client_address);
	client_socket2 = accept(server_socket2, (struct sockaddr*)&client_address, &client_address_len);
	if (client_socket2 == -1) {
		perror(RED"Error accepting connection 2");
		exit(EXIT_FAILURE);
	}

	printf(BLUE"Client connected to port %d\n", SERVER_PORT1);

	// Receive name and surname from client
	bytes_received = recv(client_socket1, buffer, BUFFER_SIZE, 0);
	if (bytes_received == -1) {
		perror(RED"Error receiving data 1");
		exit(EXIT_FAILURE);
	}
	printf(BLUE"Received name and surname from client: %s\n", buffer);

	// Receive Birthdate
	bytes_received = recv(client_socket1, buffer, BUFFER_SIZE, 0);

	// Extract last 4 digits of date of birth
	int year = atoi(buffer + 4); // Convert last 4 digits to integer
	year -= 543; // Subtract 543
	printf(BLUE"Received birthdate from client: %s\n", buffer);

	// Send confirmation to client via Port 10000
	bytes_sent = send(client_socket1, "Server Received", strlen("Server Received"), 0);
	if (bytes_sent == -1) {
		perror(RED"Error sending data 1");
		exit(EXIT_FAILURE);
	}
	printf(GREEN"Confirmation sent to client via port %d\n", SERVER_PORT1);

	// Convert the updated year back to string
	sprintf(buffer, "%d", year);

	// Send the updated year to client through Port 20000
	bytes_sent = send(client_socket2, buffer, strlen(buffer), 0);
	if (bytes_sent == -1) {
		perror(RED"Error sending data 2");
		exit(EXIT_FAILURE);
	}

	printf(GREEN"Year sent to client via port %d\n", SERVER_PORT2);

	// Receive "Bye" from client via Port 20000
	bytes_received = recv(client_socket2, buffer, BUFFER_SIZE, 0);
	if (bytes_received == -1) {
		perror(RED"Error receiving data 2");
		exit(EXIT_FAILURE);
	}
	buffer[bytes_received] = '\0';  // Null-terminate the received data
	printf(GREEN"Received from client via port %d: %s\n", SERVER_PORT2, buffer);

	// Close sockets
	close(client_socket1);
	close(server_socket1);
	close(server_socket2);

	return 0;
}

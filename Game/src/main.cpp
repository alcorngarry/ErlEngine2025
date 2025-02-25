#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread> // Include for threading support
#include <string>
#include <cstring>
#include "Engine.h"
#include "Game.h"
#include "commands/ToggleMenuCommand.h"
#include "commands/CloseWindowCommand.h"

#define DEFAULT_PORT "8080"

// Function to start the server and handle HTTP requests
void start_server()
{
    SOCKET listenSocket, clientSocket = INVALID_SOCKET;
    WSADATA wsaData;
    struct addrinfo* result = NULL, hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return;
    }

    // Set up the hints struct
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE; // Use my IP address

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return;
    }

    // Create a SOCKET for the server to listen for client connections
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Bind the socket to the local address and port
    // Here we use inet_pton to convert IP address from string to network byte order
    const char* local_ip = "192.168.0.15"; // Replace with your actual local network IP

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    if (inet_pton(AF_INET, local_ip, &sa.sin_addr) <= 0) {
        std::cerr << "Invalid IP address format" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    sa.sin_port = htons(8080); // Port 8080

    iResult = bind(listenSocket, (struct sockaddr*)&sa, sizeof(sa));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::cout << "Waiting for incoming connections on " << local_ip << ":8080..." << std::endl;

    // Accept a client socket
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::cout << "Client connected!" << std::endl;

    // Receive HTTP request (for demonstration, we're assuming the request is minimal)
    char recvbuf[512];
    int recvResult = recv(clientSocket, recvbuf, sizeof(recvbuf), 0);
    if (recvResult > 0) {
        recvbuf[recvResult] = '\0'; // Null-terminate the received data
        std::cout << "Received request:\n" << recvbuf << std::endl;

        // Simple HTML content to return
        const char* httpResponse = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html>"
            "<head><title>Simple Server</title></head>"
            "<body>"
            "<h1>Welcome to the HTTP Server</h1>"
            "<p>This is a simple HTML response from the server.</p>"
            "</body>"
            "</html>";

        // Send HTTP response with HTML content
        iResult = send(clientSocket, httpResponse, (int)strlen(httpResponse), 0);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        }
    }
    else if (recvResult == 0) {
        std::cout << "Connection closed by client." << std::endl;
    }
    else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    // Close client and listen sockets
    closesocket(clientSocket);
    closesocket(listenSocket);

    // Clean up Winsock
    WSACleanup();
}


int main(int argc, char** argv)
{
    // Start the server in a separate thread
   // std::thread serverThread(start_server);

	GLFWwindow* window = Engine::start();
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	Game* game = new Game(window);
	game->init();

	InputManager::set_key_binding(GLFW_KEY_F9, new SaveCommand(game->Maps[game->level]));
	InputManager::set_key_binding(GLFW_KEY_M, new ToggleMenuCommand(game));
	InputManager::set_key_binding(GLFW_KEY_ESCAPE, new CloseWindowCommand(window));
	
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game->update(deltaTime);

		//do you need this?
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->render(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();

		InputManager::update();
	}

	Engine::shut_down(window);
	return 0;
}
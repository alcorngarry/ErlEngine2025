#include"Server.h"

#define DEFAULT_PORT "8080"
const char* m_page;

void Server::add_page(std::string page)
{
    m_page = page.c_str();
}

void Server::start_server()
{
    SOCKET listenSocket, clientSocket = INVALID_SOCKET;
    WSADATA wsaData;
    struct addrinfo* result = NULL, hints;
    int iResult;

    m_page = "HTTP/1.1 200 OK\r\n"
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

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE; // Use my IP address

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return;
    }

    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

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

    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::cout << "Waiting for incoming connections on " << local_ip << ":8080..." << std::endl;

    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::cout << "Client connected!" << std::endl;

    char recvbuf[512];
    int recvResult = recv(clientSocket, recvbuf, sizeof(recvbuf), 0);
    if (recvResult > 0) {
        recvbuf[recvResult] = '\0'; // Null-terminate the received data
        std::cout << "Received request:\n" << recvbuf << std::endl;

        iResult = send(clientSocket, m_page, (int)strlen(m_page), 0);
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

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();
}
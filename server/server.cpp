#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include<thread>
using namespace std;

void handleClient(int clientSocket,string ip,int port) {
    while(true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket,buffer,sizeof(buffer),0);
        if(bytesReceived < 0) {
            perror("Receive failed\n");
            break;
        }
        if(bytesReceived == 0) {
            cout << "Client disconnected\n";
            break;
        }
        cout << "Received from client ip : "<<ip<<" port : "<<port<<" "<<buffer<< endl;
    }
    close(clientSocket);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        cerr << "Socket creation failed\n";
        return 1;
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if(bind(serverSocket,(sockaddr*)&serverAddress,sizeof(serverAddress)) < 0) {
        cerr << "Bind failed\n";
        return 1;
    }
    if(listen(serverSocket, 5) < 0) {
        cerr << "Listen failed\n";
        return 1;
    }
    cout << "Server listening on port 8080...\n";
    while(true) {
    sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket,(sockaddr*)&clientAddress,&clientSize);
    if(clientSocket < 0) {
        cerr << "Accept failed\n";
        return 1;
    }
    cout << "Client connected!\n";
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&clientAddress.sin_addr,clientIP,INET_ADDRSTRLEN);
    int port = ntohs(clientAddress.sin_port);
    thread clientThread(handleClient,clientSocket,clientIP,port);
    clientThread.detach();
    }
    close(serverSocket);
    return 0;
}
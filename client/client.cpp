#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        cerr << "Socket creation failed\n";
        return 1;
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&serverAddress.sin_addr);
    if(connect(clientSocket,(sockaddr*)&serverAddress,sizeof(serverAddress)) < 0) {
        cerr << "Connection failed\n";
        return 1;
    }
    cout << "Connected to server!\n";
    while(true) {
        cout<<"Enter Message : ";
        string message;
        getline(cin, message);
        send(clientSocket,message.c_str(),message.size(),0);
    }
    close(clientSocket);
    return 0;
}
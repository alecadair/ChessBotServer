#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int socket_fd;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

int main(){
    
    if((socket_fd = socket(AF_INET,SOCK_STREAM, 0)) < 0){
        cout << "could not create socket" << endl;
    }
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
         cout << "Invalid address" << endl;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 8080;
    if(connect(socket_fd, (struct sockaddr*)&server_addr, 
                             sizeof(server_addr)) < 0){
        cout << "could not connect to server" << endl;
    }
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define SERVER_WRITE_PIPE 0
#define SERVER_READ_PIPE  1

#define READ_FD  0
#define WRITE_FD 1

#define SERVER_READ_FD  (pipes[SERVER_READ_PIPE][READ_FD])
#define SERVER_WRITE_FD (pipes[SERVER_WRITE_PIPE][WRITE_FD])

#define STOCKFISH_READ_FD   (pipes[SERVER_WRITE_PIPE][READ_FD])
#define STOCKFISH_WRITE_FD  (pipes[SERVER_READ_PIPE][WRITE_FD])

int pipes[2][2];
int socket_fd;
pid_t stockfish_pid;
struct sockaddr_in addr;

/*Handle ctrl-c to shut down stockfish on background thread*/
void SIGINT_handler(int sig_num){
    kill(stockfish_pid, SIGTERM);
    cout << "SIGTERM received and sent to stockfish" << endl;
    exit(EXIT_SUCCESS);
}

/*
 * Create, connect, and bind socket and return socket's fd for listenting on.
 */
int create_socket(pid_t stockfish_pid){
    if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) == 0){
        perror("socket failed");
    }
    int opt = 1;
    if(setsockopt(socket_fd,SOL_SOCKET,
                   SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))){
        perror("set sockopt failed");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    if(bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("socket bind failed");
    }
    if(listen(socket_fd,3) < 0){
        perror("socket listen failed");
    }
    int valread, new_socket;
    int addrlen = sizeof(addr);
    cout << "socket created" << endl;
    if((new_socket = accept(socket_fd,(struct sockaddr*) &addr, (socklen_t*) &addrlen)) < 0){
        perror("socket accept failed");
    }
    cout << "connection accepted killing stockfish" << endl;
    kill(stockfish_pid, SIGTERM);
    cout << "stockfish killed" << endl;
    // can now read from socket

}

void service_uci_command(){

}

int main(){
    //pid_t stockfish_pid;

    int server_pipefd[2], stockfish_pipefd[2];
   
    pipe(pipes[SERVER_READ_PIPE]);
    pipe(pipes[SERVER_WRITE_PIPE]);
    
    signal(SIGINT, SIGINT_handler);  

    if((stockfish_pid = fork())== 0){
        dup2(STOCKFISH_READ_FD, STDIN_FILENO);
        dup2(STOCKFISH_WRITE_FD, STDOUT_FILENO);
        
        close(STOCKFISH_READ_FD);
        close(STOCKFISH_WRITE_FD);
        close(SERVER_READ_FD);
        close(SERVER_WRITE_FD);

        execv("./stockfish",NULL);
    }else{
        close(STOCKFISH_READ_FD);
        close(STOCKFISH_WRITE_FD);

        //can now read and write to stockfish
        socket_fd = create_socket(stockfish_pid);
    }
    
    return 0;
}

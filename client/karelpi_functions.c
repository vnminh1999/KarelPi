// Header file
#include "karelpi.h"

// Socket libraries
#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
//#include <unistd.h>
#endif

// Other libraries
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Port and IP addr
#define SERVERNAME "192.168.4.1"
#define PORT 2300

// Pragma
#pragma comment(lib,"ws2_32.lib") // Winsock library
void ClearWinSock() {
    #ifdef __WIN32__
    WSACleanup();
    #endif
}

// response struct
#pragma pack(1)
typedef struct response_struct {
    uint32_t result;
} response;
#pragma pack()


// Headers for helper functions
int get_socket(int init);
int call_function(int sock, void * func_name, uint32_t len);
void close_socket(int sock, int exit_status);

// Header function implementation
void move() {
    // get socket and call function
    int sock = get_socket(1);
    int result = call_function(sock, "move", strlen("move"));

    // check if crash in wall
    if(!result) {
        printf("Karel crashed into a wall\n");
        close_socket(sock, EXIT_FAILURE);
    }
}

void turn_left() {
    int sock = get_socket(1);
    call_function(sock, "turn_left", strlen("turn_left"));
}

void turn_right() {
    int sock = get_socket(1);
    call_function(sock, "turn_right", strlen("turn_right"));
}

int wall_in_front() {
    int sock = get_socket(1);
    return call_function(sock, "wall_in_front", strlen("wall_in_front"));
}

int wall_to_left() {
    int sock = get_socket(1);
    return call_function(sock, "wall_to_left", strlen("wall_to_left"));
}

int wall_to_right() {
    int sock = get_socket(1);
    return call_function(sock, "wall_to_right", strlen("wall_to_right"));
}

int facing_north() {
    int sock = get_socket(1);
    return call_function(sock, "facing_north", strlen("facing_north"));
}

int facing_east() {
    int sock = get_socket(1);
    return call_function(sock, "facing_east", strlen("facing_east"));
}

int facing_south() {
    int sock = get_socket(1);
    return call_function(sock, "facing_south", strlen("facing_south"));
}

int facing_west() {
    int sock = get_socket(1);
    return call_function(sock, "facing_west", strlen("facing_west"));
}

void karel_setup() {
    get_socket(0);
}

void pause() {
    static int times = 0;
    printf("[%d] PAUSED - Press enter to continue", ++times);
    while(getchar() != '\n');
}
void turn_off() {
    int sock = get_socket(1);
    close_socket(sock, EXIT_SUCCESS);
}

// Helper method to get socket ID
int get_socket(int init) {
    // Start winsock if Windows
    #ifdef __WIN32__
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Cannot start WSA\n");
        exit(EXIT_FAILURE);
    }
    #endif

    static int sock;

    if(!init) {
        // Create socket
        if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            printf("Socket creation failed\n");
            close_socket(sock, EXIT_FAILURE);
        }

        // Get server address
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);
        #ifdef __WIN32__
        server_address.sin_addr.s_addr = inet_addr(SERVERNAME);
        #else
        inet_pton(AF_INET, SERVERNAME, &server_address.sin_addr);
        #endif

        // Connect to socket
        if (connect(sock, (struct sockaddr*)&server_address, 
            sizeof(server_address)) < 0) {
            printf("Unable to connect to server\n");
            close_socket(sock, EXIT_FAILURE);
        }

        init = 1;
    }

    return sock;
}

// Helper method to call server function
int call_function(int sock, void * func_name, uint32_t len) {
    #ifdef __WIN32__
    if (send(sock, func_name, len, 0) < 0) {
        printf("Can't call function: %s\n", (char*)func_name);
        close_socket(sock, EXIT_FAILURE);
    }
    #else
    if (write(sock, func_name, len) < 0) {
        printf("Can't call function: %s\n", (char*)func_name);
        close_socket(sock, EXIT_FAILURE);
    }
    #endif

    // create read buffer
    int BUFFSIZE = sizeof(response);
    char buff[BUFFSIZE];
    memset(buff, 0, BUFFSIZE);

    int read_size;
    #ifdef __WIN32__
    read_size = recv(sock, buff, BUFFSIZE, 0);
    #else
    read_size = read(sock, buff, BUFFSIZE);
    #endif

    // check read result error
    if(read_size <= 0) {
        printf("Can't receive result from function: %s\n", (char*)func_name);
        close_socket(sock, EXIT_FAILURE);
    }

    response * res = (response *)buff;
    int result = (int)res->result;
    return result;
}

// Helper method to close socket
void close_socket(int sock, int exit_status) {
    #ifdef __WIN32__
    closesocket(sock);
    #else
    close(sock);
    #endif
    ClearWinSock();
    exit(exit_status);
}
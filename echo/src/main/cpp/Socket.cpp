//
// Created by 薛祥清 on 2017/6/28.
//
#include <sys/types.h>
#include <sys/socket.h>

//sockaddr_un
#include <sys/un.h>
//htons,sockaddr_in
#include <netinet/in.h>
//inet_ntop
#include <arpa/inet.h>
//close,unlink
#include <unistd.h>
//offsetof
#include <stddef.h>
#include "LogUtil.cpp"

//############################ TCP ############################

//创建 socket
static int newTcpSocket(JNIEnv *env, jobject obj) {
    logMessage(env, obj, "Constructing a new TCP socket...");
    int tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (tcpSocket == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    }
    return tcpSocket;
}

//绑定端口
static int bindSocketToPort(JNIEnv *env, jobject obj, int sd, unsigned short port) {
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    address.sin_addr.s_addr = htonl(INADDR_ANY); //绑定所有地址
    address.sin_port = htons(port); //转化为网络编码 big-endian

    logMessage(env, obj, "Binding to port %hu.", port);
    if (bind(sd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
        return 0;
    }
    return 1;
}

static unsigned short getSocketPort(JNIEnv *env, jobject obj, int sd) {
    unsigned short port = 0;
    struct sockaddr_in address;
    socklen_t addressLen = sizeof(address);

    // 获取 socket 地址
    if (getsockname(sd, (struct sockaddr *) &address, &addressLen) == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    } else {
        port = ntohs(address.sin_port); //转化为主机编码
        logMessage(env, obj, "Binded to random port %hu.", port);
    }
    return port;
}

static void listenOnSocket(JNIEnv *env, jobject obj, int sd, int backlog) {
    logMessage(env, obj, "Listening on socket with a backlog of %d pending connections.", backlog);
    if (listen(sd, backlog) == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    }
}

static void logAddress(JNIEnv *env, jobject obj, const char *message, const struct sockaddr_in *address) {
    char ip[INET_ADDRSTRLEN];
    if (inet_ntop(PF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN) == NULL) { //ip转化
        throwErrnoException(env, IOEXCEPTION, errno);
    } else {
        unsigned short port = ntohs(address->sin_port);
        logMessage(env, obj, "%s %s:%hu.", message, ip, port);
    }
}

static int acceptOnSocket(JNIEnv *env, jobject obj, int sd) {
    struct sockaddr_in address;
    socklen_t addressLen = sizeof(address);

    logMessage(env, obj, "Warting for a client connection...");
    int clientSocket = accept(sd, (struct sockaddr *) &address, &addressLen);
    if (clientSocket == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    } else {
        logAddress(env, obj, "Client connection from ", &address);
    }
    return clientSocket;
}

static ssize_t receiverFromSocket(JNIEnv *env, jobject obj, int sd, char *buffer, size_t bufferSize) {
    logMessage(env, obj, "Receiving from the socket...");
    ssize_t recvSize = recv(sd, buffer, bufferSize - 1, 0);
    if (recvSize == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    } else {
        buffer[recvSize] = NULL;
        if (recvSize > 0) {
            logMessage(env, obj, "Received %d bytes: %s", recvSize, buffer);
        } else {
            logMessage(env, obj, "Client disconnected.");
        }
    }
    return recvSize;
}


static ssize_t sendToSocket(JNIEnv *env, jobject obj, int sd, char *buffer, size_t bufferSize) {
    logMessage(env, obj, "Sending to the socket...");
    ssize_t sendSize = send(sd, buffer, bufferSize, 0);
    if (sendSize == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    } else {
        buffer[sendSize] = NULL;
        if (sendSize > 0) {
            logMessage(env, obj, "Send %d bytes: %s", sendSize, buffer);
        } else {
            logMessage(env, obj, "Client disconnected.");
        }
    }
    return sendSize;
}

static void connectToAddress(JNIEnv* env, jobject obj, int sd, const char* ip, unsigned short port){
    logMessage(env, obj, "Connecting to %s:%hu...", ip, port);
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;
    //将 ip 转化为网络地址
    if(inet_aton(ip, &(address.sin_addr)) == 0){
         throwErrnoException(env, IOEXCEPTION, errno);
    }else{
        address.sin_port = htons(port);
        if(connect(sd, (const sockaddr*) &address, sizeof(address)) == -1){
            throwErrnoException(env, IOEXCEPTION, errno);
        }else{
            logMessage(env, obj, "Connected.");
        }
    }
}


//############################ UDP ############################

//创建 socket
static int newUdpSocket(JNIEnv *env, jobject obj) {
    logMessage(env, obj, "Constructing a new UDP socket...");
    int udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    }
    return udpSocket;
}


static ssize_t receiverDatagramFromSocket(JNIEnv *env, jobject obj, int sd, struct sockaddr_in *address, char *buffer,
                                          size_t bufferSize) {
    socklen_t addressLen = sizeof(struct sockaddr_in);
    logMessage(env, obj, "Receiving from the socket...");
    ssize_t recvSize = recvfrom(sd, buffer, bufferSize, 0, (struct sockaddr *) address, &addressLen);
    if (recvSize == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    } else {
        logMessage(env, obj, "Received from", address);
        buffer[recvSize] = NULL;
        if (recvSize > 0) {
            logMessage(env, obj, "Received %d bytes: %s", recvSize, buffer);
        }
    }
    return recvSize;
}


static ssize_t sendDatagramToSocket(JNIEnv *env, jobject obj, int sd, const struct sockaddr_in *address, char *buffer,
                                    size_t bufferSize) {
    logMessage(env, obj, "Sending to", address);
    ssize_t sendSize = sendto(sd, buffer, bufferSize, 0, (const sockaddr *) address, sizeof(struct sockaddr_in));
    if (sendSize == -1) {
        throwErrnoException(env, IOEXCEPTION, errno);
    } else if (sendSize > 0) {
        logMessage(env, obj, "Send %d bytes: %s", sendSize, buffer);
    }
    return sendSize;
}






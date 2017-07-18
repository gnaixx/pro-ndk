//
// Created by 薛祥清 on 2017/7/16.
//

#include "Socket.cpp"


extern "C" {

JNIEXPORT void JNICALL
Java_cc_gnaixx_echo_ServerActivity_nativeStartTcpServer(JNIEnv *env, jobject obj, jint port) {
    int serverSocket = newTcpSocket(env, obj);
    if (env->ExceptionOccurred() == NULL) {
        bindSocketToPort(env, obj, serverSocket, (unsigned short) port);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }
        if (port == 0) {
            getSocketPort(env, obj, serverSocket);
            if (env->ExceptionOccurred() != NULL) {
                goto exit;
            }
        }

        listenOnSocket(env, obj, serverSocket, 4);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        int clientSocket = acceptOnSocket(env, obj, serverSocket);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];
        ssize_t recvSize;
        ssize_t sendSize;

        while (1) {
            recvSize = receiverFromSocket(env, obj, clientSocket, buffer, MAX_BUFFER_SIZE);
            if (recvSize == 0 || env->ExceptionOccurred() != NULL) {
                break;
            }

            sendSize = sendToSocket(env, obj, clientSocket, buffer, (size_t) recvSize);
            if (sendSize == 0 || env->ExceptionOccurred() != NULL) {
                break;
            }
        }
        close(clientSocket);
    }

    exit:
    if (serverSocket > 0) {
        close(serverSocket);
    }
}


JNIEXPORT void JNICALL
Java_cc_gnaixx_echo_ServerActivity_nativeStartUdpServer(JNIEnv *env, jobject obj, jint port) {
    int serverSocket = newUdpSocket(env, obj);
    if (env->ExceptionOccurred() == NULL) {
        bindSocketToPort(env, obj, serverSocket, (unsigned short) port);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }
        if (port == 0) {
            getSocketPort(env, obj, serverSocket);
            if (env->ExceptionOccurred() != NULL) {
                goto exit;
            }
        }

        struct sockaddr_in address;
        memset(&address, 0, sizeof(address));

        char buffer[MAX_BUFFER_SIZE];
        ssize_t recvSize;
        ssize_t sendSize;

        while (1) {
            recvSize = receiverDatagramFromSocket(env, obj, serverSocket, &address, buffer, MAX_BUFFER_SIZE);
            if (recvSize == 0 || env->ExceptionOccurred() != NULL) {
                break;
            }

            sendSize = sendDatagramToSocket(env, obj, serverSocket, &address, buffer, (size_t) recvSize);
            if (sendSize == 0 || env->ExceptionOccurred() != NULL) {
                break;
            }
        }
    }

    exit:
    if (serverSocket > 0) {
        close(serverSocket);
    }
}


}


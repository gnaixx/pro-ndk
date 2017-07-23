//
// Created by 薛祥清 on 2017/7/18.
//

#include "Socket.cpp"


extern "C" {

JNIEXPORT void JNICALL
Java_cc_gnaixx_echo_LocalActivity_nativeStartLoacalServer(JNIEnv *env, jobject obj, jstring jname) {
    int serverSocket = newLocalSocket(env, obj);
    if (env->ExceptionOccurred() == NULL) {
        const char* cname = env->GetStringUTFChars(jname, NULL);
        if (cname == NULL) {
            goto exit;
        }

        bindLocalSocketToName(env, obj, serverSocket, cname);
        env->ReleaseStringUTFChars(jname, cname);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        listenOnSocket(env, obj, serverSocket, 4);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        int clientSocket = acceptOnLocalSocket(env, obj, serverSocket);
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
}
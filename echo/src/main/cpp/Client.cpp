//
// Created by 薛祥清 on 2017/7/16.
//

#include "Socket.cpp"

extern "C" {

JNIEXPORT void JNICALL
Java_cc_gnaixx_echo_ClientActivity_nativeStartTcpClient(JNIEnv *env, jobject obj, jstring jip, jint port, jstring jmsg) {
    static int clientSocket = 0;
    if(clientSocket == 0) {
        clientSocket = newTcpSocket(env, obj);
        if(env->ExceptionOccurred() == NULL) {
            const char* cip = env->GetStringUTFChars(jip, NULL);
            if(cip == NULL){
                goto exit;
            }
            connectToAddress(env, obj, clientSocket, cip, (unsigned short) port);
            env->ReleaseStringUTFChars(jip, cip);

            if(env->ExceptionOccurred() != NULL){
                goto exit;
            }
        }
    }
    if(env->ExceptionOccurred() == NULL){
        const char* cmsg = env->GetStringUTFChars(jmsg, NULL);
        if(cmsg == NULL){
            goto exit;
        }

        jsize msgSize = env->GetStringUTFLength(jmsg);
        sendToSocket(env, obj, clientSocket, (char *) cmsg, msgSize);
        env->ReleaseStringUTFChars(jmsg, cmsg);

        if(env->ExceptionOccurred() != NULL){
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];
        receiverFromSocket(env, obj, clientSocket, buffer, MAX_BUFFER_SIZE);
        return;
    }

    exit:
    if (clientSocket > 0) {
        close(clientSocket);
        clientSocket = 0;
    }
}

JNIEXPORT void JNICALL
Java_cc_gnaixx_echo_ClientActivity_nativeStartUdpClient(JNIEnv *env, jobject obj, jstring jip, jint port, jstring jmsg) {
    static int clientSocket = newUdpSocket(env, obj);

    if(env->ExceptionOccurred() == NULL){
        struct sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = PF_INET;

        const char* cip = env->GetStringUTFChars(jip, NULL);
        if(cip == NULL){
            goto exit;
        }
        int result = inet_aton(cip, &(address.sin_addr));
        env->ReleaseStringUTFChars(jip, cip);

        if(result == 0){
            throwErrnoException(env, IOEXCEPTION, errno);
            goto exit;
        }
        address.sin_port = htons(port);
        const char* cmsg = env->GetStringUTFChars(jmsg, NULL);
        if(cmsg == NULL){
            goto exit;
        }

        jsize msgSize = env->GetStringUTFLength(jmsg);

        sendDatagramToSocket(env, obj, clientSocket, &address, (char *) cmsg, msgSize);
        env->ReleaseStringUTFChars(jmsg, cmsg);

        if(env->ExceptionOccurred() != NULL){
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];
        receiverDatagramFromSocket(env, obj, clientSocket, &address, buffer, MAX_BUFFER_SIZE);
    }

    exit:
    if (clientSocket > 0) {
        close(clientSocket);
        clientSocket = 0;
    }
}

}
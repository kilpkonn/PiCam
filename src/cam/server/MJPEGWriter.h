/**
 * See @https://github.com/JPery/MJPEGWriter/blob/master/MJPEGWriter.h
 */

#include <unistd.h>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <csignal>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET    int
#define SOCKADDR    struct sockaddr
#define SOCKADDR_IN  struct sockaddr_in
#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
#define TIMEOUT_M       200000
#define NUM_CONNECTIONS 10

#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

struct clientFrame {
    uchar *outbuf;
    int outlen;
    int client;
};

struct clientPayload {
    void *context;
    clientFrame cf;
};

class MJPEGWriter {
    SOCKET sock;
    fd_set master;
    int timeout;
    int quality; // jpeg compression [1..100]
    std::vector<int> clients;
    pthread_t thread_listen, thread_write;
    pthread_mutex_t mutex_client = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_cout = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_writer = PTHREAD_MUTEX_INITIALIZER;
    Mat lastFrame;
    int port;

    static int _write(int sock, char *s, int len) {
        if (len < 1) { len = strlen(s); }
        {
            try {
                int retval = ::send(sock, s, len, 0x4000);
                return retval;
            }
            catch (int e) {
                cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
        }
        return -1;
    }

    /*int _read(int socket, char *buffer) {
        int result;
        result = recv(socket, buffer, 4096, MSG_PEEK);
        if (result < 0) {
            cout << "An exception occurred. Exception Nr. " << result << '\n';
            return result;
        }
        string s = buffer;
        buffer = (char *) s.substr(0, (int) result).c_str();
        return result;
    }*/

    static void *listenHelper(void *context) {
        ((MJPEGWriter *) context)->Listener();
        return nullptr;
    }

    static void *writerHelper(void *context) {
        ((MJPEGWriter *) context)->Writer();
        return nullptr;
    }

    static void *clientWriteHelper(void *payload) {
        void *ctx = ((clientPayload *) payload)->context;
        struct clientFrame cf = ((clientPayload *) payload)->cf;
        ((MJPEGWriter *) ctx)->ClientWrite(cf);
        return nullptr;
    }

public:
    explicit MJPEGWriter(int port = 0) :
            sock(INVALID_SOCKET),
            timeout(TIMEOUT_M),
            quality(90),
            port(port) {
        signal(SIGPIPE, SIG_IGN);
        FD_ZERO(&master);
        // if (port)
        //     open(port);
    }

    ~MJPEGWriter() {
        release();
    }

    bool release() {
        if (sock != INVALID_SOCKET)
            shutdown(sock, 2);
        sock = (INVALID_SOCKET);
        return false;
    }

    bool open() {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        SOCKADDR_IN address;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (::bind(sock, (SOCKADDR *) &address, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
            cerr << "error : couldn't bind sock " << sock << " to port " << port << "!" << endl;
            return release();
        }
        if (listen(sock, NUM_CONNECTIONS) == SOCKET_ERROR) {
            cerr << "error : couldn't listen on sock " << sock << " on port " << port << " !" << endl;
            return release();
        }
        FD_SET(sock, &master);
        return true;
    }

    bool isOpened() {
        return sock != INVALID_SOCKET;
    }

    void start() {
        pthread_mutex_lock(&mutex_writer);
        pthread_create(&thread_listen, nullptr, MJPEGWriter::listenHelper, this);
        pthread_create(&thread_write, nullptr, MJPEGWriter::writerHelper, this);
    }

    void stop() {
        std::cout << "Stopping server..." << std::endl;
        this->release();
        pthread_join(thread_listen, nullptr);
        pthread_join(thread_write, nullptr);
        std::cout << "Server shut down!" << std::endl;
    }

    void write(const Mat &frame) {
        pthread_mutex_lock(&mutex_writer);
        if (!frame.empty()) {
            lastFrame.release();
            std::cout << lastFrame << std::endl;
            lastFrame = frame.clone();
        }
        pthread_mutex_unlock(&mutex_writer);
    }

private:
    void Listener();

    void Writer();

    void ClientWrite(clientFrame &cf);
};
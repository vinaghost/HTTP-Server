#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"


class Server {  
    private:
        WSADATA wsaData;

        addrinfo hints, *result;

        SOCKET sck_server;
        SOCKET sck_client;
    public:
        Server();
        ~Server();

        void run();
        void stop();


        void startWSA();
        void getAddressinfo();
        void createSocket();
        void bindSocket();
        void listenSocket();

        SOCKET acceptSocket();
        std::string receiveSocket(SOCKET client);
        void sendSocket(SOCKET client, const std::string &data);
        void shutdownSocket(SOCKET client);

        std::string getContentFile(const std::string &filename);

    private:
        bool writeStrToClient(SOCKET socket, const std::string &str);
        bool writeDataToClient(SOCKET socket, const char *data, int datalen);

        std::string readFileBinary(const std::string &fileName);    
};
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"


class Server {  
    private:
        WSADATA                     wsaData;

        addrinfo                    hints;
        addrinfo                    *result;

        SOCKET                      server_sck;
        std::thread                 server_thread;

        bool stop_server;
        std::vector<SOCKET>         client_sck;
        std::mutex                  client_mutex;
        
    public:
        Server();
        ~Server();

        void run();
        void stop();


        void handleConnection();
        void handleClient(unsigned int id);

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
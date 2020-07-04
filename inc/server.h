#ifndef _SERVER_INC
#define _SERVER_INC

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 1024

////////////////////////////////////////////////////////////
/// \brief Xử lí các thông tin của request từ trình duyệt
///
////////////////////////////////////////////////////////////
class Server {  
    private:
        WSADATA                     wsaData;

        addrinfo                    hints;
        addrinfo                    *result;
        const PCSTR                 port;

        SOCKET                      server_sck;
        std::thread                 server_thread;

        bool                        stop_server;
        std::vector<SOCKET>         client_sck;
        std::mutex                  client_mutex;
        
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Tạo Server có port listen mặc định: 80.
        ///
        ////////////////////////////////////////////////////////////
        Server();

        ////////////////////////////////////////////////////////////
        /// \brief Tạo Server dựa trên port được chỉ định
        ///
        /// \param port port được chỉ định 
        ///
        ////////////////////////////////////////////////////////////
        Server(PCSTR port);

        ////////////////////////////////////////////////////////////
        /// \brief Default destructor 
        ///
        /// \see stop
        ///
        ////////////////////////////////////////////////////////////
        ~Server();

        ////////////////////////////////////////////////////////////
        /// \brief Thực thi server
        ///
        /// Tạo các theard để xử lí các kết nối từ trình duyệt 
        ///
        ////////////////////////////////////////////////////////////
        void run();

        private:
        ////////////////////////////////////////////////////////////
        /// \brief Dừng server
        ///
        /// Đóng socket server, shutdown các socket từ trình duyệt
        /// Dọn dẹp WSA 
        ///
        ////////////////////////////////////////////////////////////
        void stop();

        ////////////////////////////////////////////////////////////
        /// \brief Xử lí các kết nối tới server
        ///
        /// Tạo các theard để xử lí
        ///
        ////////////////////////////////////////////////////////////
        void handleConnection();

        ////////////////////////////////////////////////////////////
        /// \brief Xử lí các kết nối
        ///
        /// Tiếp nhận, xử lí và phản hồi các yêu cầu từ browser
        ///
        ////////////////////////////////////////////////////////////
        void handleClient(size_t id);

        ////////////////////////////////////////////////////////////
        /// \brief Khởi tạo ban đầu cho Windows Sockets API (WSA)
        ///
        ////////////////////////////////////////////////////////////
        void startWSA();

        ////////////////////////////////////////////////////////////
        /// \brief Chuyển địa chỉ từ dạng chuỗi ANSI sang
        ///
        /// địa chỉ API có thể hiểu được 
        ////////////////////////////////////////////////////////////        
        void getAddressinfo();

        ////////////////////////////////////////////////////////////
        /// \brief Khởi tạo socket của server và chỉ định giao thức cho socket
        /// 
        /// Loại địa chỉ: iPv4  
        /// Giao thức: TCP
        //////////////////////////////////////////////////////////// 
        void createSocket();

        ////////////////////////////////////////////////////////////
        /// \brief Ràng buộc socket của server với port xác định
        /// 
        //////////////////////////////////////////////////////////// 
        void bindSocket();

        ////////////////////////////////////////////////////////////
        /// \brief Đặt socket của server vào trạng thái sẵn sàng
        ///
        /// cho các kết nối tới server
        ///
        //////////////////////////////////////////////////////////// 
        void listenSocket();

        ////////////////////////////////////////////////////////////
        /// \brief Cấp phép cho kết nối tới socket của server
        ///
        /// \return         socket của kết nối tới
        ///
        //////////////////////////////////////////////////////////// 
        SOCKET acceptSocket();

        ////////////////////////////////////////////////////////////
        /// \brief Nhận thông điệp từ browser 
        ///
        /// \param  client  socket của kết nối từ browser
        ///
        /// \return         chuỗi thông điệp
        ///
        ////////////////////////////////////////////////////////////
        std::string receiveSocket(SOCKET client);

        ////////////////////////////////////////////////////////////
        /// \brief Gửi thông điệp tới browser 
        ///
        /// \param  client   socket của kết nối từ browser
        /// \param  data     thông điệp tới browser
        ///
        ////////////////////////////////////////////////////////////
        void sendSocket(SOCKET client, const std::string &data);

        ////////////////////////////////////////////////////////////
        /// \brief Ngừng socket
        ///
        /// \param  client   socket cần ngừng
        ///
        ////////////////////////////////////////////////////////////
        void shutdownSocket(SOCKET client);

        ////////////////////////////////////////////////////////////
        /// \brief Đọc nội dung file trong thư mục /html
        ///
        /// \param filename   tên file
        ///
        /// \return           nội dung file 
        /// 
        /// \see readFileBinary
        ///
        ////////////////////////////////////////////////////////////
        std::string getContentFile(const std::string &filename);

        ////////////////////////////////////////////////////////////
        /// \brief Đọc nội dung file
        ///
        /// Đọc nhị phân
        ///
        /// \param filename   tên file
        ///
        /// \return           nội dung file  
        ///
        ////////////////////////////////////////////////////////////
        std::string readFileBinary(const std::string &fileName);    

        ////////////////////////////////////////////////////////////
        /// \brief Gửi chuỗi đến browser
        ///
        /// \param  client   socket của kết nối từ browser cần gửi
        /// \param  str      chuỗi cần gửi
        ///
        /// \return          true nếu gửi thành công và ngược lại
        ///
        ////////////////////////////////////////////////////////////
        bool writeStrToClient(SOCKET socket, const std::string &str);

        ////////////////////////////////////////////////////////////
        /// \brief Gửi thông điệp đến browser
        ///
        /// \param  client   socket của kết nối từ browser cần gửi
        /// \param  data     thông điệp cần gửi
        /// \param  datalen  độ dài thông điệp cần gửi
        ///
        /// \return          true nếu gửi thành công ngược lại nếu thất bại
        ///
        ////////////////////////////////////////////////////////////
        bool writeDataToClient(SOCKET socket, const char *data, int datalen);
};

#endif
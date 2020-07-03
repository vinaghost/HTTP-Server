#ifndef _RESPONSE_INC
#define _RESPONSE_INC

#include <sstream>
#include "header.h" 
class Response : public Header {
    private:
        unsigned int code;
        std::stringstream ss;
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Đặt code response
        ///
        /// \param code     code của response         
        ///
        ////////////////////////////////////////////////////////////
        void set(unsigned int code);

        ////////////////////////////////////////////////////////////
        /// \brief Đặt code response và độ dài thông điệp
        ///
        /// \param code     code của response         
        /// \param size     độ dài của thông điệp
        ///
        ////////////////////////////////////////////////////////////
        void set(unsigned int code, unsigned long int size);

        ////////////////////////////////////////////////////////////
        /// \brief Đặt code response và thông điệp
        ///
        /// \param code     code của response         
        /// \param content     thông điệp
        ///
        ////////////////////////////////////////////////////////////
        void set(unsigned int code, const std::string& content);

        ////////////////////////////////////////////////////////////
        /// \brief Đặt lại code response
        ///
        ////////////////////////////////////////////////////////////
        void reset();

        ////////////////////////////////////////////////////////////
        /// \brief Lấy reponse hoàn chỉnh
        ///
        /// \return     chuỗi reponse hoàn chỉnh
        ///
        ////////////////////////////////////////////////////////////
        std::string getResponse();

        ////////////////////////////////////////////////////////////
        /// \brief Hiện thị toàn bộ dữ liệu
        ///
        ////////////////////////////////////////////////////////////
        void showData();
    private:
        ////////////////////////////////////////////////////////////
        /// \brief Tạo reponse hoàn chỉnh
        ///
        ////////////////////////////////////////////////////////////
        void createResponse();


};

#endif
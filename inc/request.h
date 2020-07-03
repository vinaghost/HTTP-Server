#ifndef _REQUEST_INC
#define _REQUEST_INC

#include "header.h"

////////////////////////////////////////////////////////////
/// \brief Xử lý các yêu cầu từ browser 
///
////////////////////////////////////////////////////////////
class Request : public Header {
    private:
        bool bGet;
        bool bPost;
        
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Nhận request từ browser và tự động phân tách
        ///
        /// \param req      request từ browser
        ///
        ////////////////////////////////////////////////////////////
        Request(const std::string &req);

        ////////////////////////////////////////////////////////////
        /// \brief Kiểm tra phương thức của request có phải là POST
        ///
        /// \return          true nếu là POST và ngược lại
        ///
        ////////////////////////////////////////////////////////////
        bool isPostMethod();

        ////////////////////////////////////////////////////////////
        /// \brief Kiểm tra phương thức của request có phải là GET
        ///
        /// \return          true nếu là GET và ngược lại
        ///
        ////////////////////////////////////////////////////////////
        bool isGetMethod();

        ////////////////////////////////////////////////////////////
        /// \brief Trả về header của request
        ///
        /// \return          map dữ liệu header request
        ///
        ////////////////////////////////////////////////////////////
        std::unordered_map<std::string, std::string> const &getHeader() const;

        ////////////////////////////////////////////////////////////
        /// \brief Trả về data của request
        ///
        /// \return          map dữ liệu data request
        ///
        ////////////////////////////////////////////////////////////
        std::unordered_map<std::string, std::string> const &getData() const;
        
        ////////////////////////////////////////////////////////////
        /// \brief Hiện thị toàn bộ dữ liệu
        ///
        ////////////////////////////////////////////////////////////
        void showData();

    private:
        ////////////////////////////////////////////////////////////
        /// \brief Phân tách dữ liệu của request
        ///
        /// Được lưu trữ dưới dạng map
        ///
        /// \param req      yêu cầu
        ///
        ////////////////////////////////////////////////////////////
        void parseData(const std::string &req);

        ////////////////////////////////////////////////////////////
        /// \brief Phân tách dữ liệu từ method của request
        ///
        /// Được lưu trữ dưới dạng map
        ///
        /// \param req      yêu cầu
        /// 
        /// \see parseData
        ///
        ////////////////////////////////////////////////////////////
        void parseMethod(const std::string &req);

};

#endif
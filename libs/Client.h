#include <cstdlib>

#include <algorithm>
#include <boost/asio.hpp>

#include "joinServer.h"

using namespace boost::asio;


class ClientSession : public std::enable_shared_from_this<ClientSession> {
    public:
        explicit ClientSession(JoinServer&, ip::tcp::socket);
        ~ClientSession() = default;
        void start();
    
    private:
        void handle_read();

        static constexpr size_t buf_len = 1024;
        JoinServer&                 m_assync;
        std::array<char, buf_len>   m_buf;
        ip::tcp::socket             m_sock;
        uint64_t                    m_handle{};
        std::shared_ptr<ClientSession>     m_self;
        
};
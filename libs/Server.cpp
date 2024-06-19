#include "Server.h"

Server::Server(uint16_t port) :
  m_acceptor(m_service, ip::tcp::endpoint(ip::tcp::v4(), port)),
  m_sock(m_service) {
}

Server::~Server(){}

void Server::handle_accept(){
    auto acceptProcessor = [this](const boost::system::error_code& ec) {
        if(!ec) {
            std::make_shared<ClientSession>(m_assync, std::move(m_sock))->start();
        }
        handle_accept();
    };
    m_acceptor.async_accept(m_sock, acceptProcessor);
}

void Server::start()
{
    signal_set signals{m_service, SIGINT, SIGTERM};
    signals.async_wait([this, &signals](const boost::system::error_code&, int) {
    handle_stop();
  });
  handle_accept();
  m_service.run();
}

void Server::handle_stop()
{
    m_service.stop();
}
#ifndef HTTP_SOCKET
#define HTTP_SOCKET

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class basic_socket
{
protected:
  int m_fd;

public:
  basic_socket()
  {
    if ((m_fd = ::socket(AF_INET, SOCK_STREAM, 0)) == 0)
      throw "basic_socket::socket";
  }

  basic_socket(int fd) : m_fd(fd)
  { }
};

class socket_server : public basic_socket
{
private:
  struct sockaddr_in m_addr;

public:
  socket_server(int port) : basic_socket()
  {
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_family      = AF_INET;
    m_addr.sin_port        = htons(port);
    if (::bind(m_fd, (struct sockaddr*)(&m_addr), sizeof(m_addr)) < 0)
      throw "socket_server::bind";
  }

  void listen(int num_connects)
  {
    if (::listen(m_fd, num_connects) < 0)
      throw "socket_server::listen";
  }

  int accept()
  {
    int fd;
    socklen_t addr_len = sizeof(m_addr);
    if ((fd = ::accept(m_fd, (struct sockaddr*)(&m_addr), &addr_len)) < 0)
      throw "socket_server::accept";
    return fd;
  }
};

class socket_client : public basic_socket
{
public:
  socket_client(int fd) : basic_socket(fd)
  { }

  void send(std::string msg)
  {
    ::send(m_fd, msg.c_str(), msg.length(), 0);
  }

  std::string recv()
  {
    std::string msg;

    int len = 32, recvlen = 0;
    char* buffer = new char[len];

    while ((recvlen = ::recv(m_fd, buffer, len, 0)) > 0)
    {
      msg += std::string(buffer, buffer + recvlen);
      if (recvlen < len)
        break;
    }

    return msg;
  }
};

#endif // HTTP_SOCKET

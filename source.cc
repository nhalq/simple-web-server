#include <iostream>
#include <thread>
using namespace std;

#include "socket.hh"

void create_and_send(socket_server* server)
{
  socket_client* client = new socket_client(server->accept());
  client->send("HTTP/1.1 200 Status OK\r\nContent-Length:12\r\n\r\nHello World!");
}

int main()
{
  socket_server server(8080);
  server.listen(1024);

  while (true)
  {
    thread t_cas(create_and_send, &server);
    t_cas.detach();
  }

  return 0;
}

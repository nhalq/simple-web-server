#include <iostream>
#include <thread>
using namespace std;

#include "socket.hh"

void create_and_send(socket_server* server)
{
  socket_client* client = new socket_client(server->accept());
  string msg = client->recv();
  string status = msg.substr(0, msg.find("\r\n"));
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

#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
using namespace std;

#include "socket.hh"

string get_url(string req)
{
  string status = req.substr(0, req.find("\r\n"));
  cout << status << "\n";

  int found_0 = status.find(' ');
  int found_1 = status.find(' ', ++found_0);
  return status.substr(found_0, found_1 - found_0);
}

int main()
{
  socket_server server(8080);
  server.listen(1024);

  while (true)
  {

    socket_client client(server.accept());
    string req = client.recv();
    string url = get_url(req);

    string msg = "404 not found";
    ifstream fin("./static" + url);
    if (fin.is_open())
    {
      msg = "";
      string buffer;
      while (getline(fin, buffer))
        msg += buffer + "\n";
      fin.close();
    }


    stringstream ss;
    ss << "HTTP/1.1 200 OK\r\n";
    ss << "Content-Length: " << msg.length() << "\r\n";
    ss << "Connection: Close\r\n";
    ss << "\r\n";
    ss << msg;

    client.send(ss.str());
  }

  return 0;
}

// https://blog.csdn.net/csfreebird/article/details/8502169

#include "Server.h"
#include "Connection.h"
#include "Connections.h"

int main(int argc, char** argv)
{
    io_service s;

    tcp::endpoint listen_endpoint(tcp::v4(), 8888);

    Server server(s, listen_endpoint);
    server.Run();
    return 0;
}

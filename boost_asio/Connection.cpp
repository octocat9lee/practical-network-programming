#include "Connection.h"
#include "Connections.h"
#include <boost/bind.hpp>
#include <iostream>


Connection::Connection(io_service& s, Connections& cons)
    : socket(s), read_buffer_(1, 0), cons_(cons)
{

}

Connection::~Connection()
{
    cout << "~Connection" << endl;
}

void Connection::StartWork()
{
    cout << "the new connection object is starting now." << endl;
    async_read(socket, buffer(read_buffer_),
        boost::bind(&Connection::AfterReadChar, shared_from_this(), _1));
}

void Connection::CloseSocket()
{
    cout << "closing the socket" << endl;
    socket.shutdown(tcp::socket::shutdown_both);
    socket.close();
}

void Connection::AfterReadChar(error_code const& ec)
{
    if(ec)
    {
        cout << ec.message() << endl;
        return;
    }

    char x = read_buffer_[0];
    if(x == 'a')
    {
        cout << "correct data received" << endl;
        async_read(socket, buffer(read_buffer_),
            boost::bind(&Connection::AfterReadChar, shared_from_this(), _1));
    }
    else
    {
        cout << "wrong data received, char is:" << (int)x << endl;
        CloseSocket();
        cons_.Remove(shared_from_this());
    }
}

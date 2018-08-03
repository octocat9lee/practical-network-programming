#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Connection.h"
#include "Connections.h"

using namespace boost;
using namespace boost::asio;
using ip::tcp;
using namespace std;
using boost::system::error_code;

class Server
{
public:

    Server(io_service & s, tcp::endpoint const& listen_endpoint)
        : io_(s), signals_(s), acceptor_(s, listen_endpoint)
    {
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
#if defined(SIGQUIT)
        signals_.add(SIGQUIT);
#endif
        signals_.async_wait(boost::bind(&Server::Stop, this));
        shared_ptr<Connection> c(new Connection(io_, cons_));

        acceptor_.async_accept(c->socket,
            boost::bind(&Server::AfterAccept, this, c, _1));
    }

    void Run()
    {
        io_.run();
    }

    void AfterAccept(shared_ptr<Connection>& c, error_code const& ec)
    {
        // Check whether the server was stopped by a signal before this completion
        // handler had a chance to run.
        if(!acceptor_.is_open())
        {
            return;
        }

        if(!ec)
        {
            cons_.Add(c);
            c->StartWork();
            shared_ptr<Connection> c2(new Connection(io_, cons_));
            acceptor_.async_accept(c2->socket,
                boost::bind(&Server::AfterAccept, this, c2, _1));
        }
    }

private:

    void Stop()
    {
        cout << "stopping" << endl;
        acceptor_.close();
        cons_.CloseAll();
        io_.stop();
    }

private:
    io_service& io_;
    boost::asio::signal_set signals_;
    tcp::acceptor acceptor_;
    Connections cons_;
};


#endif

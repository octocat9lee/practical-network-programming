#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::asio;
using ip::tcp;
using boost::system::error_code;

class Connection
{
public:
    Connection(io_service& s) : socket(s)
    {

    }

    ~Connection()
    {
        socket.close();
        cout << "~Connection" << endl;
    }

    void StartWork()
    {
        cout << "The new connection object is starting now." << endl;
    }

public:
    tcp::socket socket;
};

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
        shared_ptr<Connection> c(new Connection(io_));
        cout << "count1:" << c.use_count() << endl;
        acceptor_.async_accept(c->socket,
            boost::bind(&Server::AfterAccept, this, c, _1));
        cout << "count2:" << c.use_count() << endl;
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

        cout << "count3:" << c.use_count() << endl;

        if(!ec)
        {
            c->StartWork();

            shared_ptr<Connection> c2(new Connection(io_));

            acceptor_.async_accept(c2->socket,
                boost::bind(&Server::AfterAccept, this, c2, _1));
        }
    }

private:
    void Stop()
    {
        cout << "stop io_service" << endl;
        io_.stop();
    }

private:
    io_service& io_;
    boost::asio::signal_set signals_;
    tcp::acceptor acceptor_;
};

int main(int argc, char** argv)
{
    io_service s;

    tcp::endpoint listen_endpoint(tcp::v4(), 8888);

    Server server(s, listen_endpoint);
    server.Run();
    return 0;
}

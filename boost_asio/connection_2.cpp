//https://blog.csdn.net/csfreebird/article/details/8498557
//https://blog.csdn.net/csfreebird/article/details/8499339

#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace boost;
using namespace boost::asio;
using ip::tcp;

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
    Connection(io_service& s) :
        socket(s),
        read_buffer_(1, 0)
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
        // ERROR: 即便 shared_ptr 的引用计数为 0 导致析勾函数被调用，
        // 而 this 指针由于被 bind_t 保存，所以 asio 框架仍然能够调用
        // 到这个被删除的对象的成员函数
        async_read(socket, buffer(read_buffer_),
            boost::bind(&Connection::AfterReadChar, shared_from_this(), _1));
    }

    void AfterReadChar(const boost::system::error_code &ec)
    {
        if(ec)
        {
            cout << "error: " << ec.message() << endl;
            return;
        }

        char x = read_buffer_[0];
        if(x == 'a')
        {
            cout << "correct data received" << endl;
        }
        else
        {
            cout << "wrong data received, char is: " << (int)x << endl;
        }
    }

public:
    tcp::socket socket;

private:
    vector<char> read_buffer_;
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
        cout << "add SIGQUIT" << endl;
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

    void AfterAccept(shared_ptr<Connection>& c, const boost::system::error_code& ec)
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

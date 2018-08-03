#ifndef CONNECTION_H
#define	CONNECTION_H

#include <vector>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost;
using namespace boost::asio;
using ip::tcp;
using namespace std;
using boost::system::error_code;

class Connections;

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
    Connection(io_service& s, Connections& cons);

    ~Connection();

    void StartWork();

    void CloseSocket();

    void AfterReadChar(error_code const& ec);

public:
    tcp::socket socket;

private:
    vector<char> read_buffer_;
    Connections& cons_;
};


#endif

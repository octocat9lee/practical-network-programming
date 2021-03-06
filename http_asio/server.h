//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER2_SERVER_HPP
#define HTTP_SERVER2_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.h"
#include "io_service_pool.h"
#include "request_handler.h"

namespace http
{
namespace server2
{

/// The top-level class of the HTTP server.
class server
    : private boost::noncopyable
{
public:
    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit server(const std::string& address, const std::string& port,
        const std::string& doc_root, std::size_t io_service_pool_size);

    /// Run the server's io_service loop.
    void run();

    /// Stop the server.
    void stop();

private:
    /// Handle completion of an asynchronous accept operation.
    void handle_accept(const boost::system::error_code& e);

    /// The pool of io_service objects used to perform asynchronous operations.
    io_service_pool io_service_pool_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The next connection to be accepted.
    connection_ptr new_connection_;

    /// The handler for all incoming requests.
    request_handler request_handler_;
};

} // namespace server2
} // namespace http

#endif // HTTP_SERVER2_SERVER_HPP

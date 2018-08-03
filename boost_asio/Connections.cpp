#include "Connections.h"
#include <boost/bind.hpp>
#include <iostream>

void Connections::Add(shared_ptr<Connection>& con)
{
    values_.insert(con);
    cout << "the number of connections is: " << values_.size() << endl;
}

void Connections::Remove(shared_ptr<Connection> con)
{
    values_.erase(con);
}

void Connections::CloseAll()
{
    for_each(values_.begin(), values_.end(), boost::bind(&Connection::CloseSocket, _1));
    values_.clear();
}

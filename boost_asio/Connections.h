#ifndef CONNECTIONS_H
#define	CONNECTIONS_H

#include <set>
#include <algorithm>
#include <boost/shared_array.hpp>
#include "Connection.h"

using namespace std;
using namespace boost;

class Connections
{
public:
    void Add(shared_ptr<Connection>& con);

    void Remove(shared_ptr<Connection> con);

    void CloseAll();

private:
    set<shared_ptr<Connection> > values_;
};

#endif

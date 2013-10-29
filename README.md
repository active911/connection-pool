## connection-pool

Generic, efficient, thread safe connection pooling for C++

### Introduction

We needed a safe, efficient way to concurrently access MySQL without connection banging.  

### Features

- Fast
- Thread safe
- Generic (MySQL implementation using Connection/C++ is provided)
- Pre-opens arbitrary number of connections
- Used connections can be returned for immediate reuse
- Unreturned connections are automatically closed and replaced (thanks to shared_ptr reference counting)
- Apache 2.0 license


### Example
```cpp
#include <string>
#include <boost/shared_ptr.hpp>
#include "MySQLConnection.h"

// Create a pool of 5 MySQL connections
shared_ptr<MySQLConnectionFactory>mysql_connection_factory(new MySQLConnectionFactory("mysql_server","mysql_username","mysql_password"));
shared_ptr<ConnectionPool<MySQLConnection> >mysql_pool(new ConnectionPool<MySQLConnection>(5, mysql_connection_factory));


// Get a connection and do something with it (throws exception if pool is completely used)
shared_ptr<MySQLConnection> conn=mysql_pool->borrow();	
// conn->sql_connection->do_whatever();

/* If our code dies here, the connection will be closed and replaced with a new one! :) */

// Release for immediate reuse
mysql_pool->unborrow(conn);

```

### Design notes
Connections are stored as a std::deque of boost::shared_ptr so we can pop from the front and push from back; this makes sure all connections get cycled through as fast as possible (so we don't accidentally hang onto any dead ones for a long time).

We managed to get all of this WITHOUT a separate curator thread.  Calling borrow() should only block very briefly while we access the pool deque, etc.  If we have to replace a dead connection with a new one, borrow() will additionally block while the new connection is set up.  If we are still unable to serve a live connection, borrow() will throw.

The use of boost::shared_ptr for tracking connections yeilds some nice side effects. If you have a problem with a connection, just let the shared_ptr fall out of scope and the connection will automagically be closed (immediately) and then replaced with a brand new connection (later, when we need one).  In this way, it promotes 'safe' handling of connections without the need to manually ping() (or whatever) each connection periodically.

Debugging may be enabled by using the ```#define _DEBUG(x)``` macro:

```cpp
// Log to stdout
#define _DEBUG(x) 								\
	do { 										\
		std::cout << "  (" << x << ")" << endl;	\
	} while(0)

```


### Dependencies

- Boost
- Connection/C++ (for MySQL implementation)





#include "ConnectionPool.h"
#include <string>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <iostream>

using boost::shared_ptr;

namespace active911 {


	class MySQLConnection : public Connection {

	public:


		~MySQLConnection() {

			if(this->sql_connection) {

				std::cout << "MYSQL Destruct" << std::endl;

				this->sql_connection->close();
				this->sql_connection.reset(); 	// Release and destruct
				
			}

		};

		boost::shared_ptr<sql::Connection> sql_connection;
		int a;
	};


	class MySQLConnectionFactory : public ConnectionFactory {

	public:
		MySQLConnectionFactory(std::string server, std::string username, std::string password) {

			this->server=server;
			this->username=username;
			this->password=password;

		};

		// Any exceptions thrown here should be caught elsewhere
		shared_ptr<Connection> create() {

			// Get the driver
			sql::Driver *driver;
			driver=get_driver_instance();

			// Create the connection
			shared_ptr<MySQLConnection>conn(new MySQLConnection());

			// Connect
			conn->sql_connection=boost::shared_ptr<sql::Connection>(driver->connect(this->server,this->username,this->password));

			return boost::static_pointer_cast<Connection>(conn);
		};

	private:
		string server;
		string username;
		string password;
	};





}
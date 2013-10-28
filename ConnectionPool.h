
// class Connection
// class ConnectionFactory
// class ConnectionPool<T>
// struct ConnectionPoolStats
//
// ConnectionPool manages a connection pool of some kind.  Worker threads can ask for a connection, and must return it when done.
// Each connection is guaranteed to be healthy, happy, and free of disease.
//
// Connection and ConnectionFactory are virtual classes that should be overridden to their actual type.
//
// NOTE: To avoid using templates AND inheritance at the same time in the ConnectionFactory, ConnectionFactory::create must create a derved type 
// but return the base class. 	
 


#include <deque>
#include <set>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <exception>
using namespace std;
using boost::shared_ptr;

namespace active911 {


	struct ConnectionUnavailable : std::exception { 

		char const* what() const throw() {

			return "Unable to allocate connection";
		}; 
	};


	class Connection {

	public:
		Connection(){};
		virtual ~Connection(){};

	};

	class ConnectionFactory {

	public:
		virtual shared_ptr<Connection> create()=0;
	};

	struct ConnectionPoolStats {

		size_t pool_size;
		size_t borrowed_size;

	};

	template<class T>
	class ConnectionPool {

	public:

		ConnectionPoolStats get_stats() {

			// Lock
			boost::mutex::scoped_lock lock(this->io_mutex);

			// Get stats
			ConnectionPoolStats stats;
			stats.pool_size=this->pool.size();
			stats.borrowed_size=this->borrowed.size();			

			return stats;
		};

		ConnectionPool(size_t pool_size, shared_ptr<ConnectionFactory> factory){

			// Setup
			this->pool_size=pool_size;
			this->factory=factory;

			// Fill the pool
			while(this->pool.size() < this->pool_size){

				this->pool.push_back(this->factory->create());
			}


		};

		~ConnectionPool() {


		};

		/**
		 * Borrow
		 *
		 * Borrow a connection for temporary use
		 *
		 * When done, either (a) call unborrow() to return it, or (b) (if it's bad) just let it go out of scope.  This will cause it to automatically be replaced.
		 * @retval a shared_ptr to the connection object
		 */
		shared_ptr<T> borrow(){

			// Lock
			boost::mutex::scoped_lock lock(this->io_mutex);

			// Check for a free connection
			if(this->pool.size()==0){

				// Are there any crashed connections listed as "borrowed"?
				for(std::set<shared_ptr<Connection> >::iterator it=this->borrowed.begin(); it!=this->borrowed.end(); ++it){

					if((*it).unique()) {

						// This connection has been abandoned! Destroy it and create a new connection
						try {

							// If we are able to create a new connection, return it
							shared_ptr<Connection> conn=this->factory->create();
							this->borrowed.erase(it);
							this->borrowed.insert(conn);
							return boost::static_pointer_cast<T>(conn);

						} catch(std::exception& e) {

							// Error creating a replacement connection
							throw ConnectionUnavailable();
						}
					}
				}

				// Nothing available
				throw ConnectionUnavailable();
			}

			// Take one off the front
			shared_ptr<Connection>conn=this->pool.front();
			this->pool.pop_front();

			// Add it to the borrowed list
			this->borrowed.insert(conn);

			return boost::static_pointer_cast<T>(conn);
		};

		/**
		 * Unborrow a connection
		 *
		 * Only call this if you are returning a working connection.  If the connection was bad, just let it go out of scope (so the connection manager can replace it).
		 * @param the connection
		 */
		void unborrow(shared_ptr<T> conn) {

			// Lock
			boost::mutex::scoped_lock lock(this->io_mutex);

			// Push onto the pool
			this->pool.push_back(boost::static_pointer_cast<Connection>(conn));

			// Unborrow
			this->borrowed.erase(conn);

		};

	protected:
		shared_ptr<ConnectionFactory> factory;
		size_t pool_size;
		deque<shared_ptr<Connection> > pool;
		set<shared_ptr<Connection> > borrowed;
		boost::mutex io_mutex;

	};




}
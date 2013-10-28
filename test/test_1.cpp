#include <string>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include "../DummyConnection.h"

using namespace active911;
using namespace std;

int main(int argc, char **argv) {

	// Create a pool of 2 dummy connections
	boost::shared_ptr<DummyConnectionFactory>connection_factory(new DummyConnectionFactory());
	boost::shared_ptr<ConnectionPool<DummyConnection> >pool(new ConnectionPool<DummyConnection>(2, connection_factory));

	{

		// Get both available connections
		shared_ptr<DummyConnection> conn1=pool->borrow();		
		shared_ptr<DummyConnection> conn2=pool->borrow();		

		// Trying to get a third should throw
		bool exception_thrown=false;
		try {

			shared_ptr<DummyConnection> conn3=pool->borrow();		

		} catch (std::exception& e) {

			cout << "Exception thrown (intentional)" << endl;
			exception_thrown=true;
		}
		assert(exception_thrown);


		// Release one, and make sure it was repatriated (no exception)
		pool->unborrow(conn1);
		conn1=pool->borrow();		// Should not throw
	}

	// We never released pool 2, but it went out of scope.  2 connections should once again be available...
	shared_ptr<DummyConnection> conn1=pool->borrow();		
	shared_ptr<DummyConnection> conn2=pool->borrow();		


	return 0;
}


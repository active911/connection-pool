/* Copyright 2013 Active911 Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http: *www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ConnectionPool.h"
using boost::shared_ptr;

namespace active911 {


	class DummyConnection : public Connection {

	public:

		DummyConnection() {

			_DEBUG("Dummy connection created");

		};

		~DummyConnection() {

			// Destroy the connection

			_DEBUG("Dummy connection destroyed");

		};

		// shared_ptr to some kind of actual connection object would go here

	};


	class DummyConnectionFactory : public ConnectionFactory {

	public:
		DummyConnectionFactory() {


		};

		// Any exceptions thrown here should be caught elsewhere
		shared_ptr<Connection> create() {

			// Create the connection
			shared_ptr<DummyConnection>conn(new DummyConnection());

			// Perform some kind of ->connect operation here

			return boost::static_pointer_cast<Connection>(conn);
		};

	};





}
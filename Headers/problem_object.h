#ifndef _PROBLEM_OBJECT_H_
#define _PROBLEM_OBJECT_H_

#include <vector>
#include "point.h"

<<<<<<< HEAD
#include "../Utilities/JSON_parser/json_file.h"
=======
#include "../Utilities/JSON_parser/Headers/json_file.h"
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
#include "../Utilities/JSON_parser/Headers/json_array.h"
#include "../Utilities/JSON_parser/Headers/json_value.h"
#include "../Utilities/JSON_parser/Headers/json_object.h"
#include "../Utilities/JSON_parser/Headers/json_parser.h"

<<<<<<< HEAD

=======
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
using JSON::JsonArray;
using JSON::JsonValue;
using JSON::JsonObject;
using Utilities::Point;
using std::vector;

struct Connection {
	string name;
	Point source;
	Point sink;
};

struct Blocker {
	string name;
	Point location;
	unsigned int width;
	unsigned int height;
};

namespace Utilities {
	class ProblemObject {
		private:
			string name;
			unsigned int width;
			unsigned int height;
			vector<Connection> connections;
			vector<Blocker> blockers;

			void add_blocker(JsonObject* blocker);
			void add_connection(JsonObject* connection);
			int extract_int(JsonValue* int_value);

		public:
			/* Constructors */
			ProblemObject();
			ProblemObject(string filename);

			/* Destructor */
			~ProblemObject();

			/* Accessors */
			string get_name() { return this->name; }
			unsigned int get_height() { return this->height; }
			unsigned int get_width() { return this->width; }
			vector<Connection> get_connections() { return this->connections; }
			vector<Blocker> get_blockers() { return this->blockers; }

			/*
			Note: The Problem Objects should be created by JSON problem files, because of this
				We have made a conscious decision to not give any mutators. If you need to create
				a test case for a specific problem, please write a JSON file to parse
			*/

	};
}

<<<<<<< HEAD
#endif // _PROBLEM_OBJECT_H_
=======
#endif // _PROBLEM_OBJECT_H_
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81

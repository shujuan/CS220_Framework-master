<<<<<<< HEAD
=======

>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
#ifndef __JSON_PARSER_JSON_ARRAY_H_
#define __JSON_PARSER_JSON_ARRAY_H_

#include "json_value.h"

#include <vector>
#include <cstdio>

namespace JSON {
	class JsonArray {
		private:
			std::vector<JsonValue*> values;
		public:
			/* Constructors */

			/* Modifiers */
			void push_back(JsonValue* new_value) { values.push_back(new_value); }

			/* Accessors */
			unsigned size() { return values.size(); }
			bool empty() { return values.empty(); }
			JsonValue* at(unsigned index) {
				if (index >= values.size()) {
					return NULL;
				}
				return values.at(index);
			}
	};
}


#endif // __JSON_PARSER_JSON_ARRAY_H_

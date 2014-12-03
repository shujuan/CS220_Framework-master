#ifndef _NETLIST_BASE_H_
#define _NETLIST_BASE_H_

#include "segmentgroup.h"
#include <vector>

using std::vector;
using Utilities::Point;
using Utilities::PathSegment;

namespace Utilities {

	class Netlist: public SegmentGroup {

		private:
            Point source;
            vector<Point> sinks;

        public:
            Netlist();
            ~Netlist();

            /* Accessors */
            Point get_source();
            vector<Point> get_sinks();
            Point getSink();

            /* Mutators */
            void set_source(Point source);
            void add_sink(Point sink);

    };
}

#endif //_NETLIST_BASE_H_

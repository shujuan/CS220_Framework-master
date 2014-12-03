#ifndef _NETLIST_BASE_H_
#define _NETLIST_BASE_H_

#include "segmentgroup.h"
#include <vector>

using std::vector;
using Utilities::Point;
using Utilities::PathSegment;

namespace Utilities {
<<<<<<< HEAD

	class Netlist: public SegmentGroup {

		private:
=======
    class Netlist: public SegmentGroup {
        private:
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
            Point source;
            vector<Point> sinks;

        public:
            Netlist();
            ~Netlist();

            /* Accessors */
            Point get_source();
            vector<Point> get_sinks();
<<<<<<< HEAD
            Point getSink();
=======

            /* 
            == Inherited Accessors ==
            unsigned size() const;
            PathSegment* at(unsigned index) const;
            bool empty() const;
            bool contains(const Point& point);
            int get_length();
            */
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81

            /* Mutators */
            void set_source(Point source);
            void add_sink(Point sink);

<<<<<<< HEAD
    };
}

#endif //_NETLIST_BASE_H_
=======
            /*
            == Inherited Mutators ==
            void add_segment(PathSegment* pathsegment);
            void add_segment(Point source, Point sink);
            void remove_segment(PathSegment* pathsegment);
            void remove_segment(Point soruce, Point sink);
            void remove_segment(int index);
            */
    };
}

#endif //_NETLIST_BASE_H_
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81

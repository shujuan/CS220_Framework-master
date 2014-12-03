#ifndef _SEGMENT_GROUP_BASE_H_
#define _SEGMENT_GROUP_BASE_H_

#include "pathsegment.h"
#include <vector>

using std::vector;
using Utilities::Point;
using Utilities::PathSegment;

namespace Utilities {
<<<<<<< HEAD

class SegmentGroup {

		private:

			vector<PathSegment*> segments;
            bool on_segment(Point p1, Point p2, Point p3);

        public:

=======
    class SegmentGroup {
        private:
            vector<PathSegment*> segments;

            bool on_segment(Point p1, Point p2, Point p3);

        public:
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
            SegmentGroup();
            ~SegmentGroup();

            /* Accessors */
            unsigned size() const;
            PathSegment* at(unsigned index) const;
            bool empty() const;
            bool contains(const Point& point);
            int get_length();

            /* Mutators */
            void add_segment(PathSegment* pathsegment);
            void add_segment(Point source, Point sink);
            void remove_segment(PathSegment* pathsegment);
            void remove_segment(Point soruce, Point sink);
            void remove_segment(int index);
    };
}

<<<<<<< HEAD
#endif //_PATH_SEGMENT_GROUP_BASE_H_
=======
#endif //_PATH_SEGMENT_GROUP_BASE_H_
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81

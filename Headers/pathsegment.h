#ifndef _PATH_SEGMENT_BASE_H_
#define _PATH_SEGMENT_BASE_H_

#include "point.h"

namespace Utilities {
<<<<<<< HEAD

=======
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
    class PathSegment {
        private:
            Point source;
            Point sink;
            int length;

            void update_length();

        public:
            /* Constructors/Destructors */
            PathSegment(Point source, Point sink);
            PathSegment(int p1_x, int p1_y, int p2_x, int p2_y);
            ~PathSegment();

            /* Overloaded Operators */
            bool operator==(const PathSegment& rhs);

            /* Accessors */
            Point get_source();
            Point get_sink();
            int get_length();
            bool contains_points(Point source, Point sink);

            /* Mutators */
<<<<<<< HEAD
            void set_source(Point source);
=======
            void set_source(Point source);          
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
            void set_sink(Point sink);
    };
}

#endif //_PATH_SEGMENT_BASE_H_

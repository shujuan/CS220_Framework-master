<<<<<<< HEAD

/**
 *
 */

=======
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
#ifndef _POINT_BASE_H_
#define _POINT_BASE_H_

namespace Utilities {
<<<<<<< HEAD

	struct Point {
=======
    struct Point {
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
        int x;
        int y;

        /* Constructor/Destructor */
        Point() { x = 0;y = 0; }
        Point(int x, int y) { this->x = x; this->y = y; }
        ~Point() {}

        /* Overloaded Operators */
        bool operator==(const Point& rhs) {
<<<<<<< HEAD
            if(rhs.x == this->x && rhs.y == this->y) {
                return true;
=======
            if(rhs.x == this->x && rhs.y == this->y) { 
                return true; 
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
            }
            return false;
        }
    };
}

#endif  //_POINT_BASE_H_

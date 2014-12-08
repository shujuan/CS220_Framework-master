#ifndef _PATH_BASE_H_
#define _PATH_BASE_H_

#include "segmentgroup.h"
#include <vector>

using Utilities::Point;

namespace Utilities {

class Path: public SegmentGroup {

private:
	Point source;
	Point sink;

public:
	Path();
	~Path();

	/* Accessors */
	Point get_source();
	Point get_sink();

	/* Mutators */
	void set_source(Point source);
	void set_sink(Point sink);


};
}

namespace Utilities {

class path {
};

} /* namespace Utilities */
#endif //_PATH_BASE_H_

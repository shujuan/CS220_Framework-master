
#ifndef LEEROUTER_H_
#define LEEROUTER_H_

#include "grgrid.h"
#include "grnode.h"

using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class LeeRouter {

private :

	GRGrid * maze ;

public:

	LeeRouter( GRGrid *grid );

	virtual ~LeeRouter();

	bool routeNet(GRNode * target, GRNode * source);

	bool findPath(GRNode * target, GRNode * source);

};

} /* namespace Utilities */

#endif /* LEEROUTER_H_ */

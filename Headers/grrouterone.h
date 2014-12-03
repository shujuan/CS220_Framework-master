/*
 * GRRouterOne.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#ifndef GRROUTERONE_H_
#define GRROUTERONE_H_

#include <vector>

#include "grgrid.h"
#include "grnode.h"
#include "GRNetList.h"
#include "grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class GRRouterOne: public GRBaseRouter {

public:

	GRRouterOne( GRGrid *grid, vector<Utilities::GRNetList*> list );

	virtual ~GRRouterOne();

	void routeCells( int x1, int y1 );

	void routeGrid( bool even );

	void updateNets(vector<Utilities::GRNetList*> nets);

	void routeNets( int cycles );

	bool isLocalBottleNeck( int x, int y, int net );


};

} /* namespace Utilities */

#endif /* GRROUTERONE_H_ */

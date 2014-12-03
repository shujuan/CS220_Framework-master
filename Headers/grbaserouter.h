/*
 * grbaserouter.h
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#ifndef GRBASEROUTER_H_
#define GRBASEROUTER_H_

#include <vector>

#include "grgrid.h"
#include "grnode.h"
#include "GRNetList.h"

using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class GRBaseRouter {

protected:

	GRGrid * maze ;
	vector<Utilities::GRNetList*> inputNets ;

public:

	GRBaseRouter( GRGrid *grid, vector<Utilities::GRNetList*> list );
	virtual ~GRBaseRouter();

	void markGenNeckCells(vector<GRNode*> * pathnodes, GRNode* s, GRNode* t ) ;

	void sortNets(vector<Utilities::GRNetList*> * nets);
	void sortNodes(vector<Utilities::GRNode*> * n);

	Utilities::GRNetList * getHPriorityNet(vector<GRNode*> listofnodes);
	Utilities::GRNode * getHPriorityNode(vector<GRNode*> listofnodes);

	bool findGRPath(GRNode * target, GRNode * source, int cycle ) ;
	int findGRPaths( int cycle, int p  ) ;
	void retraceGRPath( GRNode * source, GRNode * sink ) ;

	GRNetList * getGRNetObject(int netid ) ;

};

} /* namespace Utilities */

#endif /* GRBASEROUTER_H_ */

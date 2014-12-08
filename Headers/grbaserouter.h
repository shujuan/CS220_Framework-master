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

	GRGrid * nextmaze ;
	GRGrid * prevmaze ;
	vector<Utilities::GRNetList*> inputNets ;

public:

	GRBaseRouter( GRGrid *grid1,  GRGrid *grid2, vector<Utilities::GRNetList*> list );
	virtual ~GRBaseRouter();

	virtual void markGenNeckCells( GRGrid * m, vector<GRNode*> * pathnodes, GRNode* s, GRNode* t ) ;
	virtual void retraceGRPath( GRGrid * m, GRNode *source, GRNode * sink ) ;
	virtual bool isLocalBottleNeck( GRGrid * m, int x1, int y1, int net ) ;
	virtual int findGRPaths( GRGrid * m, int cycle, int p  ) ;
	virtual bool findLeePath( GRGrid * m, GRNode * target, GRNode * source, int cycle ) ;

	void sortNets(vector<Utilities::GRNetList*> * nets);
	void sortNodes(vector<Utilities::GRNode*> * n);

	Utilities::GRNetList * getHPriorityNet(vector<GRNode*> listofnodes);
	Utilities::GRNode * getHPriorityNode(vector<GRNode*> listofnodes);

	virtual GRNetList * getGRNetObject(int netid ) ;

	virtual void synchonizeGrids ();


};

} /* namespace Utilities */

#endif /* GRBASEROUTER_H_ */

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
#include "grnetlist.h"

using Utilities::GRGrid;
using Utilities::GRNode;

#define USHORT int
#define UINT  int


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

	virtual void retracePath( GRGrid * m, GRNode *source, GRNode * sink ) ;

	virtual bool isLocalBottleNeck( GRGrid * m, int x1, int y1, int net ) ;

	virtual int findPathNets( GRGrid * m, int cycle, int p  ) ;

	virtual bool findLeePath( GRGrid * m, GRNode * target, GRNode * source, int cycle ) ;

	void sortNets(vector<Utilities::GRNetList*> * nets);
	void sortNodes(vector<Utilities::GRNode*> * n);

	Utilities::GRNetList * getHPriorityNet(vector<GRNode*> listofnodes);
	virtual GRNetList * getNetObject(int netid ) ;

	Utilities::GRNode * getHighPriorityNode(vector<GRNode*> listofnodes);
	Utilities::GRNode * getLowPriorityNode(vector<GRNode*> listofnodes);
	Utilities::GRNode * getLowPriorityNode(vector<GRNode*> listofnodes, int net );

	void setNetMaxPriorities( GRGrid * m, int targetNetID, int p  ) ;
	void initGridPriorities( GRGrid * m, int pushNet ) ;

	bool findPathNet( GRGrid *m,  int cycle, int net  ) ;

	virtual void synchonizeGrids ();


};

} /* namespace Utilities */

#endif /* GRBASEROUTER_H_ */

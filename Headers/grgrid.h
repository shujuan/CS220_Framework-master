/*
 * GRGrid.h
 *
 *  Created on: Nov 14, 2014
 *      Author: jmilet
 */

#ifndef GRGRID_H_
#define GRGRID_H_

#include <list>
#include <vector>

#include "grid.h"
#include "grnode.h"
#include "path.h"

#define UINT int 

using std::list ;
using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class GRGrid: public Grid {

public:

	GRGrid( int w, int h);
	virtual ~GRGrid();

	bool addBlocker( int x, int y, int w, int h ) ;
	bool checkdPoint( int x, int y ) ;

	list<GRNode*> findNeighborW4( UINT r, UINT c)  ;

	bool checkPointW6( UINT r, UINT c , int netID  )  ;

	list<GRNode*> findNeighborFree( UINT r, UINT c  )  ;

	GRNode* findNeighborWithMark( UINT r, UINT c, int mark )  ;

	vector<GRNode*> findGRNeighbor( UINT r, UINT c , bool n )  ;
	vector<GRNode*> findGRNeighbor( UINT r, UINT c , UINT netID )  ;
	GRNode*         findGRNeighborWithMark( UINT r, UINT c, UINT netID, int mark )  ;

	vector<GRNode*> retraceGRPath( GRNode * source, GRNode *target  )  ;

	Path * createPath( vector<GRNode*> klist )  ;
	void clearPaths();


	void deleteLeeMarks() ;
	void deleteBNetFlag() ;
	void deleteLeeMarks( int netID ) ;

	void displayNets();
	void displayMarks();

	void pruneGrid();

	void writeGridDiskNets( int filen ) ;

	void addNodeToNet( int x, int y, int netID );


};

} /* namespace Utilities */

#endif /* GRGRID_H_ */

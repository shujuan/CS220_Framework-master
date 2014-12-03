/*
 * grnode.h
 *
 *  Created on: Nov 14, 2014
 *      Author: jmilet
 */

#ifndef GRNODE_H_
#define GRNODE_H_

#include "node.h"
#include "point.h"

namespace Utilities {

class GRNode: public Node {

private:

	bool block ;   	// The cell is part of the board
	int  leeMark ;  // Lee mark routing

	bool inpath ; 	// If the cell is in a path
	int  netID ; 	// Net ID per cell
	bool pin ; 		// Pin flag

	bool bottleNeck ; // Bottle neck flag
	bool updateCell ; // If the cell was updated in the last cycle
	int  priority ;  // Cell priority

public:

    /* Constructors/Destructors */

	GRNode(int x, int y, int cost, bool isblock, int mark );

	virtual ~GRNode();

	int getX() ;
	int getY() ;

	void setInBlock( bool b);
	bool getInBlock();

	void setLeeMark( int m );
	int  getLeeMark();
	void clearLeeMark() ;

	void setNetID( int netid ) ;
	int getNetID() ;

	void setBlocker();

	bool getInPath();
	void setInPath( bool p ) ;

	bool getPin() ;
	void setPin( bool p ) ;

	void setBottleNeck( bool b);
	bool getBottleNeck( );

	void setUpdatedCell( bool n) ;
	bool getUpdatedCell() ;

	void setPriority( int p) ;
	int  getPriority( ) ;

	void displayNode();
	void displayPriority();

	bool getBoardCell();

};

} /* namespace Utilities */

#endif /* GRNODE_H_ */

/*
 * GRNetList.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#ifndef GRNETLIST_H_
#define GRNETLIST_H_

#include "netlist.h"

#include <string>

using Utilities::Netlist;
using Utilities::Point;

using std::string;

namespace Utilities {

class GRNetList: public Netlist {

private:

	int netID ;
	string netname ;
	int netPriority;
	bool connected ;

public:

	GRNetList(Point source, Point sink, int netId, string name, int priority);
	virtual ~GRNetList();

	int getPriority();
	void setPriority( int p );

	int getNetID();
	void setNetID( int id );

	void setConnected( bool c );
	bool getConnected();

	string getNetName() ;

	bool operator < ( const GRNetList& b ) const ;

	bool compareNet(GRNetList* a, GRNetList* b) { return (*a < *b); }


};

} /* namespace Utilities */

#endif /* GRNETLIST_H_ */

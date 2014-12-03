/*
 * GRNetList.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#include "..\Headers\GRNetList.h"

#include <iostream>

using std::endl;
using std::cout;

Utilities::GRNetList::GRNetList(Point source, Point sink, int Id, string s,int p ):Netlist(),
		netID(Id), netname(s),  netPriority(p) , connected (false) {

	this->set_source(source) ;
	this->add_sink( sink ) ;

}

void Utilities::GRNetList::setPriority( int p)  {

	this->netPriority = p ;

}

int Utilities::GRNetList::getPriority()  {

	return this->netPriority ;

}

int Utilities::GRNetList::getNetID() {

	return this->netID ;

}

void Utilities::GRNetList::setNetID( int id ) {

	this->netID = id ;

}


string Utilities::GRNetList::getNetName() {

	return this->netname ;

}

bool Utilities::GRNetList::getConnected() {
	return this->connected ;
}

void Utilities::GRNetList::setConnected( bool c ) {
	this->connected = c ;
}


bool Utilities::GRNetList::operator<(const GRNetList& net ) const {

	// cout << " Net 1 : " << netname << " " << netPriority << " \n " ;
	// cout << " Net 2 : " << net.netname << " " << net.netPriority << " \n " ;
	// cout << " Compare : " << (  this->netname < net.netname )  << " \n " ;

	if ( this->netPriority == net.netPriority )  {

		return this->netname <  net.netname 	;

	} else {

		return (this->netPriority < net.netPriority ) ;

	}

   return 0;

}

Utilities::GRNetList::~GRNetList() {

}


/*
 * grnode.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: jmilet
 */

#include "../Headers/grnode.h"
#include "../Headers/node.h"

Utilities::GRNode::GRNode(int x, int y, int cost, bool isblock, int mark )
	:Node(x,y,cost), block(isblock),
	  leeMark(mark),
	  inpath(false),
	  netID(0),
	  pin(false),
	  bottleNeck(false),
	  priority(0) {

}

int Utilities::GRNode::getX() {
	return this->get_x() ;
}

int Utilities::GRNode::getY() {
	return this->get_y() ;
}

bool Utilities::GRNode::getInBlock() {
	return this->block ;
}

void Utilities::GRNode::setInBlock( bool b) {
	this->block = b;
}


void Utilities::GRNode::setLeeMark( int m ) {
	this->leeMark = m ;
}

int Utilities::GRNode::getLeeMark( ) {
	return this->leeMark ;
}

void Utilities::GRNode::setBlocker( ) {

	 this->setInBlock( true ) ;
	 this->setLeeMark(0 ) ;
	 this->setInPath(false) ;
	 this->setPin(false) ;

}

void Utilities::GRNode::clearLeeMark( ) {

	if ( !this->getInBlock() )  {

		this->setLeeMark(0) ;
		this->setInPath(false);

	}

}

int Utilities::GRNode::getNetID() {
	return this->netID ;
}


void Utilities::GRNode::setNetID(int id )  {
	this->netID = id;
}

bool Utilities::GRNode::getInPath( ) {
	return this->inpath ;
}

void Utilities::GRNode::setInPath( bool p) {
	this->inpath = p;
}

void Utilities::GRNode::setPin( bool p) {
	this->pin = p ;
}

bool Utilities::GRNode::getPin( ) {
	return this->pin ;
}

bool Utilities::GRNode::getBoardCell()  {

	return  (this->pin | this->block );

}

void Utilities::GRNode::displayNode( ) {
	std::cout << "(" << get_x() << " " << get_y() <<
								" ID " << getNetID() << " M " << this->getPriority() << ") \n " ;
}

bool Utilities::GRNode::getBottleNeck()  {

	return this->bottleNeck ;

}

void Utilities::GRNode::setBottleNeck( bool bn)  {

	this->bottleNeck = bn ;
}

void Utilities::GRNode::setPriority(int p) {
	this->priority = p ;
}

int Utilities::GRNode::getPriority() {
	return this->priority ;
}

Utilities::GRNode::~GRNode() {

}


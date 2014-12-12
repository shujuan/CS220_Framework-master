/*
 * GRGrid.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: jmilet
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <vector>

#include "../Headers/grgrid.h"
#include "../Headers/grnode.h"
#include "../Headers/edge.h"
#include "../Headers/path.h"
#include "../Headers/pathsegment.h"
#include "../Headers/claim.h"

#include <stdio.h>
#include <stdlib.h>

using std::cout;
using std::list ;

Utilities::GRGrid::GRGrid( int w, int h):Grid() {

	for(int y = 0; y < h; y++) {

		vector<Node*> temp_row;

		for(int x = 0; x < w; x++) {

			Node* new_node = new GRNode(x,y,0,false,0);

			if (x > 0) {
				Edge* west = new Edge( (Node *) new_node,temp_row.at(temp_row.size()-1));
				new_node->add_connection(west);
			}

			if (y > 0) {
				Edge* north = new Edge( (Node *) new_node,grid.at(y-1).at(x));
				new_node->add_connection(north);
			}

			temp_row.push_back(new_node);

		}

		this->grid.push_back(temp_row);
	}

	// We have to define the borders - Rows

	for ( int x=0; x< w; x++ )  {

		GRNode* node1 = (GRNode*) this->get_node(x,0) ;
		node1->setInBlock(true) ;

		GRNode* node2 = (GRNode*) this->get_node(x,h-1) ;
		node2->setInBlock(true) ;

	}

	// We have to define the borders - Cols

	for ( int c=0; c< h; c++ )  {

		GRNode* node = (GRNode*) this->get_node(0,c) ;
		node->setInBlock(true) ;

		node = (GRNode*) this->get_node(w-1,c) ;
		node->setInBlock(true) ;

	}

}

bool Utilities::GRGrid::checkdPoint( UINT x, UINT y)  {

	if ( ( 0 <= x  ) && ( x < this->get_width() ) )  {

		if ( ( 0 <= y  ) && ( y < this->get_height() ) )  {

			return true ;

		}

	}

	return false ;
}


list<GRNode*> Utilities::GRGrid::findNeighborW4( UINT x, UINT y)  {

	list<GRNode*> nlist ;

	// We have to make sure the point x - w is inside rect

	if ( ( 0 <= x  ) && ( x < this->get_width() ) )  {

		if ( ( 0 <= y  ) && ( y < this->get_height() ) )  {

			if ( y >= 1 ) {
				nlist.push_front( (GRNode*) this->get_node(x, y-1) ) ;
			}

			if ( y < (this->get_height()-1) )   {
				nlist.push_front( (GRNode*) this->get_node(x, y+1) ) ;
			}

			if ( x >= 1 )  {
				nlist.push_front( (GRNode*) this->get_node(x-1, y) ) ;
			}

			if ( x < (this->get_width()-1) )  {
				nlist.push_front( (GRNode*) this->get_node(x+1, y) ) ;
			}

		}
	}

	return nlist ;

}

bool Utilities::GRGrid::checkPointW6( UINT x1, UINT y1, int netID )  {

	if ( checkdPoint( x1, y1 ) ) {

		GRNode* node1 =  (GRNode*) this->get_node(x1, y1) ;

		if ( node1->getNetID() == netID ) {

			return true ;

		}
	}

	return false ;


}

GRNode* Utilities::GRGrid::findNeighborWithMark( UINT x, UINT y, int mark )  {

	list<GRNode*> listn = this->findNeighborW4(x,y);

	GRNode * node = NULL;

	for(list<GRNode*>::iterator iter=listn.begin(); iter != listn.end(); ++iter) {

		// We do not consider block nodes ***********

		if ( !(*iter)->getInBlock() )  {

			if ( (*iter)->getLeeMark() == (mark-1) )  {

				node = (*iter) ;

				break ;

			}
		}

	}

	return node ;

}


GRNode* Utilities::GRGrid::findGRNeighborWithMark( UINT x, UINT y, UINT netid, UINT mark )  {

	list<GRNode*> listn = this->findNeighborW4(x,y);

	GRNode * node = NULL;

	for(list<GRNode*>::iterator iter=listn.begin(); iter != listn.end(); ++iter) {

		// We do not consider block nodes ***********

		if ( !(*iter)->getInBlock() )  {

			if ( (*iter)->getNetID() == netid  )  {

				if ( (*iter)->getLeeMark() == (mark-1) )  {

					node = (*iter) ;

					break ;

				}
			}
		}

	}

	return node ;

}



list<GRNode*> Utilities::GRGrid::findNeighborFree( UINT x, UINT y )  {

	list<GRNode*> nlist ;

	list<GRNode*> listn = this->findNeighborW4(x,y);

	for(list<GRNode*>::iterator iter=listn.begin(); iter != listn.end(); ++iter) {

		// We do not consider block nodes ***********

		if ( !(*iter)->getInBlock() )  {

			if ( !(*iter)->getInPath()  )  {

				nlist.push_front( *iter ) ;

			}
		}

	}

	return nlist ;

}

vector<GRNode*> Utilities::GRGrid::findGRNeighbor( UINT x, UINT y , bool n  )  {

	vector<GRNode*> nlist ;

	list<GRNode*> listn = this->findNeighborW4(x,y);

	for(list<GRNode*>::iterator iter=listn.begin(); iter != listn.end(); iter++) {

		if ( !(*iter)->getInBlock() )  {

			if ( (*iter)->getNetID() > 0 ) {

				nlist.push_back( *iter ) ;

			}
		}

	}

	return nlist ;

}


vector<GRNode*> Utilities::GRGrid::findGRNeighbor( UINT x, UINT y , UINT netID  )  {

	vector<GRNode*> nlist ;

	list<GRNode*> listn = this->findNeighborW4(x,y);

	for(list<GRNode*>::iterator iter=listn.begin(); iter != listn.end(); iter++ ) {

		if ( !(*iter)->getInBlock() )  {

			if ( (*iter)->getNetID() == netID ) {

				nlist.push_back( *iter ) ;

			}
		}

	}

	return nlist ;

}


void Utilities::GRGrid::displayNets() {

	for ( int y=0;y < this->get_height(); y++ )  {

		std::cout << "\n" ;

		for ( int x=0; x < this->get_width(); x++ )  {

			GRNode * node = (GRNode*) this->get_node(x,y) ;

			std::cout << "(" << node->get_x() << "-" << node->getNetID() << "-" << node->get_y() << ")|" ;

		}

		std::cout << "\n" ;

	}

}


void Utilities::GRGrid::displayMarks() {

	for ( int y=0;y < this->get_height(); y++ )  {

		std::cout << "\n" ;

		for ( int x=0; x < this->get_width(); x++ )  {

			GRNode * node = (GRNode*) this->get_node(x,y) ;

			std::cout << "(" << node->get_x() << "-" << node->getLeeMark() << "-" << node->get_y() << ")|" ;

		}

		std::cout << "\n" ;

	}

}

bool Utilities::GRGrid::addBlocker( int x, int y, int w, int h ) {

	for ( int x1=x; x1 < (x+w); x1++ )   {

		for ( int y1=y; y1 < (y+h); y1++ )   {

			GRNode * node = (GRNode *) this->get_node(x1, y1);

			node->setBlocker();

		}
	}

	return true ;

}


void Utilities::GRGrid::addNodeToNet( int x1, int y1, int netID ) {

	GRNode * node = (GRNode *) this->get_node(x1, y1);
	node->setNetID( netID ) ;

}

void Utilities::GRGrid::clearPaths( ) {

	while (true) {

		vector<Path*> allpaths = this->get_paths() ;

		if ( allpaths.size() > 0 )  {

			this->remove_path(0) ;

		}
		else {

			break ;

		}
	}

}


Path * Utilities::GRGrid::createPath( vector<GRNode*>  list ) {

	Path* path = new Path();

	GRNode * nextNode  ;
	GRNode * backNode = list.back() ;

	path->set_sink(backNode->get_coord()) ;

	list.pop_back() ;

	while ( list.size() > 0 )  {

		nextNode =  list.back() ;

		if ( backNode->getX() == nextNode->getX() )  {

			// Going in to the x direction
			PathSegment * pathSeg = new PathSegment(backNode->get_coord(), nextNode->get_coord()) ;
			path->add_segment(pathSeg) ;
			path->set_sink(backNode->get_coord()) ;
			backNode = nextNode ;

		} else if ( backNode->getY() == nextNode->getY() )  {

			// Going in to the y direction
			PathSegment * pathSeg = new PathSegment(backNode->get_coord(), nextNode->get_coord()) ;
			path->add_segment(pathSeg) ;
			path->set_sink(backNode->get_coord()) ;
			backNode = nextNode ;

		} else {

			claim("PathSegment being created does not represent a straight line segment", kError);
			break ;

		}

		list.pop_back() ;

	}  // End while

	return path ;
}

vector<GRNode*> Utilities::GRGrid::retraceGRPath( GRNode * source, GRNode *target ) {

	vector<GRNode*> nodesInPath ;

	nodesInPath.push_back(target);

	GRNode * gnode = this->findGRNeighborWithMark(
			target->getX(), target->getY() , target->getNetID() , target->getLeeMark() ) ;

	while ( gnode != NULL )  {

		nodesInPath.push_back(gnode) ;

		if ( (*gnode) == (*source) )  {

			break ;

		}

		else {

			gnode = this->findGRNeighborWithMark(
					gnode->getX(), gnode->getY() , target->getNetID(), gnode->getLeeMark() ) ;

		}

	}  // End While

	return nodesInPath ;

}


void Utilities::GRGrid::deleteBNetFlag() {

	for ( int y=1; y < (this->get_height()); y++ )  {

		for ( int x=1; x < (this->get_width()); x++ )  {

			GRNode * gnode = (GRNode *) this->get_node( x,y ) ;

			if ( !gnode->getInBlock() ) {

				gnode->setBottleNeck(false) ;

			}
		}
	}
}

void Utilities::GRGrid::deleteLeeMarks() {

	for ( int y=1; y < (this->get_height()); y++ )  {

		for ( int x=1; x < (this->get_width()); x++ )  {

			GRNode * gnode = (GRNode *) this->get_node( x,y ) ;

			if ( !gnode->getInBlock() ) {

				gnode->clearLeeMark() ;

			}
		}
	}
}

void Utilities::GRGrid::deleteLeeMarks(int netID ) {

	for ( int y=1; y < (this->get_height()); y++ )  {

		for ( int x=1; x < (this->get_width()); x++ )  {

			GRNode * gnode = (GRNode *) this->get_node( x,y ) ;

			if ( !gnode->getInBlock() ) {

				if ( gnode->getNetID() ==  netID)  {

					gnode->clearLeeMark() ;

				}
			}
		}
	}
}


void Utilities::GRGrid::writeGridDiskNets( int filen ) {

	char buffer1[32] ;
	sprintf(buffer1,"%d",filen); 

	char buffer2[256] ;

	// strcpy(buffer2, "f:\\testcases\\file0" );
	strcpy(buffer2, "file0" );

	strcat (buffer2, buffer1 );
	strcat (buffer2, ".txt" );

	// cout << buffer2 << "\n" ;

	std::ofstream file ( buffer2 );

	if ( file.is_open() ) {

		// Write one line with the file size

		file << this->get_width()  << " " << this->get_height() << "\n" ;

		for ( int y=0; y < this->get_height(); y++ )  {

			for ( int x=0; x < this->get_width(); x++ )  {

				GRNode * gnode = (GRNode *) this->get_node( x,y ) ;

				if ( gnode->getInBlock() ) {

					file << " B " << gnode->getX() << " " << gnode->getY() << " | " ;
					continue ;

				}

				if ( gnode->getPin() ) {

					file << " P " << gnode->getX() << " " << gnode->getY() <<
							" " << gnode->getNetID() << " " << gnode->getPriority() << " | " ;
					continue ;
				}

				file << " C " << gnode->getX() << " " << gnode->getY() <<
						" " << gnode->getNetID() << " " << gnode->getPriority() << " | " ;

			}

			file << "\n" ;
		}

		file.close();
	}

	else {

		cout << "Unable to open file";

	}
}


void Utilities::GRGrid::pruneGrid() {

	for ( int x=1; x < this->get_width(); x++ )  {

		for ( int y=1; y < this->get_height(); y++ )  {

			GRNode * gnode = (GRNode *) this->get_node( x,y ) ;

			if ( !gnode->getBoardCell() ) {

				if ( !gnode->getInPath() )  {

					gnode->setLeeMark(0) ;
					gnode->setNetID(0) ;

				}
			}
		}
	}
}


Utilities::GRGrid::~GRGrid() {

}


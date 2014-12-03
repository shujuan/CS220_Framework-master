#ifndef _EDGE_BASE_H_
#define _EDGE_BASE_H_

#include "node.h"

/* Note: Edges are undirected */

namespace Utilities {
<<<<<<< HEAD

=======
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81
    class Node;

    class Edge {
        private:
            Node* head;
            Node* tail;

        public:
            /* Constructors/Destructors */
            Edge(Node*, Node*);
            ~Edge();

            /* Accessors */
            Node* get_head();
            Node* get_tail();
            Node* get_end(Node* start);

            /* Mutators */
            void set_head(Node* head);
            void set_tail(Node* tail);
    };
}

<<<<<<< HEAD
#endif  //_EDGE_BASE_H_
=======
#endif  //_EDGE_BASE_H_
>>>>>>> 2c3d3687aac8e1c8c30b7d3a57d58e48fe1abb81

# documentation for _hulljus_

## introduction

in _hulljus_, programs are created by connecting __nodes__.

a __connode__ is a node that represents a function. upon __activation__, a connode performs its function, taking one or more inputs from __varnodes__ connected to it.

a connode is only activated by a left double click or another connode __from-connected__ to it. upon __activation__, a connode activates all connodes __to-connected__ to it.

the function of a connode corresponds to its __label__.

## savefile format

### savefile extension

the savefile extension is `.hjs` for __h__ull__j__us __s__avefile.

### savefile content

the savefile content consists of two parts, separated from each other by a tilde `~`:

`part 1~part 2`

#### part one

the first part of the savefile content is a semicolon `;`-separated sequence of information for each node:

`information for node 1;information for node 2;...`

each piece of information for a node is given by:

`nodetype,label,x position,y position`

where

*	`nodetype` is an integer representing the type of the node. this corresponds to the enumeration `gennode::nodetype` used in the source code for _hulljus_.
*	`label` is a string representing the label of the node. if this node is a varnode, then its value should be assigned by calling the function `varnode::setFromStringIfValid()`, while passing the label as the string argument.
*	`x position` is an integer representing the x position of the top-left corner of the node on the client screen.
*	`y position` is an integer representing the y position of the top-left corner of the node on the client screen.

#### part two

the second part of the savefile content is a semicolon `;`-separated sequence of information for the connections and varnections each node has:

`information for connections and varnections of node 1;information for connections and varnections of node 2;...`

each piece of information consists of a part about connections and another about varnections. the two parts are separated from each other by a comma `,`:

`about connections,about varnections`

information about connections is a space ` `-separated sequence of integers, each of which represents the index of a to-connected node in the `std::vector` of nodes kept by the program. this `std::vector` should be freshly populated according to node information provided in part one of the savefile content.

information about varnections is a space ` `-separated sequence of integers, each of which represents the index of a out-connected node in the `std::vector` of nodes kept by the program.

### savefile example

`example.hjs` has the following content

    1,popmsg,10,10;2,10,80,80~ ;,1
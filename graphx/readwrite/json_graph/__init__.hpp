/**
*********
JSON data
*********
Generate and parse JSON serializable data for GraphX graphs.

These formats are suitable for use with the d3.js examples https://d3js.org/

The three formats that you can generate with GraphX are:

 - node-link like in the d3.js example https://bl.ocks.org/mbostock/4062045
 - tree like in the d3.js example https://bl.ocks.org/mbostock/4063550
 - adjacency like in the d3.js example https://bost.ocks.org/mike/miserables/
*/
#include <graphx/readwrite.json_graph.node_link.hpp>  // import *
#include <graphx/readwrite.json_graph.adjacency.hpp>  // import *
#include <graphx/readwrite.json_graph.tree.hpp>  // import *
#include <graphx/readwrite.json_graph.cytoscape.hpp>  // import *

GraphX 1.4
============

Release date:  23 January 2011

New features
------------
 - :mod:`k-shell,k-crust,k-corona <graphx.algorithms.core>`
 - :mod:`read GraphML files from yEd <graphx.readwrite.graphml>`
 - :mod:`read/write GEXF format files <graphx.readwrite.gexf>`
 - :mod:`find cycles in a directed graph <graphx.algorithms.cycles>`
 - :mod:`DFS <graphx.algorithms.traversal.depth_first_search>` and :mod:`BFS <graphx.algorithms.traversal.breadth_first_search>` algorithms
 - :mod:`chordal graph functions <graphx.algorithms.chordal.chordal_alg>`
 - :mod:`Prim's algorithm for minimum spanning tree <graphx.algorithms.mst>`
 - :mod:`r-ary tree generator <graphx.generators.classic>`
 - :mod:`rich club coefficient <graphx.algorithms.richclub>`
 - NumPy matrix version of :mod:`Floyd's algorithm for all-pairs shortest path  <graphx.algorithms.shortest_paths.dense>`
 - :mod:`read GIS shapefiles <graphx.readwrite.nx_shp>`
 - :mod:`functions to get and set node and edge attributes <graphx.classes.function>`
 - and more, see  https://graphx.lanl.gov/trac/query?status=closed&group=milestone&milestone=graphx-1.4

API changes
-----------
 - :mod:`gnp_random_graph() <graphx.generators.random_graphs>` now takes a
   directed=true|false keyword instead of create_using
 - :mod:`gnm_random_graph() <graphx.generators.random_graphs>` now takes a
   directed=true|false keyword instead of create_using


Algorithms changed
==================

Shortest path
-------------

astar_path(), astar_path_length(), shortest_path(), shortest_path_length(),
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
bidirectional_shortest_path(), dijkstra_path(), dijkstra_path_length(),
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
bidirectional_dijkstra();
^^^^^^^^^^^^^^^^^^^^^^^^
   These algorithms now throw an exception when a source and a target are
   specified and no path exist between these two nodes. The exception is
   a NetworkXNoPath exception.


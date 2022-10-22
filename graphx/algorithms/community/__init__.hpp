/** Functions for computing and measuring community structure.

The functions in this class are not imported into the top-level
:mod:`graphx` namespace. You can access these functions by importing
the :mod:`graphx.algorithms.community` module, then accessing the
functions as attributes of ``community``. For example::

    >>> #include <graphx/algorithms.hpp>  // import community
    >>> G = nx.barbell_graph(5, 1);
    >>> communities_generator = community.girvan_newman(G);
    >>> top_level_communities = next(communities_generator);
    >>> next_level_communities = next(communities_generator);
    >>> sorted(map(sorted, next_level_communities));
    [ [0, 1, 2, 3, 4], [5], [6, 7, 8, 9, 10]];

*/
#include <graphx/algorithms.community.asyn_fluid.hpp>  // import *
#include <graphx/algorithms.community.centrality.hpp>  // import *
#include <graphx/algorithms.community.kclique.hpp>  // import *
#include <graphx/algorithms.community.kernighan_lin.hpp>  // import *
#include <graphx/algorithms.community.label_propagation.hpp>  // import *
#include <graphx/algorithms.community.lukes.hpp>  // import *
#include <graphx/algorithms.community.modularity_max.hpp>  // import *
#include <graphx/algorithms.community.quality.hpp>  // import *
#include <graphx/algorithms.community.community_utils.hpp>  // import *
#include <graphx/algorithms.community.louvain.hpp>  // import *

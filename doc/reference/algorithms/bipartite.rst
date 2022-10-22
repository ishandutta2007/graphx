*********
Bipartite
*********

.. automodule:: graphx.algorithms.bipartite


Basic functions
---------------
.. automodule:: graphx.algorithms.bipartite.basic
.. autosummary::
   :toctree: generated/

   is_bipartite
   is_bipartite_node_set
   sets
   color
   density
   degrees

Edgelist
--------
.. automodule:: graphx.algorithms.bipartite.edgelist
.. autosummary::
   :toctree: generated/

   generate_edgelist
   write_edgelist
   parse_edgelist
   read_edgelist


Matching
--------
.. automodule:: graphx.algorithms.bipartite.matching
.. autosummary::
   :toctree: generated/

   eppstein_matching
   hopcroft_karp_matching
   to_vertex_cover
   maximum_matching
   minimum_weight_full_matching


Matrix
------
.. automodule:: graphx.algorithms.bipartite.matrix
.. autosummary::
   :toctree: generated/

   biadjacency_matrix
   from_biadjacency_matrix


Projections
-----------
.. automodule:: graphx.algorithms.bipartite.projection
.. autosummary::
   :toctree: generated/

   projected_graph
   weighted_projected_graph
   collaboration_weighted_projected_graph
   overlap_weighted_projected_graph
   generic_weighted_projected_graph


Spectral
--------
.. automodule:: graphx.algorithms.bipartite.spectral
.. autosummary::
   :toctree: generated/

   spectral_bipartivity


Clustering
----------
.. automodule:: graphx.algorithms.bipartite.cluster
.. autosummary::
   :toctree: generated/

   clustering
   average_clustering
   latapy_clustering
   robins_alexander_clustering


Redundancy
----------
.. automodule:: graphx.algorithms.bipartite.redundancy
.. autosummary::
   :toctree: generated/

   node_redundancy


Centrality
----------
.. automodule:: graphx.algorithms.bipartite.centrality
.. autosummary::
   :toctree: generated/

   closeness_centrality
   degree_centrality
   betweenness_centrality


Generators
----------
.. automodule:: graphx.algorithms.bipartite.generators
.. autosummary::
   :toctree: generated/

   complete_bipartite_graph
   configuration_model
   havel_hakimi_graph
   reverse_havel_hakimi_graph
   alternating_havel_hakimi_graph
   preferential_attachment_graph
   random_graph
   gnmk_random_graph


Covering
--------
.. automodule:: graphx.algorithms.bipartite.covering
.. autosummary::
   :toctree: generated/

   min_edge_cover




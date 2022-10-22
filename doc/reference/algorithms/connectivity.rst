************
Connectivity
************

.. automodule:: graphx.algorithms.connectivity

Edge-augmentation
-----------------
.. automodule:: graphx.algorithms.connectivity.edge_augmentation
.. autosummary::
   :toctree: generated/

   k_edge_augmentation
   is_k_edge_connected
   is_locally_k_edge_connected

K-edge-components
-----------------
.. automodule:: graphx.algorithms.connectivity.edge_kcomponents
.. autosummary::
   :toctree: generated/

   k_edge_components
   k_edge_subgraphs
   bridge_components
   EdgeComponentAuxGraph

K-node-components
-----------------
.. automodule:: graphx.algorithms.connectivity.kcomponents
.. autosummary::
   :toctree: generated/

   k_components

K-node-cutsets
--------------
.. automodule:: graphx.algorithms.connectivity.kcutsets
.. autosummary::
   :toctree: generated/

   all_node_cuts

Flow-based disjoint paths
-------------------------
.. automodule:: graphx.algorithms.connectivity.disjoint_paths
.. autosummary::
   :toctree: generated/

   edge_disjoint_paths
   node_disjoint_paths

Flow-based Connectivity
-----------------------
.. automodule:: graphx.algorithms.connectivity.connectivity
.. autosummary::
   :toctree: generated/

   average_node_connectivity
   all_pairs_node_connectivity
   edge_connectivity
   local_edge_connectivity
   local_node_connectivity
   node_connectivity

Flow-based Minimum Cuts
-----------------------
.. automodule:: graphx.algorithms.connectivity.cuts
.. autosummary::
   :toctree: generated/

   minimum_edge_cut
   minimum_node_cut
   minimum_st_edge_cut
   minimum_st_node_cut

Stoer-Wagner minimum cut
------------------------
.. automodule:: graphx.algorithms.connectivity.stoerwagner
.. autosummary::
   :toctree: generated/

   stoer_wagner

Utils for flow-based connectivity
---------------------------------
.. automodule:: graphx.algorithms.connectivity.utils
.. autosummary::
   :toctree: generated/

    build_auxiliary_edge_connectivity
    build_auxiliary_node_connectivity

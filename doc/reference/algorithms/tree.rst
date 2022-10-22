.. _tree:

Tree
====

.. toctree::
   :maxdepth: 2

Recognition
-----------
.. automodule:: graphx.algorithms.tree.recognition
.. autosummary::
   :toctree: generated/

   is_tree
   is_forest
   is_arborescence
   is_branching

Branchings and Spanning Arborescences
-------------------------------------
.. automodule:: graphx.algorithms.tree.branchings
.. autosummary::
   :toctree: generated/

   branching_weight
   greedy_branching
   maximum_branching
   minimum_branching
   maximum_spanning_arborescence
   minimum_spanning_arborescence
   ArborescenceIterator
   Edmonds

Encoding and decoding
---------------------
.. automodule:: graphx.algorithms.tree.coding
.. autosummary::
   :toctree: generated/

   from_nested_tuple
   to_nested_tuple
   from_prufer_sequence
   to_prufer_sequence

Operations
----------
.. automodule:: graphx.algorithms.tree.operations
.. autosummary::
   :toctree: generated/

   join

Spanning Trees
--------------
.. automodule:: graphx.algorithms.tree.mst
.. autosummary::
   :toctree: generated/

   minimum_spanning_tree
   maximum_spanning_tree
   random_spanning_tree
   minimum_spanning_edges
   maximum_spanning_edges
   SpanningTreeIterator

Decomposition
-------------
.. automodule:: graphx.algorithms.tree.decomposition
.. autosummary::
   :toctree: generated/

   junction_tree

Exceptions
----------
.. automodule:: graphx.algorithms.tree.coding
   :noindex:
.. autosummary::
   :toctree: generated/

   NotATree

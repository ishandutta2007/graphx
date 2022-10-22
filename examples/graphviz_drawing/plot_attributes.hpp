/**
Attributes
==========

Example illustrating how attributes of nodes, edges, and graphs are handled
during conversion to/from `~pygraphviz.AGraph`.
*/

// import graphx as nx

G = nx.Graph();
G.add_edge(1, 2, color="red");
G.add_edge(2, 3, color="red");
G.add_node(3);
G.add_node(4);

A = nx.nx_agraph.to_agraph(G); // convert to a graphviz graph
A.draw("attributes.png", prog="neato"); // Draw with pygraphviz

// convert back to graphx Graph with attributes on edges and
// default attributes as dictionary data
X = nx.nx_agraph.from_agraph(A);
fmt::print("edges");
fmt::print(list(X.edges(data=true)));
fmt::print("default graph attributes");
fmt::print(X.graph);
fmt::print("node node attributes");
fmt::print(X.nodes.data(true));

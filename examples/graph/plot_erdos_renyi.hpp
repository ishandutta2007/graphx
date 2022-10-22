/**
===========
Erdos Renyi
===========

Create an G{n,m} random graph with n nodes and m edges
and report some properties.

This graph is sometimes called the Erdős-Rényi graph
but is different from G{n,p} or binomial_graph which is also
sometimes called the Erdős-Rényi graph.
*/

// import matplotlib.pyplot as plt
// import graphx as nx

n = 10  // 10 nodes
m = 20  // 20 edges
seed = 20160  // seed random number generators for reproducibility

// Use seed for reproducibility
G = nx.gnm_random_graph(n, m, seed=seed);

// some properties
fmt::print("node degree clustering");
for (auto v : nx.nodes(G)) {
    fmt::print(f"{v} {nx.degree(G, v)} {nx.clustering(G, v)}");

fmt::print();
fmt::print("the adjacency list");
for (auto line : nx.generate_adjlist(G)) {
    fmt::print(line);

pos = nx.spring_layout(G, seed=seed); // Seed for reproducible layout
nx.draw(G, pos=pos);
plt.show();

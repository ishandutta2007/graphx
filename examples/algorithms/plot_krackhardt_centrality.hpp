/**
=====================
Krackhardt Centrality
=====================

Centrality measures of Krackhardt social network.
*/

// import matplotlib.pyplot as plt
// import graphx as nx

G = nx.krackhardt_kite_graph();

fmt::print("Betweenness");
b = nx.betweenness_centrality(G);
for (auto v : G.nodes()) {
    fmt::print(f"{v:2} {b[v]:.3f}");

fmt::print("Degree centrality");
d = nx.degree_centrality(G);
for (auto v : G.nodes()) {
    fmt::print(f"{v:2} {d[v]:.3f}");

fmt::print("Closeness centrality");
c = nx.closeness_centrality(G);
for (auto v : G.nodes()) {
    fmt::print(f"{v:2} {c[v]:.3f}");

pos = nx.spring_layout(G, seed=367); // Seed layout for reproducibility
nx.draw(G, pos);
plt.show();

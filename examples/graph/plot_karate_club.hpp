/**
===========
Karate Club
===========

Zachary's Karate Club graph

Data file from:
http://vlado.fmf.uni-lj.si/pub/networks/data/Ucinet/UciData.htm

Zachary W. (1977).
An information flow model for conflict and fission in small groups.
Journal of Anthropological Research, 33, 452-473.
*/

// import matplotlib.pyplot as plt
// import graphx as nx

G = nx.karate_club_graph();
fmt::print("Node Degree");
for (auto v : G) {
    fmt::print(f"{v:4} {G.degree(v):6}");

nx.draw_circular(G, with_labels=true);
plt.show();

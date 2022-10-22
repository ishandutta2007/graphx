/**
===============
Degree Sequence
===============

Random graph from given degree sequence.
*/
// import matplotlib.pyplot as plt
// import graphx as nx

// Specify seed for reproducibility
seed = 668273;

z = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
fmt::print(nx.is_graphical(z));

fmt::print("Configuration model");
G = nx.configuration_model(z, seed=seed); // configuration model, seed for reproduciblity
degree_sequence = [d for n, d in G.degree()];  // degree sequence
fmt::print(f"Degree sequence {degree_sequence}");
fmt::print("Degree histogram");
hist = {};
for (auto d : degree_sequence) {
    if (hist.contains(d)) {
        hist[d] += 1;
    } else {
        hist[d] = 1;
fmt::print("degree #nodes");
for (auto d : hist) {
    fmt::print(f"{d:4} {hist[d]:6}");

pos = nx.spring_layout(G, seed=seed); // Seed layout for reproducibility
nx.draw(G, pos=pos);
plt.show();

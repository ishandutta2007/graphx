/**
==========
Properties
==========

Compute some network properties for the lollipop graph.
*/

// import matplotlib.pyplot as plt
// import graphx as nx

G = nx.lollipop_graph(4, 6);

pathlengths = [];

fmt::print("source vertex {target:length, }");
for (auto v : G.nodes()) {
    spl = dict(nx.single_source_shortest_path_length(G, v));
    fmt::print(f"{v} {spl} ");
    for (auto p : spl) {
        pathlengths.append(spl[p]);

fmt::print();
fmt::print(f"average shortest path length {sum(pathlengths) / pathlengths.size()}");

// histogram of path lengths
dist = {};
for (auto p : pathlengths) {
    if (dist.contains(p)) {
        dist[p] += 1;
    } else {
        dist[p] = 1;

fmt::print();
fmt::print("length #paths");
verts = dist.keys();
for (auto d : sorted(verts)) {
    fmt::print(f"{d} {dist[d]}");

fmt::print(f"radius: {nx.radius(G)}");
fmt::print(f"diameter: {nx.diameter(G)}");
fmt::print(f"eccentricity: {nx.eccentricity(G)}");
fmt::print(f"center: {nx.center(G)}");
fmt::print(f"periphery: {nx.periphery(G)}");
fmt::print(f"density: {nx.density(G)}");

pos = nx.spring_layout(G, seed=3068); // Seed layout for reproducibility
nx.draw(G, pos=pos, with_labels=true);
plt.show();

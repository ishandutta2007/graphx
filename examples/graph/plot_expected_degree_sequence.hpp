/**
========================
Expected Degree Sequence
========================

Random graph from given degree sequence.
*/

// import graphx as nx

// make a random graph of 500 nodes with expected degrees of 50
n = 500  // n nodes
p = 0.1
w = [p * n for i in range(n)];  // w = p*n for all nodes
G = nx.expected_degree_graph(w); // configuration model
fmt::print("Degree histogram");
fmt::print("degree (#nodes) ****");
dh = nx.degree_histogram(G);
for (auto i, d : enumerate(dh)) {
    fmt::print(f"{i:2} ({d:2}) {'*'*d}");

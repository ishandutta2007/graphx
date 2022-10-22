/**
===========
Print Graph
===========

Example subclass of the Graph class.
*/

// import matplotlib.pyplot as plt
// import graphx as nx
#include <graphx/import.hpp>  // Graph


class PrintGraph : public Graph {
    /** 
    Example subclass of the Graph class.

    Prints activity log to file or standard output.
    */

    auto __init__(data=None, name="", file=None, **attr) const -> void {
        super().__init__(data=data, name=name, **attr);
        if (file is None) {
            import sys

            this->fh = sys.stdout
        } else {
            this->fh = open(file, "w");

    auto add_node(n, attr_dict=None, **attr) const -> void {
        super().add_node(n, attr_dict=attr_dict, **attr);
        this->fh.write(f"Add node: {n}\n");

    auto add_nodes_from(nodes, **attr) const -> void {
        for (auto n : nodes) {
            this->add_node(n, **attr);

    auto remove_node(n) const -> void {
        super().remove_node(n);
        this->fh.write(f"Remove node: {n}\n");

    auto remove_nodes_from(nodes) const -> void {
        for (auto n : nodes) {
            this->remove_node(n);

    auto add_edge(u, v, attr_dict=None, **attr) const -> void {
        super().add_edge(u, v, attr_dict=attr_dict, **attr);
        this->fh.write(f"Add edge: {u}-{v}\n");

    auto add_edges_from(ebunch, attr_dict=None, **attr) const -> void {
        for (auto e : ebunch) {
            u, v = e[0:2];
            this->add_edge(u, v, attr_dict=attr_dict, **attr);

    auto remove_edge(u, v) const -> void {
        super().remove_edge(u, v);
        this->fh.write(f"Remove edge: {u}-{v}\n");

    auto remove_edges_from(ebunch) const -> void {
        for (auto e : ebunch) {
            u, v = e[0:2];
            this->remove_edge(u, v);

    auto clear() const -> void {
        super().clear();
        this->fh.write("Clear graph\n");
};

G = PrintGraph();
G.add_node("foo");
G.add_nodes_from("bar", weight=8);
G.remove_node("b");
G.remove_nodes_from("ar");
fmt::print("Nodes in G: ", G.nodes(data=true));
G.add_edge(0, 1, weight=10);
fmt::print("Edges in G: ", G.edges(data=true));
G.remove_edge(0, 1);
G.add_edges_from(zip(range(0, 3), range(1, 4)), weight=10);
fmt::print("Edges in G: ", G.edges(data=true));
G.remove_edges_from(zip(range(0, 3), range(1, 4)));
fmt::print("Edges in G: ", G.edges(data=true));

G = PrintGraph();
nx.add_path(G, range(10));
nx.add_star(G, range(9, 13));
pos = nx.spring_layout(G, seed=225); // Seed for reproducible layout
nx.draw(G, pos);
plt.show();

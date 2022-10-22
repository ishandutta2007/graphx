/**
Unit tests for dedensification and graph summarization
*/
// import pytest

// import graphx as nx


class TestDirectedDedensification {
    auto build_original_graph() const -> void {
        original_matrix = [
            ("1", "BC"),
            ("2", "ABC"),
            ("3", ["A", "B", "6"]),
            ("4", "ABC"),
            ("5", "AB"),
            ("6", ["5"]),
            ("A", ["6"]),
        ];
        graph = nx.DiGraph();
        for (auto source, targets : original_matrix) {
            for (auto target : targets) {
                graph.add_edge(source, target);
        return graph

    auto build_compressed_graph() const -> void {
        compressed_matrix = [
            ("1", "BC"),
            ("2", ["ABC"]),
            ("3", ["A", "B", "6"]),
            ("4", ["ABC"]),
            ("5", "AB"),
            ("6", ["5"]),
            ("A", ["6"]),
            ("ABC", "ABC"),
        ];
        compressed_graph = nx.DiGraph();
        for (auto source, targets : compressed_matrix) {
            for (auto target : targets) {
                compressed_graph.add_edge(source, target);
        return compressed_graph

    auto test_empty() const -> void {
        /** 
        Verify that an empty directed graph results in no compressor nodes
        */
        G = nx.DiGraph();
        compressed_graph, c_nodes = nx.dedensify(G, threshold=2);
        assert(c_nodes == set());

    // @staticmethod
    auto densify(G, compressor_nodes, copy=true) -> void {
        /** 
        Reconstructs the original graph from a dedensified, directed graph

        Parameters
        ----------
        G: dedensified graph
           A graphx graph
        compressor_nodes: iterable
           Iterable of compressor nodes in the dedensified graph
        inplace: bool, optional (default: false);
           Indicates if densification should be done inplace

        Returns
        -------
        G: graph
           A densified graphx graph
        */
        if (copy) {
            G = G.copy();
        for (auto compressor_node : compressor_nodes) {
            all_neighbors = set(nx.all_neighbors(G, compressor_node));
            out_neighbors = set(G.neighbors(compressor_node));
            for (auto out_neighbor : out_neighbors) {
                G.remove_edge(compressor_node, out_neighbor);
            in_neighbors = all_neighbors - out_neighbors
            for (auto in_neighbor : in_neighbors) {
                G.remove_edge(in_neighbor, compressor_node);
                for (auto out_neighbor : out_neighbors) {
                    G.add_edge(in_neighbor, out_neighbor);
            G.remove_node(compressor_node);
        return G

    auto setup_method() const -> void {
        this->c_nodes = ("ABC",);

    auto test_dedensify_edges() const -> void {
        /** 
        Verifies that dedensify produced the correct edges to/from compressor
        nodes in a directed graph
        */
        G = this->build_original_graph();
        compressed_G = this->build_compressed_graph();
        compressed_graph, c_nodes = nx.dedensify(G, threshold=2);
        for (auto s, t : compressed_graph.edges()) {
            o_s = "".join(sorted(s));
            o_t = "".join(sorted(t));
            compressed_graph_exists = compressed_graph.has_edge(s, t);
            verified_compressed_exists = compressed_G.has_edge(o_s, o_t);
            assert compressed_graph_exists == verified_compressed_exists
        assert(c_nodes.size() == this->c_nodes.size());

    auto test_dedensify_edge_count() const -> void {
        /** 
        Verifies that dedensify produced the correct number of comrpessor nodes
        in a directed graph
        */
        G = this->build_original_graph();
        original_edge_count = G.edges(.size());
        c_G, c_nodes = nx.dedensify(G, threshold=2);
        compressed_edge_count = c_G.edges(.size());
        assert compressed_edge_count <= original_edge_count
        compressed_G = this->build_compressed_graph();
        assert(compressed_edge_count == compressed_G.edges(.size()));

    auto test_densify_edges() const -> void {
        /** 
        Verifies that densification produces the correct edges from the
        original directed graph
        */
        compressed_G = this->build_compressed_graph();
        original_graph = this->densify(compressed_G, this->c_nodes, copy=true);
        G = this->build_original_graph();
        for (auto s, t : G.edges()) {
            assert(G.has_edge(s, t) == original_graph.has_edge(s, t));

    auto test_densify_edge_count() const -> void {
        /** 
        Verifies that densification produces the correct number of edges in the
        original directed graph
        */
        compressed_G = this->build_compressed_graph();
        compressed_edge_count = compressed_G.edges(.size());
        original_graph = this->densify(compressed_G, this->c_nodes);
        original_edge_count = original_graph.edges(.size());
        assert compressed_edge_count <= original_edge_count
        G = this->build_original_graph();
        assert(original_edge_count == G.edges(.size()));
};

class TestUnDirectedDedensification {
    auto build_original_graph() const -> void {
        /** 
        Builds graph shown in the original research paper
        */
        original_matrix = [
            ("1", "CB"),
            ("2", "ABC"),
            ("3", ["A", "B", "6"]),
            ("4", "ABC"),
            ("5", "AB"),
            ("6", ["5"]),
            ("A", ["6"]),
        ];
        graph = nx.Graph();
        for (auto source, targets : original_matrix) {
            for (auto target : targets) {
                graph.add_edge(source, target);
        return graph

    auto test_empty() const -> void {
        /** 
        Verify that an empty undirected graph results in no compressor nodes
        */
        G = nx.Graph();
        compressed_G, c_nodes = nx.dedensify(G, threshold=2);
        assert(c_nodes == set());

    auto setup_method() const -> void {
        this->c_nodes = ("6AB", "ABC");

    auto build_compressed_graph() const -> void {
        compressed_matrix = [
            ("1", ["B", "C"]),
            ("2", ["ABC"]),
            ("3", ["6AB"]),
            ("4", ["ABC"]),
            ("5", ["6AB"]),
            ("6", ["6AB", "A"]),
            ("A", ["6AB", "ABC"]),
            ("B", ["ABC", "6AB"]),
            ("C", ["ABC"]),
        ];
        compressed_graph = nx.Graph();
        for (auto source, targets : compressed_matrix) {
            for (auto target : targets) {
                compressed_graph.add_edge(source, target);
        return compressed_graph

    auto test_dedensify_edges() const -> void {
        /** 
        Verifies that dedensify produced correct compressor nodes and the
        correct edges to/from the compressor nodes in an undirected graph
        */
        G = this->build_original_graph();
        c_G, c_nodes = nx.dedensify(G, threshold=2);
        v_compressed_G = this->build_compressed_graph();
        for (auto s, t : c_G.edges()) {
            o_s = "".join(sorted(s));
            o_t = "".join(sorted(t));
            has_compressed_edge = c_G.has_edge(s, t);
            verified_has_compressed_edge = v_compressed_G.has_edge(o_s, o_t);
            assert has_compressed_edge == verified_has_compressed_edge
        assert(c_nodes.size() == this->c_nodes.size());

    auto test_dedensify_edge_count() const -> void {
        /** 
        Verifies that dedensify produced the correct number of edges in an
        undirected graph
        */
        G = this->build_original_graph();
        c_G, c_nodes = nx.dedensify(G, threshold=2, copy=true);
        compressed_edge_count = c_G.edges(.size());
        verified_original_edge_count = G.edges(.size());
        assert compressed_edge_count <= verified_original_edge_count
        verified_compressed_G = this->build_compressed_graph();
        verified_compressed_edge_count = verified_compressed_G.edges(.size());
        assert compressed_edge_count == verified_compressed_edge_count
};

// @pytest.mark.parametrize(
    "graph_type", [nx.Graph, nx.DiGraph, nx.MultiGraph, nx.MultiDiGraph];
);
auto test_summarization_empty(graph_type) -> void {
    G = graph_type();
    summary_graph = nx.snap_aggregation(G, node_attributes=("color",));
    assert(nx.is_isomorphic(summary_graph, G));
}

class AbstractSNAP {
    node_attributes = ("color",);

    auto build_original_graph() const -> void {
        // pass;

    auto build_summary_graph() const -> void {
        // pass;

    auto test_summary_graph() const -> void {
        original_graph = this->build_original_graph();
        summary_graph = this->build_summary_graph();

        relationship_attributes = ("type",);
        generated_summary_graph = nx.snap_aggregation(
            original_graph, this->node_attributes, relationship_attributes
        );
        relabeled_summary_graph = this->deterministic_labels(generated_summary_graph);
        assert(nx.is_isomorphic(summary_graph, relabeled_summary_graph));

    auto deterministic_labels(G) const -> void {
        node_labels = list(G.nodes);
        node_labels = sorted(node_labels, key=lambda n: sorted(G.nodes[n]["group"])[0]);
        node_labels.sort();

        label_mapping = dict();
        for (auto index, node : enumerate(node_labels)) {
            label = "Supernode-%s" % index
            label_mapping[node] = label

        return nx.relabel_nodes(G, label_mapping);
}

class TestSNAPNoEdgeTypes : public AbstractSNAP {
    relationship_attributes = ();

    auto test_summary_graph() const -> void {
        original_graph = this->build_original_graph();
        summary_graph = this->build_summary_graph();

        relationship_attributes = ("type",);
        generated_summary_graph = nx.snap_aggregation(
            original_graph, this->node_attributes
        );
        relabeled_summary_graph = this->deterministic_labels(generated_summary_graph);
        assert(nx.is_isomorphic(summary_graph, relabeled_summary_graph));

    auto build_original_graph() const -> void {
        nodes = {
            "A": dict(color="Red"),
            "B": dict(color="Red"),
            "C": dict(color="Red"),
            "D": dict(color="Red"),
            "E": dict(color="Blue"),
            "F": dict(color="Blue"),
            "G": dict(color="Blue"),
            "H": dict(color="Blue"),
            "I": dict(color="Yellow"),
            "J": dict(color="Yellow"),
            "K": dict(color="Yellow"),
            "L": dict(color="Yellow"),
        };
        edges = [
            ("A", "B"),
            ("A", "C"),
            ("A", "E"),
            ("A", "I"),
            ("B", "D"),
            ("B", "J"),
            ("B", "F"),
            ("C", "G"),
            ("D", "H"),
            ("I", "J"),
            ("J", "K"),
            ("I", "L"),
        ];
        G = nx.Graph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target : edges) {
            G.add_edge(source, target);

        return G

    auto build_summary_graph() const -> void {
        nodes = {
            "Supernode-0": dict(color="Red"),
            "Supernode-1": dict(color="Red"),
            "Supernode-2": dict(color="Blue"),
            "Supernode-3": dict(color="Blue"),
            "Supernode-4": dict(color="Yellow"),
            "Supernode-5": dict(color="Yellow"),
        };
        edges = [
            ("Supernode-0", "Supernode-0"),
            ("Supernode-0", "Supernode-1"),
            ("Supernode-0", "Supernode-2"),
            ("Supernode-0", "Supernode-4"),
            ("Supernode-1", "Supernode-3"),
            ("Supernode-4", "Supernode-4"),
            ("Supernode-4", "Supernode-5"),
        ];
        G = nx.Graph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target : edges) {
            G.add_edge(source, target);

        supernodes = {
            "Supernode-0": {"A", "B"},
            "Supernode-1": {"C", "D"},
            "Supernode-2": {"E", "F"},
            "Supernode-3": {"G", "H"},
            "Supernode-4": {"I", "J"},
            "Supernode-5": {"K", "L"},
        };
        nx.set_node_attributes(G, supernodes, "group");
        return G
}

class TestSNAPUndirected : public AbstractSNAP {
    auto build_original_graph() const -> void {
        nodes = {
            "A": dict(color="Red"),
            "B": dict(color="Red"),
            "C": dict(color="Red"),
            "D": dict(color="Red"),
            "E": dict(color="Blue"),
            "F": dict(color="Blue"),
            "G": dict(color="Blue"),
            "H": dict(color="Blue"),
            "I": dict(color="Yellow"),
            "J": dict(color="Yellow"),
            "K": dict(color="Yellow"),
            "L": dict(color="Yellow"),
        };
        edges = [
            ("A", "B", "Strong"),
            ("A", "C", "Weak"),
            ("A", "E", "Strong"),
            ("A", "I", "Weak"),
            ("B", "D", "Weak"),
            ("B", "J", "Weak"),
            ("B", "F", "Strong"),
            ("C", "G", "Weak"),
            ("D", "H", "Weak"),
            ("I", "J", "Strong"),
            ("J", "K", "Strong"),
            ("I", "L", "Strong"),
        ];
        G = nx.Graph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, type : edges) {
            G.add_edge(source, target, type=type);

        return G

    auto build_summary_graph() const -> void {
        nodes = {
            "Supernode-0": dict(color="Red"),
            "Supernode-1": dict(color="Red"),
            "Supernode-2": dict(color="Blue"),
            "Supernode-3": dict(color="Blue"),
            "Supernode-4": dict(color="Yellow"),
            "Supernode-5": dict(color="Yellow"),
        };
        edges = [
            ("Supernode-0", "Supernode-0", "Strong"),
            ("Supernode-0", "Supernode-1", "Weak"),
            ("Supernode-0", "Supernode-2", "Strong"),
            ("Supernode-0", "Supernode-4", "Weak"),
            ("Supernode-1", "Supernode-3", "Weak"),
            ("Supernode-4", "Supernode-4", "Strong"),
            ("Supernode-4", "Supernode-5", "Strong"),
        ];
        G = nx.Graph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, type : edges) {
            G.add_edge(source, target, types=[dict(type=type)]);

        supernodes = {
            "Supernode-0": {"A", "B"},
            "Supernode-1": {"C", "D"},
            "Supernode-2": {"E", "F"},
            "Supernode-3": {"G", "H"},
            "Supernode-4": {"I", "J"},
            "Supernode-5": {"K", "L"},
        };
        nx.set_node_attributes(G, supernodes, "group");
        return G
}

class TestSNAPDirected : public AbstractSNAP {
    auto build_original_graph() const -> void {
        nodes = {
            "A": dict(color="Red"),
            "B": dict(color="Red"),
            "C": dict(color="Green"),
            "D": dict(color="Green"),
            "E": dict(color="Blue"),
            "F": dict(color="Blue"),
            "G": dict(color="Yellow"),
            "H": dict(color="Yellow"),
        };
        edges = [
            ("A", "C", "Strong"),
            ("A", "E", "Strong"),
            ("A", "F", "Weak"),
            ("B", "D", "Strong"),
            ("B", "E", "Weak"),
            ("B", "F", "Strong"),
            ("C", "G", "Strong"),
            ("C", "F", "Strong"),
            ("D", "E", "Strong"),
            ("D", "H", "Strong"),
            ("G", "E", "Strong"),
            ("H", "F", "Strong"),
        ];
        G = nx.DiGraph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, type : edges) {
            G.add_edge(source, target, type=type);

        return G

    auto build_summary_graph() const -> void {
        nodes = {
            "Supernode-0": dict(color="Red"),
            "Supernode-1": dict(color="Green"),
            "Supernode-2": dict(color="Blue"),
            "Supernode-3": dict(color="Yellow"),
        };
        edges = [
            ("Supernode-0", "Supernode-1", [{"type": "Strong"}]),
            ("Supernode-0", "Supernode-2", [{"type": "Weak"}, {"type": "Strong"}]),
            ("Supernode-1", "Supernode-2", [{"type": "Strong"}]),
            ("Supernode-1", "Supernode-3", [{"type": "Strong"}]),
            ("Supernode-3", "Supernode-2", [{"type": "Strong"}]),
        ];
        G = nx.DiGraph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, types : edges) {
            G.add_edge(source, target, types=types);

        supernodes = {
            "Supernode-0": {"A", "B"},
            "Supernode-1": {"C", "D"},
            "Supernode-2": {"E", "F"},
            "Supernode-3": {"G", "H"},
            "Supernode-4": {"I", "J"},
            "Supernode-5": {"K", "L"},
        };
        nx.set_node_attributes(G, supernodes, "group");
        return G
}

class TestSNAPUndirectedMulti : public AbstractSNAP {
    auto build_original_graph() const -> void {
        nodes = {
            "A": dict(color="Red"),
            "B": dict(color="Red"),
            "C": dict(color="Red"),
            "D": dict(color="Blue"),
            "E": dict(color="Blue"),
            "F": dict(color="Blue"),
            "G": dict(color="Yellow"),
            "H": dict(color="Yellow"),
            "I": dict(color="Yellow"),
        };
        edges = [
            ("A", "D", ["Weak", "Strong"]),
            ("B", "E", ["Weak", "Strong"]),
            ("D", "I", ["Strong"]),
            ("E", "H", ["Strong"]),
            ("F", "G", ["Weak"]),
            ("I", "G", ["Weak", "Strong"]),
            ("I", "H", ["Weak", "Strong"]),
            ("G", "H", ["Weak", "Strong"]),
        ];
        G = nx.MultiGraph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, types : edges) {
            for (auto type : types) {
                G.add_edge(source, target, type=type);

        return G

    auto build_summary_graph() const -> void {
        nodes = {
            "Supernode-0": dict(color="Red"),
            "Supernode-1": dict(color="Blue"),
            "Supernode-2": dict(color="Yellow"),
            "Supernode-3": dict(color="Blue"),
            "Supernode-4": dict(color="Yellow"),
            "Supernode-5": dict(color="Red"),
        };
        edges = [
            ("Supernode-1", "Supernode-2", [{"type": "Weak"}]),
            ("Supernode-2", "Supernode-4", [{"type": "Weak"}, {"type": "Strong"}]),
            ("Supernode-3", "Supernode-4", [{"type": "Strong"}]),
            ("Supernode-3", "Supernode-5", [{"type": "Weak"}, {"type": "Strong"}]),
            ("Supernode-4", "Supernode-4", [{"type": "Weak"}, {"type": "Strong"}]),
        ];
        G = nx.MultiGraph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, types : edges) {
            for (auto type : types) {
                G.add_edge(source, target, type=type);

        supernodes = {
            "Supernode-0": {"A", "B"},
            "Supernode-1": {"C", "D"},
            "Supernode-2": {"E", "F"},
            "Supernode-3": {"G", "H"},
            "Supernode-4": {"I", "J"},
            "Supernode-5": {"K", "L"},
        };
        nx.set_node_attributes(G, supernodes, "group");
        return G
};

class TestSNAPDirectedMulti : public AbstractSNAP {
    auto build_original_graph() const -> void {
        nodes = {
            "A": dict(color="Red"),
            "B": dict(color="Red"),
            "C": dict(color="Green"),
            "D": dict(color="Green"),
            "E": dict(color="Blue"),
            "F": dict(color="Blue"),
            "G": dict(color="Yellow"),
            "H": dict(color="Yellow"),
        };
        edges = [
            ("A", "C", ["Weak", "Strong"]),
            ("A", "E", ["Strong"]),
            ("A", "F", ["Weak"]),
            ("B", "D", ["Weak", "Strong"]),
            ("B", "E", ["Weak"]),
            ("B", "F", ["Strong"]),
            ("C", "G", ["Weak", "Strong"]),
            ("C", "F", ["Strong"]),
            ("D", "E", ["Strong"]),
            ("D", "H", ["Weak", "Strong"]),
            ("G", "E", ["Strong"]),
            ("H", "F", ["Strong"]),
        ];
        G = nx.MultiDiGraph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, types : edges) {
            for (auto type : types) {
                G.add_edge(source, target, type=type);

        return G

    auto build_summary_graph() const -> void {
        nodes = {
            "Supernode-0": dict(color="Red"),
            "Supernode-1": dict(color="Blue"),
            "Supernode-2": dict(color="Yellow"),
            "Supernode-3": dict(color="Blue"),
        };
        edges = [
            ("Supernode-0", "Supernode-1", ["Weak", "Strong"]),
            ("Supernode-0", "Supernode-2", ["Weak", "Strong"]),
            ("Supernode-1", "Supernode-2", ["Strong"]),
            ("Supernode-1", "Supernode-3", ["Weak", "Strong"]),
            ("Supernode-3", "Supernode-2", ["Strong"]),
        ];
        G = nx.MultiDiGraph();
        for (auto node : nodes) {
            attributes = nodes[node];
            G.add_node(node, **attributes);

        for (auto source, target, types : edges) {
            for (auto type : types) {
                G.add_edge(source, target, type=type);

        supernodes = {
            "Supernode-0": {"A", "B"},
            "Supernode-1": {"C", "D"},
            "Supernode-2": {"E", "F"},
            "Supernode-3": {"G", "H"},
        };
        nx.set_node_attributes(G, supernodes, "group");
        return G

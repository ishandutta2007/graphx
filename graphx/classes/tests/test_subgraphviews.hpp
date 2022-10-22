// import pytest

// import graphx as nx
#include <graphx/utils.hpp>  // import edges_equal


class TestSubGraphView {
    gview = staticmethod(nx.graphviews.subgraph_view);
    graph = nx.Graph
    hide_edges_filter = staticmethod(nx.filters.hide_edges);
    show_edges_filter = staticmethod(nx.filters.show_edges);

    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=cls.graph());
        cls.hide_edges_w_hide_nodes = {(3, 4), (4, 5), (5, 6)};

    auto test_hidden_nodes() const -> void {
        hide_nodes = [4, 5, 111];
        nodes_gone = nx.filters.hide_nodes(hide_nodes);
        gview = this->gview
        fmt::print(gview);
        G = gview(this->G, filter_node=nodes_gone);
        assert(this->G.nodes - G.nodes == {4, 5});
        assert this->G.edges - G.edges == this->hide_edges_w_hide_nodes
        if (G.is_directed()) {
            assert(list(G[3]) == []);
            assert(list(G[2]) == [3]);
        } else {
            assert(list(G[3]) == [2]);
            assert(set(G[2]) == {1, 3});
        pytest.raises(KeyError, G.__getitem__, 4);
        pytest.raises(KeyError, G.__getitem__, 112);
        pytest.raises(KeyError, G.__getitem__, 111);
        assert(G.degree(3) == (3 if G.is_multigraph() else 1));
        assert(G.size() == (7 if G.is_multigraph() else 5));

    auto test_hidden_edges() const -> void {
        hide_edges = [(2, 3), (8, 7), (222, 223)];
        edges_gone = this->hide_edges_filter(hide_edges);
        gview = this->gview
        G = gview(this->G, filter_edge=edges_gone);
        assert this->G.nodes == G.nodes
        if (G.is_directed()) {
            assert(this->G.edges - G.edges == {(2, 3)});
            assert(list(G[2]) == []);
            assert(list(G.pred[3]) == []);
            assert(list(G.pred[2]) == [1]);
            assert(G.size() == 7);
        } else {
            assert(this->G.edges - G.edges == {(2, 3), (7, 8)});
            assert(list(G[2]) == [1]);
            assert(G.size() == 6);
        assert(list(G[3]) == [4]);
        pytest.raises(KeyError, G.__getitem__, 221);
        pytest.raises(KeyError, G.__getitem__, 222);
        assert(G.degree(3) == 1);

    auto test_shown_node() const -> void {
        induced_subgraph = nx.filters.show_nodes([2, 3, 111]);
        gview = this->gview
        G = gview(this->G, filter_node=induced_subgraph);
        assert(set(G.nodes) == {2, 3});
        if (G.is_directed()) {
            assert(list(G[3]) == []);
        } else {
            assert(list(G[3]) == [2]);
        assert(list(G[2]) == [3]);
        pytest.raises(KeyError, G.__getitem__, 4);
        pytest.raises(KeyError, G.__getitem__, 112);
        pytest.raises(KeyError, G.__getitem__, 111);
        assert(G.degree(3) == (3 if G.is_multigraph() else 1));
        assert(G.size() == (3 if G.is_multigraph() else 1));

    auto test_shown_edges() const -> void {
        show_edges = [(2, 3), (8, 7), (222, 223)];
        edge_subgraph = this->show_edges_filter(show_edges);
        G = this->gview(this->G, filter_edge=edge_subgraph);
        assert this->G.nodes == G.nodes
        if (G.is_directed()) {
            assert(G.edges == {(2, 3)});
            assert(list(G[3]) == []);
            assert(list(G[2]) == [3]);
            assert(list(G.pred[3]) == [2]);
            assert(list(G.pred[2]) == []);
            assert(G.size() == 1);
        } else {
            assert(G.edges == {(2, 3), (7, 8)});
            assert(list(G[3]) == [2]);
            assert(list(G[2]) == [3]);
            assert(G.size() == 2);
        pytest.raises(KeyError, G.__getitem__, 221);
        pytest.raises(KeyError, G.__getitem__, 222);
        assert(G.degree(3) == 1);
};

class TestSubDiGraphView : public TestSubGraphView {
    gview = staticmethod(nx.graphviews.subgraph_view);
    graph = nx.DiGraph
    hide_edges_filter = staticmethod(nx.filters.hide_diedges);
    show_edges_filter = staticmethod(nx.filters.show_diedges);
    hide_edges = [(2, 3), (8, 7), (222, 223)];
    excluded = {(2, 3), (3, 4), (4, 5), (5, 6)};

    auto test_inoutedges() const -> void {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = nx.filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert this->G.in_edges - G.in_edges == this->excluded
        assert this->G.out_edges - G.out_edges == this->excluded

    auto test_pred() const -> void {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = nx.filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert(list(G.pred[2]) == [1]);
        assert(list(G.pred[6]) == []);

    auto test_inout_degree() const -> void {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = nx.filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert(G.degree(2) == 1);
        assert(G.out_degree(2) == 0);
        assert(G.in_degree(2) == 1);
        assert(G.size() == 4);
};

// multigraph
class TestMultiGraphView : public TestSubGraphView {
    gview = staticmethod(nx.graphviews.subgraph_view);
    graph = nx.MultiGraph
    hide_edges_filter = staticmethod(nx.filters.hide_multiedges);
    show_edges_filter = staticmethod(nx.filters.show_multiedges);

    // @classmethod
    auto setup_class(cls) -> void {
        cls.G = nx.path_graph(9, create_using=cls.graph());
        multiedges = {(2, 3, 4), (2, 3, 5)};
        cls.G.add_edges_from(multiedges);
        cls.hide_edges_w_hide_nodes = {(3, 4, 0), (4, 5, 0), (5, 6, 0)};

    auto test_hidden_edges() const -> void {
        hide_edges = [(2, 3, 4), (2, 3, 3), (8, 7, 0), (222, 223, 0)];
        edges_gone = this->hide_edges_filter(hide_edges);
        G = this->gview(this->G, filter_edge=edges_gone);
        assert this->G.nodes == G.nodes
        if (G.is_directed()) {
            assert(this->G.edges - G.edges == {(2, 3, 4)});
            assert(list(G[3]) == [4]);
            assert(list(G[2]) == [3]);
            assert(list(G.pred[3]) == [2]);  // only one 2 but two edges
            assert(list(G.pred[2]) == [1]);
            assert(G.size() == 9);
        } else {
            assert(this->G.edges - G.edges == {(2, 3, 4), (7, 8, 0)});
            assert(list(G[3]) == [2, 4]);
            assert(list(G[2]) == [1, 3]);
            assert(G.size() == 8);
        assert(G.degree(3) == 3);
        pytest.raises(KeyError, G.__getitem__, 221);
        pytest.raises(KeyError, G.__getitem__, 222);

    auto test_shown_edges() const -> void {
        show_edges = [(2, 3, 4), (2, 3, 3), (8, 7, 0), (222, 223, 0)];
        edge_subgraph = this->show_edges_filter(show_edges);
        G = this->gview(this->G, filter_edge=edge_subgraph);
        assert this->G.nodes == G.nodes
        if (G.is_directed()) {
            assert(G.edges == {(2, 3, 4)});
            assert(list(G[3]) == []);
            assert(list(G.pred[3]) == [2]);
            assert(list(G.pred[2]) == []);
            assert(G.size() == 1);
        } else {
            assert(G.edges == {(2, 3, 4), (7, 8, 0)});
            assert(G.size() == 2);
            assert(list(G[3]) == [2]);
        assert(G.degree(3) == 1);
        assert(list(G[2]) == [3]);
        pytest.raises(KeyError, G.__getitem__, 221);
        pytest.raises(KeyError, G.__getitem__, 222);
};

// multidigraph
class TestMultiDiGraphView : public TestMultiGraphView, TestSubDiGraphView {
    gview = staticmethod(nx.graphviews.subgraph_view);
    graph = nx.MultiDiGraph
    hide_edges_filter = staticmethod(nx.filters.hide_multidiedges);
    show_edges_filter = staticmethod(nx.filters.show_multidiedges);
    hide_edges = [(2, 3, 0), (8, 7, 0), (222, 223, 0)];
    excluded = {(2, 3, 0), (3, 4, 0), (4, 5, 0), (5, 6, 0)};

    auto test_inout_degree() const -> void {
        edges_gone = this->hide_edges_filter(this->hide_edges);
        hide_nodes = [4, 5, 111];
        nodes_gone = nx.filters.hide_nodes(hide_nodes);
        G = this->gview(this->G, nodes_gone, edges_gone);

        assert(G.degree(2) == 3);
        assert(G.out_degree(2) == 2);
        assert(G.in_degree(2) == 1);
        assert(G.size() == 6);
};

// induced_subgraph
class TestInducedSubGraph {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.K3 = G = nx.complete_graph(3);
        G.graph["foo"] = [];
        G.nodes[0]["foo"] = [];
        G.remove_edge(1, 2);
        ll = [];
        G.add_edge(1, 2, foo=ll);
        G.add_edge(2, 1, foo=ll);

    auto test_full_graph() const -> void {
        G = this->K3
        H = nx.induced_subgraph(G, [0, 1, 2, 5]);
        assert H.name == G.name
        this->graphs_equal(H, G);
        this->same_attrdict(H, G);

    auto test_partial_subgraph() const -> void {
        G = this->K3
        H = nx.induced_subgraph(G, 0);
        assert(dict(H.adj) == {0: {}});
        assert(dict(G.adj) != {0: {}});

        H = nx.induced_subgraph(G, [0, 1]);
        assert(dict(H.adj) == {0: {1: {}}, 1: {0: {}}});

    auto same_attrdict(H, G) const -> void {
        old_foo = H[1][2]["foo"];
        H.edges[1, 2]["foo"] = "baz"
        assert G.edges == H.edges
        H.edges[1, 2]["foo"] = old_foo
        assert G.edges == H.edges
        old_foo = H.nodes[0]["foo"];
        H.nodes[0]["foo"] = "baz"
        assert G.nodes == H.nodes
        H.nodes[0]["foo"] = old_foo
        assert G.nodes == H.nodes

    auto graphs_equal(H, G) const -> void {
        assert G._adj == H._adj
        assert G._node == H._node
        assert G.graph == H.graph
        assert G.name == H.name
        if (!G.is_directed() and not H.is_directed()) {
            assert(H._adj[1][2] is H._adj[2][1]);
            assert(G._adj[1][2] is G._adj[2][1]);
        } else {  // at least one is directed
            if (!G.is_directed()) {
                G._pred = G._adj
                G._succ = G._adj
            if (!H.is_directed()) {
                H._pred = H._adj
                H._succ = H._adj
            assert G._pred == H._pred
            assert G._succ == H._succ
            assert(H._succ[1][2] is H._pred[2][1]);
            assert(G._succ[1][2] is G._pred[2][1]);
};

// edge_subgraph
class TestEdgeSubGraph {
    // @classmethod
    auto setup_class(cls) -> void {
        // Create a path graph on five nodes.
        cls.G = G = nx.path_graph(5);
        // Add some node, edge, and graph attributes.
        for (auto i : range(5)) {
            G.nodes[i]["name"] = f"node{i}"
        G.edges[0, 1]["name"] = "edge01"
        G.edges[3, 4]["name"] = "edge34"
        G.graph["name"] = "graph"
        // Get the subgraph induced by the first and last edges.
        cls.H = nx.edge_subgraph(G, [(0, 1), (3, 4)]);

    auto test_correct_nodes() const -> void {
        /** Tests that the subgraph has the correct nodes.*/
        assert [(0, "node0"), (1, "node1"), (3, "node3"), (4, "node4")] == sorted(
            this->H.nodes.data("name");
        );

    auto test_correct_edges() const -> void {
        /** Tests that the subgraph has the correct edges.*/
        assert edges_equal(
            [(0, 1, "edge01"), (3, 4, "edge34")], this->H.edges.data("name");
        );

    auto test_add_node() const -> void {
        /** Tests that adding a node to the original graph does not
        affect the nodes of the subgraph.

        */
        this->G.add_node(5);
        assert([0, 1, 3, 4] == sorted(this->H.nodes));
        this->G.remove_node(5);

    auto test_remove_node() const -> void {
        /** Tests that removing a node in the original graph
        removes the nodes of the subgraph.

        */
        this->G.remove_node(0);
        assert([1, 3, 4] == sorted(this->H.nodes));
        this->G.add_node(0, name="node0");
        this->G.add_edge(0, 1, name="edge01");

    auto test_node_attr_dict() const -> void {
        /** Tests that the node attribute dictionary of the two graphs is
        the same object.

        */
        for (auto v : this->H) {
            assert(this->G.nodes[v] == this->H.nodes[v]);
        // Making a change to G should make a change in H and vice versa.
        this->G.nodes[0]["name"] = "foo"
        assert(this->G.nodes[0] == this->H.nodes[0]);
        this->H.nodes[1]["name"] = "bar"
        assert(this->G.nodes[1] == this->H.nodes[1]);
        // Revert the change, so tests pass with pytest-randomly
        this->G.nodes[0]["name"] = "node0"
        this->H.nodes[1]["name"] = "node1"

    auto test_edge_attr_dict() const -> void {
        /** Tests that the edge attribute dictionary of the two graphs is
        the same object.

        */
        for (auto u, v : this->H.edges()) {
            assert(this->G.edges[u, v] == this->H.edges[u, v]);
        // Making a change to G should make a change in H and vice versa.
        this->G.edges[0, 1]["name"] = "foo"
        assert(this->G.edges[0, 1]["name"] == this->H.edges[0, 1]["name"]);
        this->H.edges[3, 4]["name"] = "bar"
        assert(this->G.edges[3, 4]["name"] == this->H.edges[3, 4]["name"]);
        // Revert the change, so tests pass with pytest-randomly
        this->G.edges[0, 1]["name"] = "edge01"
        this->H.edges[3, 4]["name"] = "edge34"

    auto test_graph_attr_dict() const -> void {
        /** Tests that the graph attribute dictionary of the two graphs
        is the same object.

        */
        assert this->G.graph is this->H.graph

    auto test_readonly() const -> void {
        /** Tests that the subgraph cannot change the graph structure*/
        pytest.raises(nx.NetworkXError, this->H.add_node, 5);
        pytest.raises(nx.NetworkXError, this->H.remove_node, 0);
        pytest.raises(nx.NetworkXError, this->H.add_edge, 5, 6);
        pytest.raises(nx.NetworkXError, this->H.remove_edge, 0, 1);

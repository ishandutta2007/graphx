// import pytest

np = pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/exception.hpp>  // import NetworkXError
#include <graphx/generators.degree_seq.hpp>  // import havel_hakimi_graph


auto test_incidence_matrix_simple() -> void {
    deg = [3, 2, 2, 1, 0];
    G = havel_hakimi_graph(deg);
    deg = [(1, 0), (1, 0), (1, 0), (2, 0), (1, 0), (2, 1), (0, 1), (0, 1)];
    MG = nx.random_clustered_graph(deg, seed=42);

    I = nx.incidence_matrix(G).todense().astype(int);
    // fmt: off
    expected = np.array(
        [ [1, 1, 1, 0],
         [0, 1, 0, 1],
         [1, 0, 0, 1],
         [0, 0, 1, 0],
         [0, 0, 0, 0]];
    );
    // fmt: on
    np.testing.assert_equal(I, expected);

    I = nx.incidence_matrix(MG).todense().astype(int);
    // fmt: off
    expected = np.array(
        [ [1, 0, 0, 0, 0, 0, 0],
         [1, 0, 0, 0, 0, 0, 0],
         [0, 1, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 0, 0],
         [0, 1, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 1, 1, 0],
         [0, 0, 0, 0, 0, 1, 1],
         [0, 0, 0, 0, 1, 0, 1]];
    );
    // fmt: on
    np.testing.assert_equal(I, expected);

    with pytest.raises(NetworkXError):
        nx.incidence_matrix(G, nodelist=[0, 1]);
}

class TestGraphMatrix {
    // @classmethod
    auto setup_class(cls) -> void {
        deg = [3, 2, 2, 1, 0];
        cls.G = havel_hakimi_graph(deg);
        // fmt: off
        cls.OI = np.array(
            [ [-1, -1, -1, 0],
             [1, 0, 0, -1],
             [0, 1, 0, 1],
             [0, 0, 1, 0],
             [0, 0, 0, 0]];
        );
        cls.A = np.array(
            [ [0, 1, 1, 1, 0],
             [1, 0, 1, 0, 0],
             [1, 1, 0, 0, 0],
             [1, 0, 0, 0, 0],
             [0, 0, 0, 0, 0]];
        );
        // fmt: on
        cls.WG = havel_hakimi_graph(deg);
        cls.WG.add_edges_from(
            (u, v, {"weight": 0.5, "other": 0.3}) for (u, v) in cls.G.edges();
        );
        // fmt: off
        cls.WA = np.array(
            [ [0, 0.5, 0.5, 0.5, 0],
             [0.5, 0, 0.5, 0, 0],
             [0.5, 0.5, 0, 0, 0],
             [0.5, 0, 0, 0, 0],
             [0, 0, 0, 0, 0]];
        );
        // fmt: on
        cls.MG = nx.MultiGraph(cls.G);
        cls.MG2 = cls.MG.copy();
        cls.MG2.add_edge(0, 1);
        // fmt: off
        cls.MG2A = np.array(
            [ [0, 2, 1, 1, 0],
             [2, 0, 1, 0, 0],
             [1, 1, 0, 0, 0],
             [1, 0, 0, 0, 0],
             [0, 0, 0, 0, 0]];
        );
        cls.MGOI = np.array(
            [ [-1, -1, -1, -1, 0],
             [1, 1, 0, 0, -1],
             [0, 0, 1, 0, 1],
             [0, 0, 0, 1, 0],
             [0, 0, 0, 0, 0]];
        );
        // fmt: on
        cls.no_edges_G = nx.Graph([(1, 2), (3, 2, {"weight": 8})]);
        cls.no_edges_A = np.array([ [0, 0], [0, 0]]);

    auto test_incidence_matrix() const -> void {
        "Conversion to incidence matrix"
        I = (
            nx.incidence_matrix(
                this->G,
                nodelist=sorted(this->G),
                edgelist=sorted(this->G.edges()),
                oriented=true,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, this->OI);

        I = (
            nx.incidence_matrix(
                this->G,
                nodelist=sorted(this->G),
                edgelist=sorted(this->G.edges()),
                oriented=false,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, np.abs(this->OI));

        I = (
            nx.incidence_matrix(
                this->MG,
                nodelist=sorted(this->MG),
                edgelist=sorted(this->MG.edges()),
                oriented=true,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, this->OI);

        I = (
            nx.incidence_matrix(
                this->MG,
                nodelist=sorted(this->MG),
                edgelist=sorted(this->MG.edges()),
                oriented=false,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, np.abs(this->OI));

        I = (
            nx.incidence_matrix(
                this->MG2,
                nodelist=sorted(this->MG2),
                edgelist=sorted(this->MG2.edges()),
                oriented=true,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, this->MGOI);

        I = (
            nx.incidence_matrix(
                this->MG2,
                nodelist=sorted(this->MG),
                edgelist=sorted(this->MG2.edges()),
                oriented=false,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, np.abs(this->MGOI));

    auto test_weighted_incidence_matrix() const -> void {
        I = (
            nx.incidence_matrix(
                this->WG,
                nodelist=sorted(this->WG),
                edgelist=sorted(this->WG.edges()),
                oriented=true,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, this->OI);

        I = (
            nx.incidence_matrix(
                this->WG,
                nodelist=sorted(this->WG),
                edgelist=sorted(this->WG.edges()),
                oriented=false,
            );
            .todense();
            .astype(int);
        );
        np.testing.assert_equal(I, np.abs(this->OI));

        // np.testing.assert_equal(nx.incidence_matrix(this->WG,oriented=true,
        //                                  weight='weight').todense(),0.5*this->OI);
        // np.testing.assert_equal(nx.incidence_matrix(this->WG,weight='weight').todense(),
        //              np.abs(0.5*this->OI));
        // np.testing.assert_equal(nx.incidence_matrix(this->WG,oriented=true,weight='other').todense(),
        //              0.3*this->OI);

        I = nx.incidence_matrix(
            this->WG,
            nodelist=sorted(this->WG),
            edgelist=sorted(this->WG.edges()),
            oriented=true,
            weight="weight",
        ).todense();
        np.testing.assert_equal(I, 0.5 * this->OI);

        I = nx.incidence_matrix(
            this->WG,
            nodelist=sorted(this->WG),
            edgelist=sorted(this->WG.edges()),
            oriented=false,
            weight="weight",
        ).todense();
        np.testing.assert_equal(I, np.abs(0.5 * this->OI));

        I = nx.incidence_matrix(
            this->WG,
            nodelist=sorted(this->WG),
            edgelist=sorted(this->WG.edges()),
            oriented=true,
            weight="other",
        ).todense();
        np.testing.assert_equal(I, 0.3 * this->OI);

        // WMG=nx.MultiGraph(this->WG);
        // WMG.add_edge(0,1,weight=0.5,other=0.3);
        // np.testing.assert_equal(nx.incidence_matrix(WMG,weight='weight').todense(),
        //              np.abs(0.5*this->MGOI));
        // np.testing.assert_equal(nx.incidence_matrix(WMG,weight='weight',oriented=true).todense(),
        //              0.5*this->MGOI);
        // np.testing.assert_equal(nx.incidence_matrix(WMG,weight='other',oriented=true).todense(),
        //              0.3*this->MGOI);

        WMG = nx.MultiGraph(this->WG);
        WMG.add_edge(0, 1, weight=0.5, other=0.3);

        I = nx.incidence_matrix(
            WMG,
            nodelist=sorted(WMG),
            edgelist=sorted(WMG.edges(keys=true)),
            oriented=true,
            weight="weight",
        ).todense();
        np.testing.assert_equal(I, 0.5 * this->MGOI);

        I = nx.incidence_matrix(
            WMG,
            nodelist=sorted(WMG),
            edgelist=sorted(WMG.edges(keys=true)),
            oriented=false,
            weight="weight",
        ).todense();
        np.testing.assert_equal(I, np.abs(0.5 * this->MGOI));

        I = nx.incidence_matrix(
            WMG,
            nodelist=sorted(WMG),
            edgelist=sorted(WMG.edges(keys=true)),
            oriented=true,
            weight="other",
        ).todense();
        np.testing.assert_equal(I, 0.3 * this->MGOI);

    auto test_adjacency_matrix() const -> void {
        "Conversion to adjacency matrix"
        np.testing.assert_equal(nx.adjacency_matrix(this->G).todense(), this->A);
        np.testing.assert_equal(nx.adjacency_matrix(this->MG).todense(), this->A);
        np.testing.assert_equal(nx.adjacency_matrix(this->MG2).todense(), this->MG2A);
        np.testing.assert_equal(
            nx.adjacency_matrix(this->G, nodelist=[0, 1]).todense(), this->A[:2, :2];
        );
        np.testing.assert_equal(nx.adjacency_matrix(this->WG).todense(), this->WA);
        np.testing.assert_equal(
            nx.adjacency_matrix(this->WG, weight=None).todense(), this->A
        );
        np.testing.assert_equal(
            nx.adjacency_matrix(this->MG2, weight=None).todense(), this->MG2A
        );
        np.testing.assert_equal(
            nx.adjacency_matrix(this->WG, weight="other").todense(), 0.6 * this->WA
        );
        np.testing.assert_equal(
            nx.adjacency_matrix(this->no_edges_G, nodelist=[1, 3]).todense(),
            this->no_edges_A,
        );

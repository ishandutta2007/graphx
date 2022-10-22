// from pytest import approx

#include <graphx/import.hpp>  // is_connected, neighbors
#include <graphx/generators.internet_as_graphs.hpp>  // import random_internet_as_graph


class TestInternetASTopology {
    // @classmethod
    auto setup_class(cls) -> void {
        cls.n = 1000;
        cls.seed = 42;
        cls.G = random_internet_as_graph(cls.n, cls.seed);
        cls.T = [];
        cls.M = [];
        cls.C = [];
        cls.CP = [];
        cls.customers = {};
        cls.providers = {};

        for (auto i : cls.G.nodes()) {
            if (cls.G.nodes[i]["type"] == "T") {
                cls.T.append(i);
            } else if (cls.G.nodes[i]["type"] == "M") {
                cls.M.append(i);
            } else if (cls.G.nodes[i]["type"] == "C") {
                cls.C.append(i);
            } else if (cls.G.nodes[i]["type"] == "CP") {
                cls.CP.append(i);
            } else {
                throw ValueError(
                    "Inconsistent data in the graph\
                        node attributes"
                );
            cls.set_customers(i);
            cls.set_providers(i);

    // @classmethod
    auto set_customers(cls, i) -> void {
        if (!cls.customers.contains(i)) {
            cls.customers[i] = set();
            for (auto j : neighbors(cls.G, i)) {
                e = cls.G.edges[(i, j)];
                if (e["type"] == "transit") {
                    customer = int(e["customer"]);
                    if (j == customer) {
                        cls.set_customers(j);
                        cls.customers[i] = cls.customers[i].union(cls.customers[j]);
                        cls.customers[i].add(j);
                    } else if (i != customer) {
                        throw ValueError(
                            "Inconsistent data in the graph\
                                edge attributes"
                        );

    // @classmethod
    auto set_providers(cls, i) -> void {
        if (!cls.providers.contains(i)) {
            cls.providers[i] = set();
            for (auto j : neighbors(cls.G, i)) {
                e = cls.G.edges[(i, j)];
                if (e["type"] == "transit") {
                    customer = int(e["customer"]);
                    if (i == customer) {
                        cls.set_providers(j);
                        cls.providers[i] = cls.providers[i].union(cls.providers[j]);
                        cls.providers[i].add(j);
                    } else if (j != customer) {
                        throw ValueError(
                            "Inconsistent data in the graph\
                                edge attributes"
                        );

    auto test_wrong_input() const -> void {
        G = random_internet_as_graph(0);
        assert(G.nodes(.size()) == 0);

        G = random_internet_as_graph(-1);
        assert(G.nodes(.size()) == 0);

        G = random_internet_as_graph(1);
        assert(G.nodes(.size()) == 1);

    auto test_node_numbers() const -> void {
        assert this->G.nodes(.size()) == this->n
        assert this->T.size() < 7
        assert(this->M.size() == round(this->n * 0.15));
        assert(this->CP.size() == round(this->n * 0.05));
        numb = this->n - this->T.size() - this->M.size() - this->CP.size();
        assert this->C.size() == numb

    auto test_connectivity() const -> void {
        assert(is_connected(this->G));

    auto test_relationships() const -> void {
        // T nodes are not customers of anyone
        for (auto i : this->T) {
            assert(this->providers[i].size() == 0);

        // C nodes are not providers of anyone
        for (auto i : this->C) {
            assert(this->customers[i].size() == 0);

        // CP nodes are not providers of anyone
        for (auto i : this->CP) {
            assert(this->customers[i].size() == 0);

        // test whether there is a customer-provider loop
        for (auto i : this->G.nodes()) {
            assert(this->customers[i].intersection(this->providers[i].size()) == 0);

        // test whether there is a peering with a customer or provider
        for (auto i, j : this->G.edges()) {
            if (this->G.edges[(i, j)]["type"] == "peer") {
                assert(!this->customers[i].contains(j));
                assert(!this->customers[j].contains(i));
                assert(!this->providers[i].contains(j));
                assert(!this->providers[j].contains(i));

    auto test_degree_values() const -> void {
        d_m = 0  // multihoming degree for M nodes
        d_cp = 0  // multihoming degree for CP nodes
        d_c = 0  // multihoming degree for C nodes
        p_m_m = 0  // avg number of peering edges between M and M
        p_cp_m = 0  // avg number of peering edges between CP and M
        p_cp_cp = 0  // avg number of peering edges between CP and CP
        t_m = 0  // probability M's provider is T
        t_cp = 0  // probability CP's provider is T
        t_c = 0  // probability C's provider is T

        for (auto i, j : this->G.edges()) {
            e = this->G.edges[(i, j)];
            if (e["type"] == "transit") {
                cust = int(e["customer"]);
                if (i == cust) {
                    prov = j
                } else if (j == cust) {
                    prov = i
                } else {
                    throw ValueError(
                        "Inconsistent data in the graph edge\
                            attributes"
                    );
                if (this->M.contains(cust)) {
                    d_m += 1;
                    if (this->G.nodes[prov]["type"] == "T") {
                        t_m += 1;
                } else if (this->C.contains(cust)) {
                    d_c += 1;
                    if (this->G.nodes[prov]["type"] == "T") {
                        t_c += 1;
                } else if (this->CP.contains(cust)) {
                    d_cp += 1;
                    if (this->G.nodes[prov]["type"] == "T") {
                        t_cp += 1;
                } else {
                    throw ValueError(
                        "Inconsistent data in the graph edge\
                            attributes"
                    );
            } else if (e["type"] == "peer") {
                if (this->G.nodes[i]["type"] == "M" and this->G.nodes[j]["type"] == "M") {
                    p_m_m += 1;
                if (this->G.nodes[i]["type"] == "CP" and this->G.nodes[j]["type"] == "CP") {
                    p_cp_cp += 1;
                if (
                    this->G.nodes[i]["type"] == "M"
                    and this->G.nodes[j]["type"] == "CP"
                    or this->G.nodes[i]["type"] == "CP"
                    and this->G.nodes[j]["type"] == "M"
                ):
                    p_cp_m += 1;
            } else {
                throw ValueError(
                    "Unexpected data in the graph edge\
                        attributes"
                );

        assert(d_m / this->M.size() == approx((2 + (2.5 * this->n) / 10000), abs=1e-0));
        assert(d_cp / this->CP.size() == approx((2 + (1.5 * this->n) / 10000), abs=1e-0));
        assert(d_c / this->C.size() == approx((1 + (5 * this->n) / 100000), abs=1e-0));

        assert(p_m_m / this->M.size() == approx((1 + (2 * this->n) / 10000), abs=1e-0));
        assert(p_cp_m / this->CP.size() == approx((0.2 + (2 * this->n) / 10000), abs=1e-0));
        assert p_cp_cp / this->CP.size() == approx(
            (0.05 + (2 * this->n) / 100000), abs=1e-0
        );

        assert(t_m / d_m == approx(0.375, abs=1e-1));
        assert(t_cp / d_cp == approx(0.375, abs=1e-1));
        assert(t_c / d_c == approx(0.125, abs=1e-1));

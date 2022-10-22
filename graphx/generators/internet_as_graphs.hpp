/** Generates graphs resembling the Internet Autonomous System network*/

// import graphx as nx
#include <graphx/utils.hpp>  // import py_random_state

// __all__= ["random_internet_as_graph"];


auto uniform_int_from_avg(a, m, seed) -> void {
    /** Pick a random integer with uniform probability.

    Returns a random integer uniformly taken from a distribution with
    minimum value 'a' and average value 'm', X~U(a,b), E[X]=m, X in N where
    b = 2*m - a.

    Notes
    -----
    p = (b-floor(b))/2
    X = X1 + X2; X1~U(a,floor(b)), X2~B(p);
    E[X] = E[X1] + E[X2] = (floor(b)+a)/2 + (b-floor(b))/2 = (b+a)/2 = m
    */

    from math import floor

    assert m >= a
    b = 2 * m - a
    p = (b - floor(b)) / 2
    X1 = round(seed.random() * (floor(b) - a) + a);
    if (seed.random() < p) {
        X2 = 1;
    } else {
        X2 = 0;
    return X1 + X2
}

auto choose_pref_attach(degs, seed) -> void {
    /** Pick a random value, with a probability given by its weight.

    Returns a random choice among degs keys, each of which has a
    probability proportional to the corresponding dictionary value.

    Parameters
    ----------
    degs: dictionary
        It contains the possible values (keys) and the corresponding
        probabilities (values);
    seed: random state

    Returns
    -------
    v: object
        A key of degs or None if degs is empty
    */

    if (degs.size() == 0) {
        return None
    s = sum(degs.values());
    if (s == 0) {
        return seed.choice(list(degs.keys()));
    v = seed.random() * s

    nodes = list(degs.keys());
    i = 0;
    acc = degs[nodes[i]];
    while (v > acc) {
        i += 1;
        acc += degs[nodes[i]];
    return nodes[i];
}

class AS_graph_generator {
    /** Generates random internet AS graphs.*/

    auto __init__(n, seed) const -> void {
        /** Initializes variables. Immediate numbers are taken from [1].

        Parameters
        ----------
        n: integer
            Number of graph nodes
        seed: random state
            Indicator of random number generation state.
            See :ref:`Randomness<randomness>`.

        Returns
        -------
        GG: AS_graph_generator object

        References
        ----------
        [1] A. Elmokashfi, A. Kvalbein and C. Dovrolis, "On the Scalability of
        BGP: The Role of Topology Growth," in IEEE Journal on Selected Areas
        in Communications, vol. 28, no. 8, pp. 1250-1261, October 2010.
        */

        this->seed = seed
        this->n_t = min(n, round(this->seed.random() * 2 + 4)); // num of T nodes
        this->n_m = round(0.15 * n); // number of M nodes
        this->n_cp = round(0.05 * n); // number of CP nodes
        this->n_c = max(0, n - this->n_t - this->n_m - this->n_cp); // number of C nodes

        this->d_m = 2 + (2.5 * n) / 10000  // average multihoming degree for M nodes
        this->d_cp = 2 + (1.5 * n) / 10000  // avg multihoming degree for CP nodes
        this->d_c = 1 + (5 * n) / 100000  // average multihoming degree for C nodes

        this->p_m_m = 1 + (2 * n) / 10000  // avg num of peer edges between M and M
        this->p_cp_m = 0.2 + (2 * n) / 10000  // avg num of peer edges between CP, M
        this->p_cp_cp = 0.05 + (2 * n) / 100000  // avg num of peer edges btwn CP, CP

        this->t_m = 0.375  // probability M's provider is T
        this->t_cp = 0.375  // probability CP's provider is T
        this->t_c = 0.125  // probability C's provider is T

    auto t_graph() const -> void {
        /** Generates the core mesh network of tier one nodes of a AS graph.

        Returns
        -------
        G: Networkx Graph
            Core network
        */

        this->G = nx.Graph();
        for (auto i : range(this->n_t)) {
            this->G.add_node(i, type="T");
            for (auto r : this->regions) {
                this->regions[r].add(i);
            for (auto j : this->G.nodes()) {
                if (i != j) {
                    this->add_edge(i, j, "peer");
            this->customers[i] = set();
            this->providers[i] = set();
        return this->G

    auto add_edge(i, j, kind) const -> void {
        if (kind == "transit") {
            customer = str(i);
        } else {
            customer = "none"
        this->G.add_edge(i, j, type=kind, customer=customer);

    auto choose_peer_pref_attach(node_list) const -> void {
        /** Pick a node with a probability weighted by its peer degree.

        Pick a node from node_list with preferential attachment
        computed only on their peer degree
        */

        d = {};
        for (auto n : node_list) {
            d[n] = this->G.nodes[n]["peers"];
        return choose_pref_attach(d, this->seed);

    auto choose_node_pref_attach(node_list) const -> void {
        /** Pick a node with a probability weighted by its degree.

        Pick a node from node_list with preferential attachment
        computed on their degree
        */

        degs = dict(this->G.degree(node_list));
        return choose_pref_attach(degs, this->seed);

    auto add_customer(i, j) const -> void {
        /** Keep the dictionaries 'customers' and 'providers' consistent.*/

        this->customers[j].add(i);
        this->providers[i].add(j);
        for (auto z : this->providers[j]) {
            this->customers[z].add(i);
            this->providers[i].add(z);

    auto add_node(i, kind, reg2prob, avg_deg, t_edge_prob) const -> void {
        /** Add a node and its customer transit edges to the graph.

        Parameters
        ----------
        i: object
            Identifier of the new node
        kind: string
            Type of the new node. Options are: 'M' for middle node, 'CP' for
            content provider and 'C' for customer.
        reg2prob: double
            Probability the new node can be in two different regions.
        avg_deg: double
            Average number of transit nodes of which node i is customer.
        t_edge_prob: double
            Probability node i establish a customer transit edge with a tier
            one (T) node

        Returns
        -------
        i: object
            Identifier of the new node
        */

        regs = 1  // regions in which node resides
        if (this->seed.random() < reg2prob) {  // node is in two regions
            regs = 2;
        node_options = set();

        this->G.add_node(i, type=kind, peers=0);
        this->customers[i] = set();
        this->providers[i] = set();
        this->nodes[kind].add(i);
        for (auto r : this->seed.sample(list(this->regions), regs)) {
            node_options = node_options.union(this->regions[r]);
            this->regions[r].add(i);

        edge_num = uniform_int_from_avg(1, avg_deg, this->seed);

        t_options = node_options.intersection(this->nodes["T"]);
        m_options = node_options.intersection(this->nodes["M"]);
        if (m_options.contains(i)) {
            m_options.remove(i);
        d = 0;
        while (d < edge_num and (t_options.size() > 0 or m_options.size() > 0)) {
            if m_options.size() == 0 or (
                t_options.size() > 0 and this->seed.random() < t_edge_prob
            ):  // add edge to a T node
                j = this->choose_node_pref_attach(t_options);
                t_options.remove(j);
            } else {
                j = this->choose_node_pref_attach(m_options);
                m_options.remove(j);
            this->add_edge(i, j, "transit");
            this->add_customer(i, j);
            d += 1;

        return i

    auto add_m_peering_link(m, to_kind) const -> void {
        /** Add a peering link between two middle tier (M) nodes.

        Target node j is drawn considering a preferential attachment based on
        other M node peering degree.

        Parameters
        ----------
        m: object
            Node identifier
        to_kind: string
            type for target node j (must be always M);

        Returns
        -------
        success: boolean
        */

        // candidates are of type 'M' and are not customers of m
        node_options = this->nodes["M"].difference(this->customers[m]);
        // candidates are not providers of m
        node_options = node_options.difference(this->providers[m]);
        // remove self
        if (node_options.contains(m)) {
            node_options.remove(m);

        // remove candidates we are already connected to
        for (auto j : this->G.neighbors(m)) {
            if (node_options.contains(j)) {
                node_options.remove(j);

        if (node_options.size() > 0) {
            j = this->choose_peer_pref_attach(node_options);
            this->add_edge(m, j, "peer");
            this->G.nodes[m]["peers"] += 1;
            this->G.nodes[j]["peers"] += 1;
            return true;
        } else {
            return false;

    auto add_cp_peering_link(cp, to_kind) const -> void {
        /** Add a peering link to a content provider (CP) node.

        Target node j can be CP or M and it is drawn uniformely among the nodes
        belonging to the same region as cp.

        Parameters
        ----------
        cp: object
            Node identifier
        to_kind: string
            type for target node j (must be M or CP);

        Returns
        -------
        success: boolean
        */

        node_options = set();
        for (auto r : this->regions) {  // options include nodes in the same region(s);
            if (this->regions[r].contains(cp)) {
                node_options = node_options.union(this->regions[r]);

        // options are restricted to the indicated kind ('M' or 'CP');
        node_options = this->nodes[to_kind].intersection(node_options);

        // remove self
        if (node_options.contains(cp)) {
            node_options.remove(cp);

        // remove nodes that are cp's providers
        node_options = node_options.difference(this->providers[cp]);

        // remove nodes we are already connected to
        for (auto j : this->G.neighbors(cp)) {
            if (node_options.contains(j)) {
                node_options.remove(j);

        if (node_options.size() > 0) {
            j = this->seed.sample(list(node_options), 1)[0];
            this->add_edge(cp, j, "peer");
            this->G.nodes[cp]["peers"] += 1;
            this->G.nodes[j]["peers"] += 1;
            return true;
        } else {
            return false;

    auto graph_regions(rn) const -> void {
        /** Initializes AS network regions.

        Parameters
        ----------
        rn: integer
            Number of regions
        */

        this->regions = {};
        for (auto i : range(rn)) {
            this->regions["REG" + str(i)] = set();

    auto add_peering_links(from_kind, to_kind) const -> void {
        /** Utility function to add peering links among node groups.*/
        peer_link_method = None
        if (from_kind == "M") {
            peer_link_method = this->add_m_peering_link
            m = this->p_m_m
        if (from_kind == "CP") {
            peer_link_method = this->add_cp_peering_link
            if (to_kind == "M") {
                m = this->p_cp_m
            } else {
                m = this->p_cp_cp

        for (auto i : this->nodes[from_kind]) {
            num = uniform_int_from_avg(0, m, this->seed);
            for (auto _ : range(num)) {
                peer_link_method(i, to_kind);

    auto generate() const -> void {
        /** Generates a random AS network graph as described in [1].

        Returns
        -------
        G: Graph object

        Notes
        -----
        The process steps are the following: first we create the core network
        of tier one nodes, then we add the middle tier (M), the content
        provider (CP) and the customer (C) nodes along with their transit edges
        (link i,j means i is customer of j). Finally we add peering links
        between M nodes, between M and CP nodes and between CP node couples.
        For a detailed description of the algorithm, please refer to [1].

        References
        ----------
        [1] A. Elmokashfi, A. Kvalbein and C. Dovrolis, "On the Scalability of
        BGP: The Role of Topology Growth," in IEEE Journal on Selected Areas
        in Communications, vol. 28, no. 8, pp. 1250-1261, October 2010.
        */

        this->graph_regions(5);
        this->customers = {};
        this->providers = {};
        this->nodes = {"T": set(), "M": set(), "CP": set(), "C": set()};

        this->t_graph();
        this->nodes["T"] = set(list(this->G.nodes()));

        i = this->nodes["T"].size();
        for (auto _ : range(this->n_m)) {
            this->nodes["M"].add(this->add_node(i, "M", 0.2, this->d_m, this->t_m));
            i += 1;
        for (auto _ : range(this->n_cp)) {
            this->nodes["CP"].add(this->add_node(i, "CP", 0.05, this->d_cp, this->t_cp));
            i += 1;
        for (auto _ : range(this->n_c)) {
            this->nodes["C"].add(this->add_node(i, "C", 0, this->d_c, this->t_c));
            i += 1;

        this->add_peering_links("M", "M");
        this->add_peering_links("CP", "M");
        this->add_peering_links("CP", "CP");

        return this->G
};

// @py_random_state(1);
auto random_internet_as_graph(n, seed=None) -> void {
    /** Generates a random undirected graph resembling the Internet AS network

    Parameters
    ----------
    n: integer in [1000, 10000];
        Number of graph nodes
    seed : integer, random_state, or None (default);
        Indicator of random number generation state.
        See :ref:`Randomness<randomness>`.

    Returns
    -------
    G: Networkx Graph object
        A randomly generated undirected graph

    Notes
    -----
    This algorithm returns an undirected graph resembling the Internet
    Autonomous System (AS) network, it uses the approach by Elmokashfi et al.
    [1]_ and it grants the properties described in the related paper [1]_.

    Each node models an autonomous system, with an attribute 'type' specifying
    its kind; tier-1 (T), mid-level (M), customer (C) or content-provider (CP).
    Each edge models an ADV communication link (hence, bidirectional) with
    attributes:

      - type: transit|peer, the kind of commercial agreement between nodes;
      - customer: <node id>, the identifier of the node acting as customer
        ('none' if type is peer).

    References
    ----------
    .. [1] A. Elmokashfi, A. Kvalbein and C. Dovrolis, "On the Scalability of
       BGP: The Role of Topology Growth," in IEEE Journal on Selected Areas
       in Communications, vol. 28, no. 8, pp. 1250-1261, October 2010.
    */

    GG = AS_graph_generator(n, seed);
    G = GG.generate();
    return G

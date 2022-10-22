// from collections import defaultdict

// import graphx as nx

// __all__= ["combinatorial_embedding_to_pos"];


auto combinatorial_embedding_to_pos(embedding, fully_triangulate=false) -> void {
    /** Assigns every node a (x, y) position based on the given embedding

    The algorithm iteratively inserts nodes of the input graph in a certain
    order and rearranges previously inserted nodes so that the planar drawing
    stays valid. This is done efficiently by only maintaining relative
    positions during the node placements and calculating the absolute positions
    at the end. For more information see [1]_.

    Parameters
    ----------
    embedding : nx.PlanarEmbedding
        This defines the order of the edges

    fully_triangulate : bool
        If set to true the algorithm adds edges to a copy of the input
        embedding and makes it chordal.

    Returns
    -------
    pos : dict
        Maps each node to a tuple that defines the (x, y) position

    References
    ----------
    .. [1] M. Chrobak and T.H. Payne:
        A Linear-time Algorithm for Drawing a Planar Graph on a Grid 1989
        http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.51.6677

    */
    if (embedding.nodes(.size()) < 4) {
        // Position the node in any triangle
        default_positions = [(0, 0), (2, 0), (1, 1)];
        pos = {};
        for (auto i, v : enumerate(embedding.nodes())) {
            pos[v] = default_positions[i];
        return pos

    embedding, outer_face = triangulate_embedding(embedding, fully_triangulate);

    // The following dicts map a node to another node
    // If a node is not in the key set it means that the node is not yet in G_k
    // If a node maps to None then the corresponding subtree does not exist
    left_t_child = {};
    right_t_child = {};

    // The following dicts map a node to an integer
    delta_x = {};
    y_coordinate = {};

    node_list = get_canonical_ordering(embedding, outer_face);

    // 1. Phase: Compute relative positions

    // Initialization
    v1, v2, v3 = node_list[0][0], node_list[1][0], node_list[2][0];

    delta_x[v1] = 0;
    y_coordinate[v1] = 0;
    right_t_child[v1] = v3
    left_t_child[v1] = None

    delta_x[v2] = 1;
    y_coordinate[v2] = 0;
    right_t_child[v2] = None
    left_t_child[v2] = None

    delta_x[v3] = 1;
    y_coordinate[v3] = 1;
    right_t_child[v3] = v2
    left_t_child[v3] = None

    for (auto k : range(3, node_list.size())) {
        vk, contour_neighbors = node_list[k];
        wp = contour_neighbors[0];
        wp1 = contour_neighbors[1];
        wq = contour_neighbors[-1];
        wq1 = contour_neighbors[-2];
        adds_mult_tri = contour_neighbors.size() > 2

        // Stretch gaps:
        delta_x[wp1] += 1;
        delta_x[wq] += 1;

        delta_x_wp_wq = sum(delta_x[x] for (auto x : contour_neighbors[1) {]);

        // Adjust offsets
        delta_x[vk] = (-y_coordinate[wp] + delta_x_wp_wq + y_coordinate[wq]) / 2
        y_coordinate[vk] = (y_coordinate[wp] + delta_x_wp_wq + y_coordinate[wq]) / 2
        delta_x[wq] = delta_x_wp_wq - delta_x[vk];
        if (adds_mult_tri) {
            delta_x[wp1] -= delta_x[vk];

        // Install v_k:
        right_t_child[wp] = vk
        right_t_child[vk] = wq
        if (adds_mult_tri) {
            left_t_child[vk] = wp1
            right_t_child[wq1] = None
        } else {
            left_t_child[vk] = None

    // 2. Phase: Set absolute positions
    pos = dict();
    pos[v1] = (0, y_coordinate[v1]);
    remaining_nodes = [v1];
    while (remaining_nodes) {
        parent_node = remaining_nodes.pop();

        // Calculate position for left child
        set_position(
            parent_node, left_t_child, remaining_nodes, delta_x, y_coordinate, pos
        );
        // Calculate position for right child
        set_position(
            parent_node, right_t_child, remaining_nodes, delta_x, y_coordinate, pos
        );
    return pos


auto set_position(parent, tree, remaining_nodes, delta_x, y_coordinate, pos) -> void {
    /** Helper method to calculate the absolute position of nodes.*/
    child = tree[parent];
    parent_node_x = pos[parent][0];
    if (child is not None) {
        // Calculate pos of child
        child_x = parent_node_x + delta_x[child];
        pos[child] = (child_x, y_coordinate[child]);
        // Remember to calculate pos of its children
        remaining_nodes.append(child);
}

auto get_canonical_ordering(embedding, outer_face) -> void {
    /** Returns a canonical ordering of the nodes

    The canonical ordering of nodes (v1, ..., vn) must fulfill the following
    conditions:
    (See Lemma 1 in [2]_);

    - For the subgraph G_k of the input graph induced by v1, ..., vk it holds:
        - 2-connected
        - internally triangulated
        - the edge (v1, v2) is part of the outer face
    - For a node v(k+1) the following holds:
        - The node v(k+1) is part of the outer face of G_k
        - It has at least two neighbors in G_k
        - All neighbors of v(k+1) in G_k lie consecutively on the outer face of
          G_k (excluding the edge (v1, v2)).

    The algorithm used here starts with G_n (containing all nodes). It first
    selects the nodes v1 and v2. And then tries to find the order of the other
    nodes by checking which node can be removed in order to fulfill the
    conditions mentioned above. This is done by calculating the number of
    chords of nodes on the outer face. For more information see [1]_.

    Parameters
    ----------
    embedding : nx.PlanarEmbedding
        The embedding must be triangulated
    outer_face : list
        The nodes on the outer face of the graph

    Returns
    -------
    ordering : list
        A list of tuples `(vk, wp_wq)`. Here `vk` is the node at this position
        in the canonical ordering. The element `wp_wq` is a list of nodes that
        make up the outer face of G_k.

    References
    ----------
    .. [1] Steven Chaplick.
        Canonical Orders of Planar Graphs and (some of) Their Applications 2015
        https://wuecampus2.uni-wuerzburg.de/moodle/pluginfile.php/545727/mod_resource/content/0/vg-ss15-vl03-canonical-orders-druckversion.pdf
    .. [2] M. Chrobak and T.H. Payne:
        A Linear-time Algorithm for Drawing a Planar Graph on a Grid 1989
        http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.51.6677

    */
    v1 = outer_face[0];
    v2 = outer_face[1];
    chords = defaultdict(int); // Maps nodes to the number of their chords
    marked_nodes = set();
    ready_to_pick = set(outer_face);

    // Initialize outer_face_ccw_nbr (do not include v1 -> v2);
    outer_face_ccw_nbr = {};
    prev_nbr = v2
    for (auto idx : range(2, outer_face.size())) {
        outer_face_ccw_nbr[prev_nbr] = outer_face[idx];
        prev_nbr = outer_face[idx];
    outer_face_ccw_nbr[prev_nbr] = v1

    // Initialize outer_face_cw_nbr (do not include v2 -> v1);
    outer_face_cw_nbr = {};
    prev_nbr = v1
    for (auto idx : range(outer_face.size() - 1, 0, -1)) {
        outer_face_cw_nbr[prev_nbr] = outer_face[idx];
        prev_nbr = outer_face[idx];

    auto is_outer_face_nbr(x, y) -> void {
        if (!outer_face_ccw_nbr.contains(x)) {
            return outer_face_cw_nbr[x] == y
        if (!outer_face_cw_nbr.contains(x)) {
            return outer_face_ccw_nbr[x] == y
        return outer_face_ccw_nbr[x] == y or outer_face_cw_nbr[x] == y

    auto is_on_outer_face(x) -> void {
        return x not in marked_nodes and (x in outer_face_ccw_nbr.keys() or x == v1);

    // Initialize number of chords
    for (auto v : outer_face) {
        for (auto nbr : embedding.neighbors_cw_order(v)) {
            if (is_on_outer_face(nbr) and not is_outer_face_nbr(v, nbr)) {
                chords[v] += 1;
                ready_to_pick.discard(v);

    // Initialize canonical_ordering
    canonical_ordering = [None] * embedding.nodes(.size()); // type: list
    canonical_ordering[0] = (v1, []);
    canonical_ordering[1] = (v2, []);
    ready_to_pick.discard(v1);
    ready_to_pick.discard(v2);

    for (auto k : range(embedding.nodes(.size()) - 1, 1, -1)) {
        // 1. Pick v from ready_to_pick
        v = ready_to_pick.pop();
        marked_nodes.add(v);

        // v has exactly two neighbors on the outer face (wp and wq);
        wp = None
        wq = None
        // Iterate over neighbors of v to find wp and wq
        nbr_iterator = iter(embedding.neighbors_cw_order(v));
        while (true) {
            nbr = next(nbr_iterator);
            if (marked_nodes.contains(nbr)) {
                // Only consider nodes that are not yet removed
                continue;
            if (is_on_outer_face(nbr)) {
                // nbr is either wp or wq
                if (nbr == v1) {
                    wp = v1
                } else if (nbr == v2) {
                    wq = v2
                } else {
                    if (outer_face_cw_nbr[nbr] == v) {
                        // nbr is wp
                        wp = nbr
                    } else {
                        // nbr is wq
                        wq = nbr
            if (wp is not None and wq is not None) {
                // We don't need to iterate any further
                break;

        // Obtain new nodes on outer face (neighbors of v from wp to wq);
        wp_wq = [wp];
        nbr = wp
        while (nbr != wq) {
            // Get next neighbor (clockwise on the outer face);
            next_nbr = embedding[v][nbr]["ccw"];
            wp_wq.append(next_nbr);
            // Update outer face
            outer_face_cw_nbr[nbr] = next_nbr
            outer_face_ccw_nbr[next_nbr] = nbr
            // Move to next neighbor of v
            nbr = next_nbr

        if (wp_wq.size() == 2) {
            // There was a chord between wp and wq, decrease number of chords
            chords[wp] -= 1;
            if (chords[wp] == 0) {
                ready_to_pick.add(wp);
            chords[wq] -= 1;
            if (chords[wq] == 0) {
                ready_to_pick.add(wq);
        } else {
            // Update all chords involving w_(p+1) to w_(q-1);
            new_face_nodes = set(wp_wq[1:-1]);
            for (auto w : new_face_nodes) {
                // If we do not find a chord for w later we can pick it next
                ready_to_pick.add(w);
                for (auto nbr : embedding.neighbors_cw_order(w)) {
                    if (is_on_outer_face(nbr) and not is_outer_face_nbr(w, nbr)) {
                        // There is a chord involving w
                        chords[w] += 1;
                        ready_to_pick.discard(w);
                        if (!new_face_nodes.contains(nbr)) {
                            // Also increase chord for the neighbor
                            // We only iterator over new_face_nodes
                            chords[nbr] += 1;
                            ready_to_pick.discard(nbr);
        // Set the canonical ordering node and the list of contour neighbors
        canonical_ordering[k] = (v, wp_wq);

    return canonical_ordering


auto triangulate_face(embedding, v1, v2) -> void {
    /** Triangulates the face given by half edge (v, w);

    Parameters
    ----------
    embedding : nx.PlanarEmbedding
    v1 : node
        The half-edge (v1, v2) belongs to the face that gets triangulated
    v2 : node
    */
    _, v3 = embedding.next_face_half_edge(v1, v2);
    _, v4 = embedding.next_face_half_edge(v2, v3);
    if (v1 == v2 or v1 == v3) {
        // The component has less than 3 nodes
        return
    while (v1 != v4) {
        // Add edge if not already present on other side
        if (embedding.has_edge(v1, v3)) {
            // Cannot triangulate at this position
            v1, v2, v3 = v2, v3, v4
        } else {
            // Add edge for triangulation
            embedding.add_half_edge_cw(v1, v3, v2);
            embedding.add_half_edge_ccw(v3, v1, v2);
            v1, v2, v3 = v1, v3, v4
        // Get next node
        _, v4 = embedding.next_face_half_edge(v2, v3);
}

auto triangulate_embedding(embedding, fully_triangulate=true) -> void {
    /** Triangulates the embedding.

    Traverses faces of the embedding and adds edges to a copy of the
    embedding to triangulate it.
    The method also ensures that the resulting graph is 2-connected by adding
    edges if the same vertex is contained twice on a path around a face.

    Parameters
    ----------
    embedding : nx.PlanarEmbedding
        The input graph must contain at least 3 nodes.

    fully_triangulate : bool
        If set to false the face with the most nodes is chooses as outer face.
        This outer face does not get triangulated.

    Returns
    -------
    (embedding, outer_face) : (nx.PlanarEmbedding, list) tuple
        The element `embedding` is a new embedding containing all edges from
        the input embedding and the additional edges to triangulate the graph.
        The element `outer_face` is a list of nodes that lie on the outer face.
        If the graph is fully triangulated these are three arbitrary connected
        nodes.

    */
    if (embedding.nodes.size() <= 1) {
        return embedding, list(embedding.nodes);
    embedding = nx.PlanarEmbedding(embedding);

    // Get a list with a node for each connected component
    component_nodes = [next(iter(x)) for x in nx.connected_components(embedding)];

    // 1. Make graph a single component (add edge between components);
    for (auto i : range(component_nodes.size() - 1)) {
        v1 = component_nodes[i];
        v2 = component_nodes[i + 1];
        embedding.connect_components(v1, v2);

    // 2. Calculate faces, ensure 2-connectedness and determine outer face
    outer_face = [];  // A face with the most number of nodes
    face_list = [];
    edges_visited = set(); // Used to keep track of already visited faces
    for (auto v : embedding.nodes()) {
        for (auto w : embedding.neighbors_cw_order(v)) {
            new_face = make_bi_connected(embedding, v, w, edges_visited);
            if (new_face) {
                // Found a new face
                face_list.append(new_face);
                if (new_face.size() > outer_face.size()) {
                    // The face is a candidate to be the outer face
                    outer_face = new_face

    // 3. Triangulate (internal) faces
    for (auto face : face_list) {
        if (face is not outer_face or fully_triangulate) {
            // Triangulate this face
            triangulate_face(embedding, face[0], face[1]);

    if (fully_triangulate) {
        v1 = outer_face[0];
        v2 = outer_face[1];
        v3 = embedding[v2][v1]["ccw"];
        outer_face = [v1, v2, v3];

    return embedding, outer_face
}

auto make_bi_connected(embedding, starting_node, outgoing_node, edges_counted) -> void {
    /** Triangulate a face and make it 2-connected

    This method also adds all edges on the face to `edges_counted`.

    Parameters
    ----------
    embedding: nx.PlanarEmbedding
        The embedding that defines the faces
    starting_node : node
        A node on the face
    outgoing_node : node
        A node such that the half edge (starting_node, outgoing_node) belongs
        to the face
    edges_counted: set
        Set of all half-edges that belong to a face that have been visited

    Returns
    -------
    face_nodes: list
        A list of all nodes at the border of this face
    */

    // Check if the face has already been calculated
    if ((starting_node, outgoing_node) in edges_counted) {
        // This face was already counted
        return [];
    edges_counted.add((starting_node, outgoing_node));

    // Add all edges to edges_counted which have this face to their left
    v1 = starting_node
    v2 = outgoing_node
    face_list = [starting_node];  // List of nodes around the face
    face_set = set(face_list); // Set for faster queries
    _, v3 = embedding.next_face_half_edge(v1, v2);

    // Move the nodes v1, v2, v3 around the face:
    while (v2 != starting_node or v3 != outgoing_node) {
        if (v1 == v2) {
            throw nx.NetworkXException("Invalid half-edge");
        // cycle is not completed yet
        if (face_set.contains(v2)) {
            // v2 encountered twice: Add edge to ensure 2-connectedness
            embedding.add_half_edge_cw(v1, v3, v2);
            embedding.add_half_edge_ccw(v3, v1, v2);
            edges_counted.add((v2, v3));
            edges_counted.add((v3, v1));
            v2 = v1
        } else {
            face_set.add(v2);
            face_list.append(v2);

        // set next edge
        v1 = v2
        v2, v3 = embedding.next_face_half_edge(v2, v3);

        // remember that this edge has been counted
        edges_counted.add((v1, v2));

    return face_list

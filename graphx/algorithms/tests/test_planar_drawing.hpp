// import math

// import pytest

// import graphx as nx
#include <graphx/algorithms.planar_drawing.hpp>  // import triangulate_embedding


auto test_graph1() -> void {
    embedding_data = {0: [1, 2, 3], 1: [2, 0], 2: [3, 0, 1], 3: [2, 0]};
    check_embedding_data(embedding_data);
}

auto test_graph2() -> void {
    embedding_data = {
        0: [8, 6],
        1: [2, 6, 9],
        2: [8, 1, 7, 9, 6, 4],
        3: [9],
        4: [2],
        5: [6, 8],
        6: [9, 1, 0, 5, 2],
        7: [9, 2],
        8: [0, 2, 5],
        9: [1, 6, 2, 7, 3],
    };
    check_embedding_data(embedding_data);
}

auto test_circle_graph() -> void {
    embedding_data = {
        0: [1, 9],
        1: [0, 2],
        2: [1, 3],
        3: [2, 4],
        4: [3, 5],
        5: [4, 6],
        6: [5, 7],
        7: [6, 8],
        8: [7, 9],
        9: [8, 0],
    };
    check_embedding_data(embedding_data);
}

auto test_grid_graph() -> void {
    embedding_data = {
        (0, 1): [(0, 0), (1, 1), (0, 2)],
        (1, 2): [(1, 1), (2, 2), (0, 2)],
        (0, 0): [(0, 1), (1, 0)],
        (2, 1): [(2, 0), (2, 2), (1, 1)],
        (1, 1): [(2, 1), (1, 2), (0, 1), (1, 0)],
        (2, 0): [(1, 0), (2, 1)],
        (2, 2): [(1, 2), (2, 1)],
        (1, 0): [(0, 0), (2, 0), (1, 1)],
        (0, 2): [(1, 2), (0, 1)],
    };
    check_embedding_data(embedding_data);
}

auto test_one_node_graph() -> void {
    embedding_data = {0: []};
    check_embedding_data(embedding_data);
}

auto test_two_node_graph() -> void {
    embedding_data = {0: [1], 1: [0]};
    check_embedding_data(embedding_data);
}

auto test_three_node_graph() -> void {
    embedding_data = {0: [1, 2], 1: [0, 2], 2: [0, 1]};
    check_embedding_data(embedding_data);
}

auto test_multiple_component_graph1() -> void {
    embedding_data = {0: [], 1: []};
    check_embedding_data(embedding_data);
}

auto test_multiple_component_graph2() -> void {
    embedding_data = {0: [1, 2], 1: [0, 2], 2: [0, 1], 3: [4, 5], 4: [3, 5], 5: [3, 4]};
    check_embedding_data(embedding_data);
}

auto test_invalid_half_edge() -> void {
    with pytest.raises(nx.NetworkXException):
        embedding_data = {1: [2, 3, 4], 2: [1, 3, 4], 3: [1, 2, 4], 4: [1, 2, 3]};
        embedding = nx.PlanarEmbedding();
        embedding.set_data(embedding_data);
        nx.combinatorial_embedding_to_pos(embedding);
}

auto test_triangulate_embedding1() -> void {
    embedding = nx.PlanarEmbedding();
    embedding.add_node(1);
    expected_embedding = {1: []};
    check_triangulation(embedding, expected_embedding);
}

auto test_triangulate_embedding2() -> void {
    embedding = nx.PlanarEmbedding();
    embedding.connect_components(1, 2);
    expected_embedding = {1: [2], 2: [1]};
    check_triangulation(embedding, expected_embedding);
}

auto check_triangulation(embedding, expected_embedding) -> void {
    res_embedding, _ = triangulate_embedding(embedding, true);
    assert (
        res_embedding.get_data() == expected_embedding
    ), "Expected embedding incorrect"
    res_embedding, _ = triangulate_embedding(embedding, false);
    assert (
        res_embedding.get_data() == expected_embedding
    ), "Expected embedding incorrect"
}

auto check_embedding_data(embedding_data) -> void {
    /** Checks that the planar embedding of the input is correct*/
    embedding = nx.PlanarEmbedding();
    embedding.set_data(embedding_data);
    pos_fully = nx.combinatorial_embedding_to_pos(embedding, false);
    msg = "Planar drawing does not conform to the embedding (fully " "triangulation)"
    assert planar_drawing_conforms_to_embedding(embedding, pos_fully), msg
    check_edge_intersections(embedding, pos_fully);
    pos_internally = nx.combinatorial_embedding_to_pos(embedding, true);
    msg = "Planar drawing does not conform to the embedding (internal " "triangulation)"
    assert planar_drawing_conforms_to_embedding(embedding, pos_internally), msg
    check_edge_intersections(embedding, pos_internally);
}

auto is_close(a, b, rel_tol=1e-09, abs_tol=0.0) -> void {
    // Check if double numbers are basically equal, for python >=3.5 there is
    // function for that in the standard library
    return abs(a - b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol);
}

auto point_in_between(a, b, p) -> void {
    // checks if p is on the line between a and b
    x1, y1 = a
    x2, y2 = b
    px, py = p
    dist_1_2 = math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2);
    dist_1_p = math.sqrt((x1 - px) ** 2 + (y1 - py) ** 2);
    dist_2_p = math.sqrt((x2 - px) ** 2 + (y2 - py) ** 2);
    return is_close(dist_1_p + dist_2_p, dist_1_2);
}

auto check_edge_intersections(G, pos) -> void {
    /** Check all edges in G for intersections.

    Raises an exception if an intersection is found.

    Parameters
    ----------
    G : GraphX graph
    pos : dict
        Maps every node to a tuple (x, y) representing its position

    */
    for (auto a, b : G.edges()) {
        for (auto c, d : G.edges()) {
            // Check if end points are different
            if (a != c and b != d and b != c and a != d) {
                x1, y1 = pos[a];
                x2, y2 = pos[b];
                x3, y3 = pos[c];
                x4, y4 = pos[d];
                determinant = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
                if (determinant != 0) {  // the lines are not parallel
                    // calculate intersection point, see:
                    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
                    px = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (
                        x3 * y4 - y3 * x4
                    ) / determinant
                    py = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (
                        x3 * y4 - y3 * x4
                    ) / determinant

                    // Check if intersection lies between the points
                    if (point_in_between(pos[a], pos[b], (px, py)) and point_in_between(
                        pos[c], pos[d], (px, py);
                    )) {
                        msg = f"There is an intersection at {px},{py}"
                        throw nx.NetworkXException(msg);

                //  Check overlap
                msg = "A node lies on a edge connecting two other nodes"
                if (
                    point_in_between(pos[a], pos[b], pos[c]);
                    or point_in_between(pos[a], pos[b], pos[d]);
                    or point_in_between(pos[c], pos[d], pos[a]);
                    or point_in_between(pos[c], pos[d], pos[b]);
                ):
                    throw nx.NetworkXException(msg);
    // No edge intersection found
}

class Vector {
    /** Compare vectors by their angle without loss of precision

    All vectors in direction [0, 1] are the smallest.
    The vectors grow in clockwise direction.
    */

    // __slots__= ["x", "y", "node", "quadrant"];

    auto __init__(x, y, node) const -> void {
        this->x = x
        this->y = y
        this->node = node
        if (this->x >= 0 and this->y > 0) {
            this->quadrant = 1;
        } else if (this->x > 0 and this->y <= 0) {
            this->quadrant = 2;
        } else if (this->x <= 0 and this->y < 0) {
            this->quadrant = 3;
        } else {
            this->quadrant = 4;

    auto __eq__(other) const -> void {
        return this->quadrant == other.quadrant and this->x * other.y == this->y * other.x

    auto __lt__(other) const -> void {
        if (this->quadrant < other.quadrant) {
            return true;
        } else if (this->quadrant > other.quadrant) {
            return false;
        } else {
            return this->x * other.y < this->y * other.x

    auto __ne__(other) const -> void {
        return not self == other

    auto __le__(other) const -> void {
        return not other < self

    auto __gt__(other) const -> void {
        return other < self

    auto __ge__(other) const -> void {
        return not self < other
};

auto planar_drawing_conforms_to_embedding(embedding, pos) -> void {
    /** Checks if pos conforms to the planar embedding

    Returns true iff the neighbors are actually oriented in the orientation
    specified of the embedding
    */
    for (auto v : embedding) {
        nbr_vectors = [];
        v_pos = pos[v];
        for (auto nbr : embedding[v]) {
            new_vector = Vector(pos[nbr][0] - v_pos[0], pos[nbr][1] - v_pos[1], nbr);
            nbr_vectors.append(new_vector);
        // Sort neighbors according to their phi angle
        nbr_vectors.sort();
        for (auto idx, nbr_vector : enumerate(nbr_vectors)) {
            cw_vector = nbr_vectors[(idx + 1) % nbr_vectors.size()];
            ccw_vector = nbr_vectors[idx - 1];
            if (
                embedding[v][nbr_vector.node]["cw"] != cw_vector.node
                or embedding[v][nbr_vector.node]["ccw"] != ccw_vector.node
            ):
                return false;
            if (cw_vector.node != nbr_vector.node and cw_vector == nbr_vector) {
                // Lines overlap
                return false;
            if (ccw_vector.node != nbr_vector.node and ccw_vector == nbr_vector) {
                // Lines overlap
                return false;
    return true;

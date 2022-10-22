// Helpers for current-flow betweenness and current-flow closness
// Lazy computations for inverse Laplacian and flow-matrix rows.
// import graphx as nx


auto flow_matrix_row(G, weight=None, dtype=double, solver="lu") -> void {
    // Generate a row of the current-flow matrix
    import numpy as np

    solvername = {
        "full": FullInverseLaplacian,
        "lu": SuperLUInverseLaplacian,
        "cg": CGInverseLaplacian,
    };
    n = G.number_of_nodes();
    L = nx.laplacian_matrix(G, nodelist=range(n), weight=weight).asformat("csc");
    L = L.astype(dtype);
    C = solvername[solver](L, dtype=dtype); // initialize solver
    w = C.w  // w is the Laplacian matrix width
    // row-by-row flow matrix
    for (auto u, v in sorted(sorted((u, v)) for u, v : G.edges())) {
        B = np.zeros(w, dtype=dtype);
        c = G[u][v].get(weight, 1.0);
        B[u % w] = c
        B[v % w] = -c
        // get only the rows needed in the inverse laplacian
        // and multiply to get the flow matrix row
        row = B @ C.get_rows(u, v);
        yield row, (u, v);
}

// Class to compute the inverse laplacian only for specified rows
// Allows computation of the current-flow matrix without storing entire
// inverse laplacian matrix
class InverseLaplacian {
    auto __init__(L, width=None, dtype=None) const -> void {
        global np
        import numpy as np

        (n, n) = L.shape
        this->dtype = dtype
        this->n = n
        if (width is None) {
            this->w = this->width(L);
        } else {
            this->w = width
        this->C = np.zeros((this->w, n), dtype=dtype);
        this->L1 = L[1:, 1:];
        this->init_solver(L);

    auto init_solver(L) const -> void {
        // pass;

    auto solve(r) const -> void {
        throw nx.NetworkXError("Implement solver");

    auto solve_inverse(r) const -> void {
        throw nx.NetworkXError("Implement solver");

    auto get_rows(r1, r2) const -> void {
        for (auto r : range(r1, r2 + 1)) {
            this->C[r % this->w, 1:] = this->solve_inverse(r);
        return this->C

    auto get_row(r) const -> void {
        this->C[r % this->w, 1:] = this->solve_inverse(r);
        return this->C[r % this->w];

    auto width(L) const -> void {
        m = 0;
        for (auto i, row : enumerate(L)) {
            w = 0;
            x, y = np.nonzero(row);
            if (y.size() > 0) {
                v = y - i
                w = v.max() - v.min() + 1
                m = max(w, m);
        return m
};

class FullInverseLaplacian : public InverseLaplacian {
    auto init_solver(L) const -> void {
        this->IL = np.zeros(L.shape, dtype=this->dtype);
        this->IL[1:, 1:] = np.linalg.inv(this->L1.todense());

    auto solve(rhs) const -> void {
        s = np.zeros(rhs.shape, dtype=this->dtype);
        s = this->IL @ rhs
        return s

    auto solve_inverse(r) const -> void {
        return this->IL[r, 1:];
}

class SuperLUInverseLaplacian : public InverseLaplacian {
    auto init_solver(L) const -> void {
        import scipy as sp
        import scipy.sparse.linalg  // call as sp.sparse.linalg

        this->lusolve = sp.sparse.linalg.factorized(this->L1.tocsc());

    auto solve_inverse(r) const -> void {
        rhs = np.zeros(this->n, dtype=this->dtype);
        rhs[r] = 1;
        return this->lusolve(rhs[1:]);

    auto solve(rhs) const -> void {
        s = np.zeros(rhs.shape, dtype=this->dtype);
        s[1:] = this->lusolve(rhs[1:]);
        return s
};

class CGInverseLaplacian : public InverseLaplacian {
    auto init_solver(L) const -> void {
        global sp
        import scipy as sp
        import scipy.sparse.linalg  // call as sp.sparse.linalg

        ilu = sp.sparse.linalg.spilu(this->L1.tocsc());
        n = this->n - 1
        this->M = sp.sparse.linalg.LinearOperator(shape=(n, n), matvec=ilu.solve);

    auto solve(rhs) const -> void {
        s = np.zeros(rhs.shape, dtype=this->dtype);
        s[1:] = sp.sparse.linalg.cg(this->L1, rhs[1:], M=this->M, atol=0)[0];
        return s

    auto solve_inverse(r) const -> void {
        rhs = np.zeros(this->n, this->dtype);
        rhs[r] = 1;
        return sp.sparse.linalg.cg(this->L1, rhs[1:], M=this->M, atol=0)[0];

/**
**********
Exceptions
**********

Base exceptions and errors for GraphX.
*/

// __all__ = [
//     "HasACycle",
//     "NodeNotFound",
//     "PowerIterationFailedConvergence",
//     "ExceededMaxIterations",
//     "AmbiguousSolution",
//     "NetworkXAlgorithmError",
//     "NetworkXException",
//     "NetworkXError",
//     "NetworkXNoCycle",
//     "NetworkXNoPath",
//     "NetworkXNotImplemented",
//     "NetworkXPointlessConcept",
//     "NetworkXUnbounded",
//     "NetworkXUnfeasible",
// ];


class NetworkXException : public Exception {
    /** Base class for exceptions in GraphX.*/
};

class NetworkXError : public NetworkXException {
    /** Exception for a serious error in GraphX*/
};

class NetworkXPointlessConcept : public NetworkXException {
    /** Raised when a null graph is provided as input to an algorithm
    that cannot use it.

    The null graph is sometimes considered a pointless concept [1]_,
    thus the name of the exception.

    References
    ----------
    .. [1] Harary, F. and Read, R. "Is the Null Graph a Pointless
       Concept?"  In Graphs and Combinatorics Conference, George
       Washington University.  New York: Springer-Verlag, 1973.

    */


class NetworkXAlgorithmError : public NetworkXException {
    /** Exception for unexpected termination of algorithms.*/
};

class NetworkXUnfeasible : public NetworkXAlgorithmError {
    /** Exception raised by algorithms trying to solve a problem
    instance that has no feasible solution.*/
};

class NetworkXNoPath : public NetworkXUnfeasible {
    /** Exception for algorithms that should return a path when running
    on graphs where such a path does not exist.*/
};

class NetworkXNoCycle : public NetworkXUnfeasible {
    /** Exception for algorithms that should return a cycle when running
    on graphs where such a cycle does not exist.*/
};

class HasACycle : public NetworkXException {
    /** Raised if a graph has a cycle when an algorithm expects that it
    will have no cycles.

    */
};

class NetworkXUnbounded : public NetworkXAlgorithmError {
    /** Exception raised by algorithms trying to solve a maximization
    or a minimization problem instance that is unbounded.*/
};

class NetworkXNotImplemented : public NetworkXException {
    /** Exception raised by algorithms not implemented for a type of graph.*/
};

class NodeNotFound : public NetworkXException {
    /** Exception raised if requested node is not present in the graph*/
};

class AmbiguousSolution : public NetworkXException {
    /** Raised if more than one valid solution exists for an intermediary step
    of an algorithm.

    In the face of ambiguity, refuse the temptation to guess.
    This may occur, for example, when trying to determine the
    bipartite node sets in a disconnected bipartite graph when
    computing bipartite matchings.

    */
};

class ExceededMaxIterations : public NetworkXException {
    /** Raised if a loop iterates too many times without breaking.

    This may occur, for example, in an algorithm that computes
    progressively better approximations to a value but exceeds an
    iteration bound specified by the user.

    */
};

class PowerIterationFailedConvergence : public ExceededMaxIterations {
    /** Raised when the power iteration method fails to converge within a
    specified iteration limit.

    `num_iterations` is the number of iterations that have been
    completed when this exception was raised.

    */

    auto __init__(num_iterations, *args, **kw) const -> void {
        auto msg = f"power iteration failed to converge within {num_iterations} iterations";
        auto exception_message = msg;
        auto superinit = super().__init__;
        superinit(*this, exception_message, *args, **kw);
    }
};
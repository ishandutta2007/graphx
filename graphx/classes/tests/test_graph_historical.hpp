/** Original GraphX graph tests*/
// import graphx
// import graphx as nx

// from .historical_tests import HistoricalTests


class TestGraphHistorical : public HistoricalTests {
    // @classmethod
    auto setup_class(cls) -> void {
        HistoricalTests.setup_class();
        cls.G = nx.Graph

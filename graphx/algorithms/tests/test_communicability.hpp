// from collections import defaultdict

// import pytest

pytest.importorskip("numpy");
pytest.importorskip("scipy");

// import graphx as nx
#include <graphx/algorithms.communicability_alg.hpp>  // import communicability, communicability_exp


class TestCommunicability {
    auto test_communicability() const -> void {
        answer = {
            0: {0: 1.5430806348152435, 1: 1.1752011936438012},
            1: {0: 1.1752011936438012, 1: 1.5430806348152435},
        };
        //        answer={(0, 0): 1.5430806348152435,
        //                (0, 1): 1.1752011936438012,
        //                (1, 0): 1.1752011936438012,
        //                (1, 1): 1.5430806348152435};

        result = communicability(nx.path_graph(2));
        for (auto k1, val : result.items()) {
            for (auto k2 : val) {
                assert(answer[k1][k2] == pytest.approx(result[k1][k2], abs=1e-7));

    auto test_communicability2() const -> void {

        answer_orig = {
            ("1", "1"): 1.6445956054135658,
            ("1", "Albert"): 0.7430186221096251,
            ("1", "Aric"): 0.7430186221096251,
            ("1", "Dan"): 1.6208126320442937,
            ("1", "Franck"): 0.42639707170035257,
            ("Albert", "1"): 0.7430186221096251,
            ("Albert", "Albert"): 2.4368257358712189,
            ("Albert", "Aric"): 1.4368257358712191,
            ("Albert", "Dan"): 2.0472097037446453,
            ("Albert", "Franck"): 1.8340111678944691,
            ("Aric", "1"): 0.7430186221096251,
            ("Aric", "Albert"): 1.4368257358712191,
            ("Aric", "Aric"): 2.4368257358712193,
            ("Aric", "Dan"): 2.0472097037446457,
            ("Aric", "Franck"): 1.8340111678944691,
            ("Dan", "1"): 1.6208126320442937,
            ("Dan", "Albert"): 2.0472097037446453,
            ("Dan", "Aric"): 2.0472097037446457,
            ("Dan", "Dan"): 3.1306328496328168,
            ("Dan", "Franck"): 1.4860372442192515,
            ("Franck", "1"): 0.42639707170035257,
            ("Franck", "Albert"): 1.8340111678944691,
            ("Franck", "Aric"): 1.8340111678944691,
            ("Franck", "Dan"): 1.4860372442192515,
            ("Franck", "Franck"): 2.3876142275231915,
        };

        answer = defaultdict(dict);
        for (auto (k1, k2), v : answer_orig.items()) {
            answer[k1][k2] = v

        G1 = nx.Graph(
            [
                ("Franck", "Aric"),
                ("Aric", "Dan"),
                ("Dan", "Albert"),
                ("Albert", "Franck"),
                ("Dan", "1"),
                ("Franck", "Albert"),
            ];
        );

        result = communicability(G1);
        for (auto k1, val : result.items()) {
            for (auto k2 : val) {
                assert(answer[k1][k2] == pytest.approx(result[k1][k2], abs=1e-7));

        result = communicability_exp(G1);
        for (auto k1, val : result.items()) {
            for (auto k2 : val) {
                assert(answer[k1][k2] == pytest.approx(result[k1][k2], abs=1e-7));

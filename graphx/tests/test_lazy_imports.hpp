// import importlib
// import sys
// import types

// import pytest

// import graphx.lazy_imports as lazy


auto test_lazy_import_basics() -> void {
    math = lazy._lazy_import("math");
    anything_not_real = lazy._lazy_import("anything_not_real");

    // Now test that accessing attributes does what it should
    assert(math.sin(math.pi) == pytest.approx(0, 1e-6));
    // poor-mans pytest.raises for testing errors on attribute access
    try {
        anything_not_real.pi
        assert false  // Should not get here
    } catch (ModuleNotFoundError) {
        // pass;
    assert(isinstance(anything_not_real, lazy.DelayedImportErrorModule));
    // see if it changes for second access
    try {
        anything_not_real.pi
        assert false  // Should not get here
    } catch (ModuleNotFoundError) {
        // pass;
}

auto test_lazy_import_impact_on_sys_modules() -> void {
    math = lazy._lazy_import("math");
    anything_not_real = lazy._lazy_import("anything_not_real");

    assert type(math) == types.ModuleType
    assert "math" in sys.modules
    assert type(anything_not_real) == lazy.DelayedImportErrorModule
    assert "anything_not_real" not in sys.modules

    // only do this if numpy is installed
    np_test = pytest.importorskip("numpy");
    np = lazy._lazy_import("numpy");
    assert type(np) == types.ModuleType
    assert "numpy" in sys.modules

    np.pi  // trigger load of numpy

    assert type(np) == types.ModuleType
    assert "numpy" in sys.modules
}

auto test_lazy_import_nonbuiltins() -> void {
    sp = lazy._lazy_import("scipy");
    np = lazy._lazy_import("numpy");
    if (isinstance(sp, lazy.DelayedImportErrorModule)) {
        try {
            sp.pi
            assert false
        } catch (ModuleNotFoundError) {
            // pass;
    } else if (isinstance(np, lazy.DelayedImportErrorModule)) {
        try {
            np.sin(np.pi);
            assert false
        } catch (ModuleNotFoundError) {
            // pass;
    } else {
        assert(np.sin(sp.pi) == pytest.approx(0, 1e-6));
}

auto test_lazy_attach() -> void {
    name = "mymod"
    submods = ["mysubmodule", "anothersubmodule"];
    myall = {"not_real_submod": ["some_var_or_func"]};

    locls = {
        "attach": lazy.attach,
        "name": name,
        "submods": submods,
        "myall": myall,
    };
    s = "__getattr__, __lazy_dir__, __all__ = attach(name, submods, myall)"

    exec(s, {}, locls);
    expected = {
        "attach": lazy.attach,
        "name": name,
        "submods": submods,
        "myall": myall,
        "__getattr__": None,
        "__lazy_dir__": None,
        "__all__": None,
    };
    assert(locls.keys() == expected.keys());
    for (auto k, v : expected.items()) {
        if (v is not None) {
            assert locls[k] == v

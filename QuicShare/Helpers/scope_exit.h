#pragma once

#include <optional>

template<typename FnT>
class scope_exit {
public:
    scope_exit() = default;
    scope_exit(FnT fn_)
        : fn(fn_)
    { }
    scope_exit(const scope_exit&) = delete;
    scope_exit(scope_exit&&) = default;

    ~scope_exit() {
        try {
            if (fn) {
                (*fn)();
            }
        }
        catch(...) {}
    }

    scope_exit& operator=(const scope_exit&) = delete;
    scope_exit& operator=(scope_exit&&) = default;

    void release() {
        fn = {};
    }

private:
    std::optional<FnT> fn;
};

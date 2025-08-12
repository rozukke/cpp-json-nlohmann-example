#include "nlohmann/detail/macro_scope.hpp"
#include <cstdio>
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

struct OuterParent {
    int parentItem = 1;
    std::optional<int> optItem = 0;
    std::optional<int> optItemNonInit;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OuterParent, parentItem, optItem, optItemNonInit)
};

struct Outer : public OuterParent {
    struct Inner {
        int innerItem = 2;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Inner, innerItem)
    };

    std::vector<Inner> innerVector;

    int outerItem = 3;

    NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE(Outer, OuterParent, innerVector, outerItem)
};

int main() {
    using json = nlohmann::json;

    Outer outer;
    Outer::Inner inner;
    outer.innerVector.push_back(inner);

    json outerJson = outer;
    std::printf("JSON:\n%s\n", outerJson.dump(2).c_str());

    return 0;
}

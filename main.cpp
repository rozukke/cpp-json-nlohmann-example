#define JSON_DIAGNOSTICS 1
#include "nlohmann/detail/macro_scope.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{
template<typename BasicJsonType, typename T>
void from_json(const BasicJsonType& j, std::optional<T>& opt)
{
    if (j.is_null())
    {
        opt = std::nullopt;
    }
    else
    {
        opt.emplace(j.template get<T>());
    }
}
}
NLOHMANN_JSON_NAMESPACE_END

template<typename T>
inline bool has_value(T& value)
{
    return true;
}

template<typename U>
inline bool has_value(const std::optional<U>& opt)
{
    return opt.has_value();
}

// Reasoning: the nlohmann json library is OLD, and BAD. There is no consideration for optionals,
// and if a check for std::optional
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"

#define NLOHMANN_JSON_TO(v1) \
    if (has_value(nlohmann_json_t.v1)) { \
        nlohmann_json_j[#v1] = nlohmann_json_t.v1; \
    }

#pragma GCC diagnostic pop

struct OuterParent {
    int parentItem = 1;
    std::optional<int> optItemNonInit;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(OuterParent, parentItem, optItemNonInit)
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

    const auto* text = R"(
    {
        "innerVector": [
            {
                "innerItem": 5
            }
        ],
        "outerItem": 6,
        "parentItem": 2
    }
    )";

    Outer outer = json::parse(text);

    Outer::Inner inner { .innerItem = 15 };
    outer.innerVector.push_back(inner);

    json outerJson = outer;
    std::printf("JSON:\n%s\n", outerJson.dump(2).c_str());

    return 0;
}

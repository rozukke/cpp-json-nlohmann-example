#define JSON_DIAGNOSTICS 1
#include "nlohmann/detail/macro_scope.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

// This is an override to get over stupid #ifdefs that make it hard to use macros with std::optional.
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

// A type check that does not use <type_traits>, for very good reason - as
// you will see if you try to use them with macros.
namespace {

// Optimised away for all non-std::optional types
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

}

// Reasoning: the nlohmann json library is OLD, and BAD. There is no consideration for optionals,
// and if one is dumped to string, it will appear as `"optional": null` instead of being excluded,
// and there is currently no option to exclude it. This is the messed up manual workaround.
//
// We hook into the macros and template in a check that only happens for std::optional

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"

#define NLOHMANN_JSON_TO(v1) \
    if (has_value(nlohmann_json_t.v1)) { \
        nlohmann_json_j[#v1] = nlohmann_json_t.v1; \
    }

#pragma GCC diagnostic pop




struct OuterParent {
    int parentItem = 1;
    std::optional<int> optItem;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(OuterParent, parentItem, optItem)
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

    // Parent item will be set to default
    const auto* text = R"(
    {
        "innerVector": [
            {
                "innerItem": 5
            }
        ],
        "optItem": 12345,
        "outerItem": 6
    }
    )";

    Outer outer = json::parse(text);

    Outer::Inner inner { .innerItem = 15 };
    outer.innerVector.push_back(inner);

    // Should NOT be output in dump
    outer.optItem = std::nullopt;

    json outerJson = outer;
    std::printf("JSON:\n%s\n", outerJson.dump(2).c_str());

    // ----- Expected output ------
    //
    // JSON:
    // {
    //   "innerVector": [
    //     {
    //       "innerItem": 5
    //     },
    //     {
    //       "innerItem": 15
    //     }
    //   ],
    //   "outerItem": 6,
    //   "parentItem": 1
    // }

    return 0;
}

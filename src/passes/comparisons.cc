#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    // TODO: Rename to boolean
    PassDef comparisons()
    {
        return {
            "comparisons",
            comparisons_wf,
            dir::bottomup | dir::once,
            {
                In(BExpr) * T(AExpr)[Lhs] * T(Equals, LT)[Op] * T(AExpr)[Rhs] >>
                    [](Match& _) -> Node
                    {
                        return BExpr << (_(Op) << _(Lhs) << _(Rhs));
                    },

                In(BExpr) * T(BExpr)[Lhs] * T(And, Or)[Op] * T(BExpr)[Rhs] >>
                    [](Match& _) -> Node
                    {
                        return BExpr << (_(Op) << _(Lhs) << _(Rhs));
                    },

                In(BExpr) * T(Not) * T(BExpr)[BExpr] >>
                    [](Match& _) -> Node
                    {
                        return BExpr << (Not << _(BExpr));
                    },

                T(BExpr) << (Start * T(BExpr)[BExpr] * End) >>
                    [](Match& _) -> Node
                    {
                        return _(BExpr);
                    },

                // Error rules
                T(Equals, LT)[Op] << End >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Op))
                                     << (ErrorMsg ^ "Not enough operands");
                    },
            }};
    }

}

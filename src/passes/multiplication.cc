#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef multiplication()
    {
        return {
            "multiplication",
            multiplication_wf,
            dir::bottomup | dir::once,
            {
                In(AExpr, BExpr) * T(AExpr)[Lhs] * T(Mul) * T(AExpr)[Rhs] >>
                    [](Match& _) -> Node
                    {
                        return AExpr << (Mul << _(Lhs) << _(Rhs));
                    },

                T(AExpr) << (Start * T(AExpr)[AExpr] * End) >>
                    [](Match& _) -> Node
                    {
                        return _(AExpr);
                    },

                // Error rules
                T(Mul)[Mul] << End >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Mul))
                                     << (ErrorMsg ^ "Not enough operands");
                    },
            }};
    }

}

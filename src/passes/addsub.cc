#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef addsub()
    {
        return {
            "addition",
            addition_wf,
            dir::bottomup | dir::once,
            {
                In(AExpr, BExpr) * T(AExpr)[Lhs] * T(Add, Sub)[Op] * T(AExpr)[Rhs] >>
                    [](Match& _) -> Node
                    {
                        return AExpr << (_(Op) << _(Lhs) << _(Rhs));
                    },

                T(AExpr) << (Start * T(AExpr)[AExpr] * End) >>
                    [](Match& _) -> Node
                    {
                        return _(AExpr);
                    },

                // Error rules
                T(Add, Mul)[Op] << End >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Op))
                                     << (ErrorMsg ^ "Not enough operands");
                    },
            }};
    }

}

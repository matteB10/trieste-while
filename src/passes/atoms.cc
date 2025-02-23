#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    // TODO: Add true/false
    // TODO: Add parentheses
    PassDef atoms()
    {
        return {
            "atoms",
            atoms_wf,
            dir::bottomup | dir::once,
            {
                In(AExpr, BExpr) * T(Ident, Int)[Ident] >>
                    [](Match& _) -> Node
                    {
                        return AExpr << _(Ident);
                    },

                T(AExpr) << (Start * T(AExpr)[AExpr] * End) >>
                    [](Match& _) -> Node
                    {
                        return _(AExpr);
                    },

                // Error rules
                --In(AExpr, BExpr, Assign) * T(Ident) >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Ident))
                                     << (ErrorMsg ^ "Unexpected identifier");
                    },

                --In(AExpr, BExpr) * T(Int)[Int] >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Int))
                                     << (ErrorMsg ^ "Unexpected integer");
                    },

            }};
    }

}

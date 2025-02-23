#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef assignments()
    {
        return {
            "assignments",
            assignments_wf,
            dir::topdown | dir::once,
            {
                // TODO: Consider building blindly and checking afterwards
                In(Stmt) *
                  Start * T(Ident)[Ident] * T(Assign) * (Any * Any++)[AExpr] >>
                    [](Match& _) -> Node
                    {
                        return Assign << _(Ident)
                                      << (AExpr << _[AExpr]);
                    },

                // Error rules
                In(Stmt) *
                  (!T(Ident)++)[Ident] * T(Assign) >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _[Ident])
                                     << (ErrorMsg ^ "Invalid left-hand side to assignment");
                    },

                In(Stmt) *
                  T(Assign) * End >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _[Ident])
                                     << (ErrorMsg ^ "Expected right-hand side to assignment");
                    },

                T(Assign)[Assign] << End >>
                [](Match& _)
                {
                    return Error << (ErrorAst << _(Assign))
                                 << (ErrorMsg ^ "Cannot have assignment here");
                }
            }};
    }

}

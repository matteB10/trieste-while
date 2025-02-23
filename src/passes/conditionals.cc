#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef conditionals()
    {
        return {
            "conditionals",
            conditionals_wf,
            dir::topdown | dir::once,
            {
                // TODO: Consider building blindly and checking afterwards
                In(Stmt) *
                  Start * T(If)[If] * T(Then)[Then] * T(Else)[Else] >>
                    [](Match& _) -> Node
                    {
                        return _(If) << *_[Then]
                                     << *_[Else];
                    },

                // Error rules
                T(If)[If] >>
                  [](Match& _) -> Node
                  {
                      return Error << (ErrorAst << _(If))
                                   << (ErrorMsg ^ "Unexpected 'if'");
                  },

                T(Then)[Then] >>
                  [](Match& _) -> Node
                  {
                      return Error << (ErrorAst << _(Then))
                                   << (ErrorMsg ^ "Unexpected 'then'");
                  },

                T(Else)[Else] >>
                  [](Match& _) -> Node
                  {
                      return Error << (ErrorAst << _(Else))
                                   << (ErrorMsg ^ "Unexpected 'else'");
                  },
            }};
    }

}

#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef loops()
    {
        return {
            "loops",
            loops_wf,
            dir::topdown | dir::once,
            {
                // TODO: Consider building blindly and checking afterwards
                In(Stmt) *
                  Start * T(While)[While] * T(Do)[Do] >>
                    [](Match& _) -> Node
                    {
                        return _(While) << *_[Do];
                    },

                // Error rules
                T(While)[While] >>
                  [](Match& _) -> Node
                  {
                      return Error << (ErrorAst << _(While))
                                   << (ErrorMsg ^ "Unexpected 'while'");
                  },

                T(Do)[Do] >>
                  [](Match& _) -> Node
                  {
                      return Error << (ErrorAst << _(Do))
                                   << (ErrorMsg ^ "Unexpected 'do'");
                  },
            }};
    }

}

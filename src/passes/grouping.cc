#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef grouping()
    {
        return {
            "grouping",
            grouping_wf,
            dir::topdown,
            {
                In(File, Semi, Then, Else, Do) *
                  (T(Group)[Group]) >>
                    [](Match &_) -> Node
                    {
                        return Stmt << *_[Group];
                    },

                In(File, Semi, Then, Else, Do) *
                  (Start * T(Paren) << T(Group)[Group] * End) >>
                    [](Match &_) -> Node
                    {
                        return Stmt << *_[Group];
                    },

                In(If, While) *
                  (T(Group)[Group]) >>
                    [](Match &_) -> Node
                    {
                        return BExpr << *_[Group];
                    },

                In(If, While) *
                  (Start * T(Paren)[Paren] * End) >>
                    [](Match &_) -> Node
                    {
                        return Seq << *_[Paren];
                    },

                In(BExpr, Stmt) *
                  Start * (T(Paren) << T(Group)[Group]) * End >>
                    [](Match &_) -> Node
                    {
                        return Seq << *_[Group];
                    },

                In(File, Stmt, Semi, BExpr, Then, Else, Do) *
                  Start * T(Paren)[Paren] * End >>
                  [](Match &_) -> Node
                  {
                      return Seq << *_[Paren];
                  },

                In(Paren) * T(Group)[Group] >>
                  [](Match &_) -> Node
                  {
                      return Seq << *_[Group];
                  },

                In(Paren) * Start * T(Paren)[Paren] * End >>
                  [](Match &_) -> Node
                  {
                      return Seq << *_[Paren];
                  },

                T(Stmt) << T(Semi)[Semi] >>
                  [](Match &_) -> Node
                  {
                      return _(Semi);
                  },

                In(Semi) * T(Semi)[Semi] >>
                  [](Match &_) -> Node
                  {
                      return Seq << *_[Semi];
                  },

                T(Paren) << T(Group)[Group] >>
                  [](Match &_) -> Node
                  {
                      return Expr << *_[Group];
                  },

                // Error rules
                In(Semi) *
                  T(Stmt)[Stmt] << End >>
                  [](Match &_) -> Node
                  {
                      return Error << (ErrorAst << _(Stmt))
                                   << (ErrorMsg ^ "Semicolon does not terminate a statement");
                  },

                In(BExpr, If, While) * T(Semi)[Semi] >>
                  [](Match &_)
                  {
                      return Error << (ErrorAst << _(Semi))
                                   << (ErrorMsg ^ "Cannot have sequence here");
                  },

                T(BExpr, If, While) << End >>
                  [](Match &_)
                  {
                      return Error << (ErrorAst << _(If))
                                   << (ErrorMsg ^ "Empty condition");
                  },

                In(BExpr, If, While) * T(Paren)[Paren] << End >>
                  [](Match &_)
                  {
                      return Error << (ErrorAst << _(Paren))
                                   << (ErrorMsg ^ "Empty condition");
                  },

                T(Stmt, Then, Else, Do)[Then] << End >>
                  [](Match &_)
                  {
                      return Error << (ErrorAst << _(Then))
                                   << (ErrorMsg ^ "Empty Statement");
                  },

                In(File, Semi, Stmt, Then, Else, Do) * T(Paren)[Paren] << End >>
                  [](Match &_)
                  {
                      return Error << (ErrorAst << _(Paren))
                                   << (ErrorMsg ^ "Empty statement");
                  },

                T(Paren)[Paren] << End >> [](Match &_)
                {
                    return Error << (ErrorAst << _(Paren))
                                 << (ErrorMsg ^ "Empty parentheses");
                }
            }
        };
    }

}

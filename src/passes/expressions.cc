#include "../internal.hh"

namespace whilelang
{
    using namespace trieste;

    PassDef expressions()
    {
        return {
            "expressions",
            expressions_wf,
            dir::bottomup | dir::once,
            {
                // All rules Reapply to be able to immediately check for errors
                T(True, False)[Expr] >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (BExpr << _(Expr));
                    },

                T(Ident, Int, Input)[Expr] >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (AExpr << _(Expr));
                    },

                T(Not)[Not] * T(True, False)[Expr] >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (BExpr << (_(Not) << (BExpr << _(Expr))));
                    },

                T(Not)[Not] * T(Paren)[Paren] >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (BExpr << (_(Not) << _(Paren)));
                    },

                T(Group) << (T(AExpr, BExpr)[Expr] * End) >>
                    [](Match &_) -> Node
                    {
                        return Reapply << _(Expr);
                    },

                T(Add, Sub, Mul)[Op] << (T(AExpr, Paren) * T(AExpr, Paren)) >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (AExpr << _(Op));
                    },

                T(Equals, LT)[Op] << (T(AExpr, Paren) * T(AExpr, Paren) * End) >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (BExpr << _(Op));
                    },

                T(And, Or)[Op] << (T(BExpr, Paren) * T(BExpr, Paren)) >>
                    [](Match &_) -> Node
                    {
                        return Reapply << (BExpr << _(Op));
                    },

                T(Paren) << (T(AExpr, BExpr)[Expr] * End) >>
                    [](Match &_) -> Node
                    {
                        return Reapply << _(Expr);
                    },

                // Error rules
                T(Paren)[Paren] << End >>
                    [](Match &_) -> Node
                    {
                        return Error << (ErrorAst << _(Paren))
                                     << (ErrorMsg ^ "Empty parenthesis");
                    },

                In(Group) * ((--Start * T(AExpr, BExpr)[Expr])) >>
                    [](Match &_) -> Node
                    {
                        return Error << (ErrorAst << _(Expr))
                                     << (ErrorMsg ^ "Unexpected expression");
                    },

                In(Group) * T(AExpr, BExpr) * Any[Rhs] >>
                    [](Match &_) -> Node
                    {
                        return Error << (ErrorAst << _(Rhs))
                                     << (ErrorMsg ^ "Not an expression");
                    },

                T(Not)[Not] * End >>
                    [](Match &_) -> Node
                    {
                        return Error << (ErrorAst << _(Not))
                                     << (ErrorMsg ^ "Not enough operands");
                    },

                T(Not)[Not] * (!T(True, False, Paren))[Expr] >>
                    [](Match &_) -> Node
                    {
                        return Error << (ErrorAst << _(Expr))
                                     << (ErrorMsg ^ "Invalid operand");
                    },

                T(Add, Sub, Mul, Equals, LT)[Op] << (T(AExpr, Paren) * End) >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Op))
                                     << (ErrorMsg ^ "Not enough operands");
                    },

                In(Add, Sub, Mul, Equals, LT) * (T(Group) << End)[Expr] >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Expr))
                                     << (ErrorMsg ^ "Expected operand");
                    },

                In(Add, Sub, Mul, Equals, LT) * (!T(AExpr, Paren))[Expr] >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Expr))
                                     << (ErrorMsg ^ "Invalid operand");
                    },

                T(And, Or)[Op] << (T(BExpr, Paren) * End) >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Op))
                                     << (ErrorMsg ^ "Not enough operands");
                    },

                In(And, Or) * (T(Group) << End)[Expr] >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Expr))
                                     << (ErrorMsg ^ "Expected operand");
                    },

                In(And, Or) * (!T(BExpr, Paren))[Expr] >>
                    [](Match& _) -> Node
                    {
                        return Error << (ErrorAst << _(Expr))
                                     << (ErrorMsg ^ "Invalid operand");
                    },
            }};
    }

}

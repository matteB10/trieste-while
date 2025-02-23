#pragma once
#include "lang.hh"

namespace whilelang
{
  using namespace trieste;

  Parse parser();
  PassDef grouping();
  PassDef assignments();
  PassDef conditionals();
  PassDef loops();
  PassDef atoms();
  PassDef multiplication();
  PassDef addsub();

  inline const auto parse_token =
    Assign | Skip |
    If | Then | Else |
    While | Do |
    Int | Add | Sub | Mul |
    True | False | And | Or | Equals | LT |
    Ident;

  inline const auto grouping_construct =
    Group | Semi | Paren;

  inline const wf::Wellformed parse_wf =
    (Top <<= File)
    | (File  <<= ~grouping_construct)
    | (Semi  <<= Group++[1])
    | (If    <<= grouping_construct)
    | (Then  <<= grouping_construct)
    | (Else  <<= grouping_construct)
    | (While <<= grouping_construct)
    | (Do    <<= grouping_construct)
    | (Paren <<= ~grouping_construct)
    | (Group <<= (parse_token | Paren)++[1])
    ;

  // TODO: Redesign parser to keep Group around for longer
  // Group means "Things that we have not yet processed"
  inline const wf::Wellformed grouping_wf =
    (parse_wf - Group)
    | (File <<= ~(Stmt | Semi))
    | (Semi <<= Stmt++[1])
    | (If <<= BExpr)
    | (Then <<= Stmt | Semi)
    | (Else <<= Stmt | Semi)
    | (While <<= BExpr)
    | (Do <<= Stmt | Semi)
    //| (Paren <<= (parse_token | Paren | Semi)++[1])
    | (Stmt <<= (parse_token | Expr)++[1])
    | (Expr <<=  (parse_token | Expr)++[1])
    ;

  inline const auto assignments_parse_token = parse_token - Assign;

  inline const wf::Wellformed assignments_wf =
    grouping_wf
    | (Assign <<= Ident * AExpr)
    | (Paren <<= (assignments_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (assignments_parse_token | Assign | Paren)++[1])
    | (AExpr <<= (assignments_parse_token | Paren)++[1])
    | (BExpr <<= (assignments_parse_token | Paren)++[1])
    ;

  inline const auto conditionals_parse_token = assignments_parse_token - Then - Else;

  inline const wf::Wellformed conditionals_wf =
    (assignments_wf - Then - Else)
    | (If <<= BExpr * (Then >>= Stmt | Semi) * (Else >>= Stmt | Semi))
    | (Paren <<= (conditionals_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (conditionals_parse_token | Assign | If | Paren)++[1])
    | (AExpr <<= (conditionals_parse_token | Paren)++[1])
    | (BExpr <<= (conditionals_parse_token | Paren)++[1])
    ;

  inline const auto loops_parse_token = conditionals_parse_token - Do;

  inline const wf::Wellformed loops_wf =
    (conditionals_wf - Do)
    | (While <<= BExpr * (Do >>= Stmt | Semi))
    | (Paren <<= (loops_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (loops_parse_token | Assign | If | While | Paren)++[1])
    | (AExpr <<= (loops_parse_token | Paren)++[1])
    | (BExpr <<= (loops_parse_token | Paren)++[1])
    ;

  inline const auto atoms_parse_token = loops_parse_token - Ident - Int;

  inline const wf::Wellformed atoms_wf =
    loops_wf
    | (Paren <<= (atoms_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (atoms_parse_token | Assign | If | While | Paren)++[1])
    | (AExpr <<= (atoms_parse_token | AExpr | Int | Ident | Paren)++[1])
    | (BExpr <<= (atoms_parse_token | AExpr | Paren)++[1])
    ;

  inline const auto multiplication_parse_token = loops_parse_token - Mul;

  inline const wf::Wellformed multiplication_wf =
    atoms_wf
    | (Mul <<= AExpr * AExpr)
    | (Paren <<= (atoms_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (atoms_parse_token | Assign | If | While | Paren)++[1])
    | (AExpr <<= (atoms_parse_token | AExpr | Mul | Int | Ident | Paren)++[1])
    | (BExpr <<= (atoms_parse_token | AExpr | Paren)++[1])
    ;

  inline const auto addition_parse_token = multiplication_parse_token - Add - Sub;

  inline const wf::Wellformed addition_wf =
    multiplication_wf
    | (Add <<= AExpr * AExpr)
    | (Sub <<= AExpr * AExpr)
    | (Paren <<= (atoms_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (atoms_parse_token | Assign | If | While | Paren)++[1])
    | (AExpr <<= (Add | Sub | Mul | Int | Ident | Paren)++[1])
    | (BExpr <<= (atoms_parse_token | AExpr | Paren)++[1])
    ;

  inline const auto comparisons_parse_token = addition_parse_token - LT - Equals - And - Or - Not;

  inline const wf::Wellformed comparisons_wf =
    addition_wf
    | (LT <<= AExpr * AExpr)
    | (Equals <<= AExpr * AExpr)
    | (And <<= BExpr * BExpr)
    | (Or <<= BExpr * BExpr)
    | (Not <<= BExpr)
    | (Paren <<= (comparisons_parse_token | Paren | Semi)++[1])
    | (Stmt <<=  (comparisons_parse_token | Assign | If | While | Paren)++[1])
    | (BExpr <<= (Equals | LT | And | Or | Not | Paren)++[1])
    ;

}

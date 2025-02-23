#include "lang.hh"
#include "internal.hh"

namespace whilelang
{
  using namespace trieste;
  using namespace trieste::detail;

  Parse parser()
  {
    Parse p(depth::file, parse_wf);

    auto term_all = [](Make &m, std::initializer_list<Token> tokens) {
      bool progress = true;
      while (progress) {
        progress = false;
        for (auto &t : tokens) {
          if (m.in(t) || m.group_in(t)) {
            m.term({t});
            progress = true;
            break;
          }
        }
      }
    };

    p("start",
    {
        // whitespace
        "[[:space:]]+" >> [](auto&) {}, // no-op

        // Line comment.
        "//[^\n]*" >> [](auto&) {}, // no-op

        // Statements
        ":=" >> [](auto& m) {m.add(Assign); },

        "skip" >> [](auto& m) {m.add(Skip); },

        ";" >> [term_all](auto& m) {term_all(m, {Else, Do}); m.seq(Semi); },

        "if\\b" >> [](auto& m) {m.push(If); },
        "then\\b" >> [](auto& m) {m.term(); m.pop({If}); m.push(Then); },
        "else\\b" >> [term_all](auto& m) {term_all(m, {Semi, Else, Do}); m.term(); m.pop(Then); m.push(Else); },

        "while\\b" >> [](auto& m) {m.push(While); },
        "do\\b" >> [](auto& m) {m.term(); m.pop({While}); m.push(Do); },

        // Expressions
        R"(\+)" >> [](auto& m) {m.add(Add); },
        "-" >> [](auto& m) {m.add(Sub); },
        R"(\*)" >> [](auto& m) {m.add(Mul); },

        "and\\b" >> [](auto& m) {m.add(And); },
        "or\\b" >> [](auto& m) {m.add(Or); },
        "not\\b" >> [](auto& m) {m.add(Not); },

        "<" >> [](auto& m) {m.add(LT); },
        "=" >> [](auto& m) {m.add(Equals); },

        "true\\b" >> [](auto& m) {m.add(True); },
        "false\\b" >> [](auto& m) {m.add(False); },

        "[[:digit:]]+" >> [](auto& m) { m.add(Int); },

        // Variables
        R"([_[:alpha:]][_[:alnum:]]*)" >> [](auto& m) { m.add(Ident); },

        // Grouping
        "\\(" >> [](auto& m) { m.push(Paren); },
        "\\)" >> [term_all](auto& m) { term_all(m, {Else, Do, Semi}); m.term(); m.pop(Paren); },
    }
    );

    p.done([term_all](auto& m) {
        term_all(m, {Else, Do});
        m.term({Semi});
    });
    return p;
  }
}

#include "internal.hh"
// #include "passes/internal.hh"


namespace whilelang {

using namespace trieste;

Reader reader()
  {
    return {
      "while",
      {
        expressions(),
        statements(),
//       atoms(),
//       multiplication(),
//       addsub(),
//       comparison(),
//       conjunction(),
//       disjunction(),
//       assignments(),
//       loops(),
//       conditionals(),
//       expressions(),
      },
      whilelang::parser(),
    };
  }

}

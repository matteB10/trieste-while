#include "internal.hh"
// #include "passes/internal.hh"


namespace whilelang {

using namespace trieste;

Reader reader()
  {
    return {
      "while",
      {grouping(),
       assignments(),
       conditionals(),
       loops(),
       atoms(),
       multiplication(),
       addsub(),
      },
      whilelang::parser(),
    };
  }

}

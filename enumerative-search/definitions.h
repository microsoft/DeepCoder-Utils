// Definitions and typedefs

#ifndef _DEFINITIONS__
#define _DEFINITIONS__

#include <string>
#include <cfloat>

using namespace std;

// keep these updated together
typedef double real_t;
#define MAX_REAL DBL_MAX
#define REAL_TOLERANCE DBL_EPSILON


#define BIG_DOUBLE 1000000000.0
#define FORCE_ASSIGNMENT BIG_DOUBLE

// Purely for convenience, mostly for use with stl data structures that
// use unsigned ints for size_t.
typedef unsigned int uint;


class Datum;
typedef void (*DatumOp0)(Datum *result_space);
typedef void (*DatumOp1)(Datum *arg1, Datum *result_space);
typedef void (*DatumOp2)(Datum *arg1, Datum *arg2, Datum *result_space);
typedef void (*DatumOp3)(Datum *arg1, Datum *arg2, Datum *arg3, Datum *result_space);

enum DatumType { Int, Array };

typedef bool (*IntBoolLambda)(int);
typedef int (*IntIntLambda)(int);
typedef bool (*IntIntBoolLambda)(int, int);
typedef int (*IntIntIntLambda)(int, int);


#endif  // _DEFINITIONS__

#ifndef _OPS__
#define _OPS__

/*
 *
 * ===================
 * Top level functions
 * ===================
 * Mark X if completed, _ if missing
 *
 * X HEAD
 * X LAST
 * X TAKE
 * X DROP
 * _ ACCESS
 * X MINIMUM
 * X MAXIMUM
 * X REVERSE
 * X SORT
 * X SUM
 * X MAP
 * X FILTER
 * X COUNT
 * X ZIPWITH
 * X SCANL
 */


#include "datum.h"

// helpers
void map_int_int(Datum *arg1, IntIntLambda f, Datum *result_space);
void zipwith(Datum *arg1, Datum *arg2, IntIntIntLambda f, Datum *result_space);
void filter(Datum *arg1, IntBoolLambda f, Datum *result_space);
void count(Datum *arg1, IntBoolLambda f, Datum *result_space);
void scanl(Datum *arg1, IntIntIntLambda f, Datum *result_space);

// int->int lambdas
int increment_lambda(int x);
int decrement_lambda(int x);
int mult2_lambda(int x);
int div2_lambda(int x);
int negate_lambda(int x);
int sqr_lambda(int x);
int mult3_lambda(int x);
int div3_lambda(int x);
int mult4_lambda(int x);
int div4_lambda(int x);


// int->bool lambdas
bool is_pos_lambda(int x);
bool is_neg_lambda(int x);
bool is_odd_lambda(int x);
bool is_even_lambda(int x);


// int->int->int lambdas
int add_lambda(int x, int y);
int subtract_lambda(int x, int y);
int mult_lambda(int x, int y);
int min_lambda(int x, int y);
int max_lambda(int x, int y);


////
//
// 1-argument int->int ops
//
////
void increment(Datum *arg1, Datum *result_space);
void decrement(Datum *arg1, Datum *result_space);


///
//
// 1-argument int[]->int ops
//
////

void arr_max(Datum *arg1, Datum *result_space);
void arr_min(Datum *arg1, Datum *result_space);
void arr_head(Datum *arg1, Datum *result_space);
void arr_last(Datum *arg1, Datum *result_space);
void arr_sum(Datum *arg1, Datum *result_space);

// COUNT
void count_is_pos(Datum *arg1, Datum *result_space);
void count_is_neg(Datum *arg1, Datum *result_space);
void count_is_odd(Datum *arg1, Datum *result_space);
void count_is_even(Datum *arg1, Datum *result_space);


////
//
// 1 argument int[]->int[] ops
//
////

// MAP
void map_increment(Datum *arg1, Datum *result_space);
void map_decrement(Datum *arg1, Datum *result_space);
void map_mult2(Datum *arg1, Datum *result_space);
void map_div2(Datum *arg1, Datum *result_space);
void map_negate(Datum *arg1, Datum *result_space);
void map_sqr(Datum *arg1, Datum *result_space);
void map_mult3(Datum *arg1, Datum *result_space);
void map_div3(Datum *arg1, Datum *result_space);
void map_mult4(Datum *arg1, Datum *result_space);
void map_div4(Datum *arg1, Datum *result_space);

// FILTER
void filter_is_pos(Datum *arg1, Datum *result_space);
void filter_is_neg(Datum *arg1, Datum *result_space);
void filter_is_even(Datum *arg1, Datum *result_space);
void filter_is_odd(Datum *arg1, Datum *result_space);

// SORT & REVERSE
void sort_datum(Datum *arg1, Datum *result_space);
void reverse_datum(Datum *arg1, Datum *result_space);

void scanl_add(Datum *arg1, Datum *result_space);
void scanl_subtract(Datum *arg1, Datum *result_space);
void scanl_mult(Datum *arg1, Datum *result_space);
void scanl_max(Datum *arg1, Datum *result_space);
void scanl_min(Datum *arg1, Datum *result_space);


////
//
// 2 argument int->int[]->int ops
//
////

void access(Datum *arg1, Datum *arg2, Datum *result_space);

////
//
// 2 argument int->int[]->int[] ops
//
////

void take(Datum *arg1, Datum *arg2, Datum *result_space);
void drop(Datum *arg1, Datum *arg2, Datum *result_space);

////
//
// 2 argument int[]->int[]->int[] ops
//
////

void zipwith_add(Datum *arg1, Datum *arg2, Datum *result_space);
void zipwith_subtract(Datum *arg1, Datum *arg2, Datum *result_space);
void zipwith_mult(Datum *arg1, Datum *arg2, Datum *result_space);
void zipwith_max(Datum *arg1, Datum *arg2, Datum *result_space);
void zipwith_min(Datum *arg1, Datum *arg2, Datum *result_space);

#endif

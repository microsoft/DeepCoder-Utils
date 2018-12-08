#include "ops.h"
#include "utils.h"
#include <algorithm>
#include <climits>

////
//
// helper high order functions
//
////

void map_int_int(Datum *arg1, IntIntLambda f, Datum *result_space) {
  result_space->SetType(Array);
  result_space->Resize(arg1->Size());

  for (int i = 0; i < arg1->Size(); i++) {
    int src_val = arg1->GetArrayElementValue(i);
    result_space->SetArrayElementValue(i, f(src_val));
  }
}


void zipwith(Datum *arg1, Datum *arg2, IntIntIntLambda f, Datum *result_space) {
  result_space->SetType(Array);
  int result_size = min(arg1->Size(), arg2->Size());
  result_space->Resize(result_size);

  for (int i = 0; i < result_size; i++) {
    int src1_val = arg1->GetArrayElementValue(i);
    int src2_val = arg2->GetArrayElementValue(i);
    result_space->SetArrayElementValue(i, f(src1_val, src2_val));
  }
}


void filter(Datum *arg1, IntBoolLambda f, Datum *result_space) {
  result_space->SetType(Array);
  vector<int> *result_values = result_space->GetValues();
  result_values->clear();

  for (int i = 0; i < arg1->Size(); i++) {
    int src1_val = arg1->GetArrayElementValue(i);
    if (f(src1_val)) {
      result_values->push_back(src1_val);
    }
  }
}


void count(Datum *arg1, IntBoolLambda f, Datum *result_space) {
  result_space->SetType(Int);

  int count = 0;
  for (int i = 0; i < arg1->Size(); i++) {
    int src1_val = arg1->GetArrayElementValue(i);
    if (f(src1_val))  count++;
  }
  result_space->SetIntValue(count);
}


void scanl(Datum *arg1, IntIntIntLambda f, Datum *result_space) {
  result_space->SetType(Array);
  vector<int> *result_values = result_space->GetValues();
  result_values->clear();

  if (arg1->Size() > 0) {
    int running_val = arg1->GetArrayElementValue(0);
    result_values->push_back(running_val);

    for (int i = 1; i < arg1->Size(); i++) {
      running_val = f(running_val, arg1->GetArrayElementValue(i));
      result_values->push_back(running_val);
    }
  }
}

////
//
// lambdas
//
////

// int->int lambdas
int increment_lambda(int x) { return x + 1; }
int decrement_lambda(int x) { return x - 1; }
int mult2_lambda(int x) { return x * 2; }
int div2_lambda(int x) { return x / 2; }
int negate_lambda(int x) { return -x; }
int sqr_lambda(int x) { return x * x; }
int mult3_lambda(int x) { return x * 3; }
int div3_lambda(int x) { return x / 3; }
int mult4_lambda(int x) { return x * 4; }
int div4_lambda(int x) { return x / 4; }

// int->bool lambdas
bool is_pos_lambda(int x) { return x > 0; }
bool is_neg_lambda(int x) { return x < 0; }
bool is_odd_lambda(int x) { return x % 2 == 1 || x % 2 == -1; }
bool is_even_lambda(int x) { return x % 2 == 0; }

// int->int->int lambdas
int add_lambda(int x, int y) { return x + y; }
int subtract_lambda(int x, int y) { return x - y; }
int mult_lambda(int x, int y) { return x * y; }
int min_lambda(int x, int y) { return min(x, y); }
int max_lambda(int x, int y) { return max(x, y); }


////
//
// 1-argument int->int ops
//
////

void increment(Datum *arg1, Datum *result_space) {
  CHECK_EQ(arg1->Type(), Int, "Type mismatch");
  result_space->SetIntValue(arg1->GetIntValue() + 1);
}

void decrement(Datum *arg1, Datum *result_space) {
  CHECK_EQ(arg1->Type(), Int, "Type mismatch");
  result_space->SetIntValue(arg1->GetIntValue() - 1);
}


///
//
// 1-argument int[]->int ops
//
////

// Head, last, max, min
void arr_max(Datum *arg1, Datum *result_space) {
  result_space->SetType(Int);
  int max_val = INT_MIN;
  for (int i = 0; i < arg1->Size(); i++) {
    int cur_val = arg1->GetArrayElementValue(i);
    max_val = (cur_val > max_val) ? cur_val : max_val;
  }
  result_space->SetIntValue(max_val);
}

void arr_min(Datum *arg1, Datum *result_space) {
  result_space->SetType(Int);
  int min_val = INT_MAX;
  for (int i = 0; i < arg1->Size(); i++) {
    int cur_val = arg1->GetArrayElementValue(i);
    min_val = (cur_val < min_val) ? cur_val : min_val;
  }
  result_space->SetIntValue(min_val);
}

void arr_head(Datum *arg1, Datum *result_space) {
  result_space->SetType(Int);
  if (arg1->Size() == 0) {
    result_space->SetIntValue(0);
  } else {
    result_space->SetIntValue(arg1->GetArrayElementValue(0));
  }
}

void arr_last(Datum *arg1, Datum *result_space) {
  result_space->SetType(Int);
  if (arg1->Size() == 0) {
    result_space->SetIntValue(0);
  } else {
    int last_idx = arg1->Size() - 1;
    result_space->SetIntValue(arg1->GetArrayElementValue(last_idx));
  }
}

void arr_sum(Datum *arg1, Datum *result_space) {
  result_space->SetType(Int);
  int total = 0;
  for (int i = 0; i < arg1->Size(); i++) {
    total += arg1->GetArrayElementValue(i);
  }
  result_space->SetIntValue(total);
}

// COUNT

void count_is_pos(Datum *arg1, Datum *result_space) {
  count(arg1, is_pos_lambda, result_space);
}

void count_is_neg(Datum *arg1, Datum *result_space) {
  count(arg1, is_neg_lambda, result_space);
}

void count_is_odd(Datum *arg1, Datum *result_space) {
  count(arg1, is_odd_lambda, result_space);
}

void count_is_even(Datum *arg1, Datum *result_space) {
  count(arg1, is_even_lambda, result_space);
}

////
//
// 1-argument int[]->int[] ops
//
////

// MAP

void map_increment(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, increment_lambda, result_space);
}

void map_decrement(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, decrement_lambda, result_space);
}

void map_mult2(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, mult2_lambda, result_space);
}

void map_div2(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, div2_lambda, result_space);
}

void map_negate(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, negate_lambda, result_space);
}

void map_sqr(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, sqr_lambda, result_space);
}

void map_mult3(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, mult3_lambda, result_space);
}

void map_div3(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, div3_lambda, result_space);
}

void map_mult4(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, mult4_lambda, result_space);
}

void map_div4(Datum *arg1, Datum *result_space) {
  map_int_int(arg1, div4_lambda, result_space);
}

void filter_is_pos(Datum *arg1, Datum *result_space) {
  filter(arg1, is_pos_lambda, result_space);
}

void filter_is_neg(Datum *arg1, Datum *result_space) {
  filter(arg1, is_neg_lambda, result_space);
}

void filter_is_odd(Datum *arg1, Datum *result_space) {
  filter(arg1, is_odd_lambda, result_space);
}

void filter_is_even(Datum *arg1, Datum *result_space) {
  filter(arg1, is_even_lambda, result_space);
}

// SORT

void sort_datum(Datum *arg1, Datum *result_space) {
  CHECK_EQ(arg1->Type(), Array, "Type mismatch");
  result_space->SetArrayValue(*arg1->GetValues());

  vector<int> *values = result_space->GetValues();
  sort(values->begin(), values->end());
}

void reverse_datum(Datum *arg1, Datum *result_space) {
  CHECK_EQ(arg1->Type(), Array, "Type mismatch");
  result_space->SetArrayValue(*arg1->GetValues());

  vector<int> *values = result_space->GetValues();
  reverse(values->begin(), values->end());
}

// SCANL

void scanl_add(Datum *arg1, Datum *result_space) {
  scanl(arg1, add_lambda, result_space);
}

void scanl_subtract(Datum *arg1, Datum *result_space) {
  scanl(arg1, subtract_lambda, result_space);
}

void scanl_mult(Datum *arg1, Datum *result_space) {
  scanl(arg1, mult_lambda, result_space);
}

void scanl_max(Datum *arg1, Datum *result_space) {
  scanl(arg1, max_lambda, result_space);
}

void scanl_min(Datum *arg1, Datum *result_space) {
  scanl(arg1, min_lambda, result_space);
}


////
//
// 2-argument int->int[]->int ops
//
////

void access(Datum *arg1, Datum *arg2, Datum *result_space) {
  result_space->SetType(Int);
  int offset = arg1->GetIntValue();

  if (offset >= 0 && offset < arg2->Size()) {
    result_space->SetIntValue(arg2->GetArrayElementValue(offset));
  } else {
    result_space->SetIntValue(-1000000);  // TODO: what should happen here?
  }
}


////
//
// 2-argument int->int[]->int[] ops
//
////

void take(Datum *arg1, Datum *arg2, Datum *result_space) {
  result_space->SetType(Array);
  int result_size = min(arg1->GetIntValue(), arg2->Size());
  vector<int> *result_values = result_space->GetValues();
  result_values->clear();

  for (int i = 0; i < result_size; i++) {
    int src_val = arg2->GetArrayElementValue(i);
    result_values->push_back(src_val);
  }
}

void drop(Datum *arg1, Datum *arg2, Datum *result_space) {
  result_space->SetType(Array);
  int offset = arg1->GetIntValue();
  vector<int> *result_values = result_space->GetValues();
  result_values->clear();

  if (offset >= 0) {
    for (int i = offset; i < arg2->Size(); i++) {
      int src_val = arg2->GetArrayElementValue(i);
      result_values->push_back(src_val);
    }
  }
}


////
//
// 2-argument int[]->int[]->int[] ops
//
////

void zipwith_add(Datum *arg1, Datum *arg2, Datum *result_space) {
  zipwith(arg1, arg2, add_lambda, result_space);
}

void zipwith_subtract(Datum *arg1, Datum *arg2, Datum *result_space) {
  zipwith(arg1, arg2, subtract_lambda, result_space);
}

void zipwith_mult(Datum *arg1, Datum *arg2, Datum *result_space) {
  zipwith(arg1, arg2, mult_lambda, result_space);
}

void zipwith_max(Datum *arg1, Datum *arg2, Datum *result_space) {
  zipwith(arg1, arg2, max_lambda, result_space);
}

void zipwith_min(Datum *arg1, Datum *arg2, Datum *result_space) {
  zipwith(arg1, arg2, min_lambda, result_space);
}


#include "successor.h"
#include "io_set.h"
#include "program_state.h"
#include "ops.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>


bool SuccPairCompare(pair<double, Successor> a, pair<double, Successor> b) {
  return (a.first > b.first);
}


ostream &operator<<(ostream &os, const Successor &s) {
  if (s.IsOp1()) {
    os << "op1_" << s.op1_ << "(" << s.op1_arg_ << ")";
  } else if (s.IsOp2()) {
    os << "op2_" << s.op2_ << "(" << s.op2_arg1_ << "," << s.op2_arg2_ << ")";
  }
  return os;
}


SuccessorIterator::SuccessorIterator() {
  InitOps();
  InitSuccessors();
}


SuccessorIterator::SuccessorIterator(IOSet *io) {
  InitOps();
  InitSuccessors();
  Init(io);
  /*
  for (auto &succ: successors_) {
    cout << succ.Name() << endl;
  }
  */
}


SuccessorIterator::SuccessorIterator(string order_filename, IOSet *io, int sort_and_add_cutoff) {
  InitOps();
  InitSuccessors(order_filename, sort_and_add_cutoff);
  Init(io);
  /*
  for (auto &succ: successors_) {
    cout << succ.Name() << endl;
  }
  */
}


SuccessorIterator::~SuccessorIterator() {}


/*
 *
 * Init captures the sequence of types that are in existence in io
 * at the time of creation of the iterator. If io changes types
 * from one call to the next of Next(), then behavior is undefined.
 *
 */
void SuccessorIterator::Init(IOSet *io) {
  auto ps = io->GetInputs(0);
  types_.clear();
  for (int i = 0; i < ps->NumUsed(); i++) {
    types_.push_back(ps->GetDatumType(i));
  }
  counter_ = 0;

  if (cumulative_counts_.size() == 0) {
    cumulative_counts_.resize(successors_.size(), -1);
  } else {
    fill(cumulative_counts_.begin(), cumulative_counts_.end(), -1);
  }
}


/*
 * Strategy: first iterate over all ops, then iterate over
 * all compatible inputs.
 */
const Successor *SuccessorIterator::Cur() {
  int counter = 0;
  // iterate over all possible successors
  int successor_size = successors_.size();
  for (int i = 0; i < successor_size; i++) {
    if (cumulative_counts_[i] != -1 && counter_ > cumulative_counts_[i]) {
      counter = cumulative_counts_[i];
      continue;
    }

    Successor &succ = successors_[i];
    if (succ.IsOp1()) {
      // iterate over single arguments of the right type
      for (int j = 0; j < types_.size(); j++) {
        if (types_[j] != succ.GetOp1ArgType())  continue;

        if (counter >= counter_) {
          succ.SetOp1Arg(j);
          return &succ;
        } else {
          counter++;
        }
      }

    } else if (succ.IsOp2()) {
      // iterate over pairs of arguments of the right type
      for (int j1 = 0; j1 < types_.size(); j1++) {
        for (int j2 = 0; j2 < types_.size(); j2++) {
          if (types_[j1] != succ.GetOp2Arg1Type() ||
              types_[j2] != succ.GetOp2Arg2Type()) {
            continue;
          }

          if (counter >= counter_) {
            succ.SetOp2Args(j1, j2);
            return &succ;
          } else {
            counter++;
          }
        }
      }
    }
    cumulative_counts_[i] = counter;
  }
  return NULL;
}

const Successor *SuccessorIterator::Next() {
  auto result = Cur();
  counter_++;
  return result;
}


void SuccessorIterator::InitOps() {
  // int to int ops
  if (false) {  // Matej doesn't use these
    int_to_int_ops_.push_back(make_pair("increment", increment));
    int_to_int_ops_.push_back(make_pair("decrement", decrement));
  }

  // int[] to int ops
  if (true) {  // element selection
    arr_to_int_ops_.push_back(make_pair("arr_min", arr_min));
    arr_to_int_ops_.push_back(make_pair("arr_max", arr_max));
    arr_to_int_ops_.push_back(make_pair("arr_head", arr_head));
    arr_to_int_ops_.push_back(make_pair("arr_last", arr_last));
    arr_to_int_ops_.push_back(make_pair("arr_sum", arr_sum));
  }

  if (true) {  // counting
    arr_to_int_ops_.push_back(make_pair("count_is_pos", count_is_pos));
    arr_to_int_ops_.push_back(make_pair("count_is_neg", count_is_neg));
    arr_to_int_ops_.push_back(make_pair("count_is_even", count_is_even));
    arr_to_int_ops_.push_back(make_pair("count_is_odd", count_is_odd));
  }

  // int[] to int[] ops
  if (true) {  // maps
    arr_to_arr_ops_.push_back(make_pair("map_increment", map_increment));
    arr_to_arr_ops_.push_back(make_pair("map_decrement", map_decrement));
    arr_to_arr_ops_.push_back(make_pair("map_mult2", map_mult2));
    arr_to_arr_ops_.push_back(make_pair("map_div2", map_div2));
    arr_to_arr_ops_.push_back(make_pair("map_negate", map_negate));
    arr_to_arr_ops_.push_back(make_pair("map_sqr", map_sqr));
    arr_to_arr_ops_.push_back(make_pair("map_mult3", map_mult3));
    arr_to_arr_ops_.push_back(make_pair("map_div3", map_div3));
    arr_to_arr_ops_.push_back(make_pair("map_mult4", map_mult4));
    arr_to_arr_ops_.push_back(make_pair("map_div4", map_div4));
  }

  if (true) {  // filters
    arr_to_arr_ops_.push_back(make_pair("filter_is_pos", filter_is_pos));
    arr_to_arr_ops_.push_back(make_pair("filter_is_neg", filter_is_neg));
    arr_to_arr_ops_.push_back(make_pair("filter_is_odd", filter_is_odd));
    arr_to_arr_ops_.push_back(make_pair("filter_is_even", filter_is_even));
  }

  if (true) {  // sort and reverse
    arr_to_arr_ops_.push_back(make_pair("sort", sort_datum));
    arr_to_arr_ops_.push_back(make_pair("reverse", reverse_datum));
  }

  if (true) {  // scanl
    arr_to_arr_ops_.push_back(make_pair("scanl_add", scanl_add));
    arr_to_arr_ops_.push_back(make_pair("scanl_subtract", scanl_subtract));
    arr_to_arr_ops_.push_back(make_pair("scanl_mult", scanl_mult));
    arr_to_arr_ops_.push_back(make_pair("scanl_max", scanl_max));
    arr_to_arr_ops_.push_back(make_pair("scanl_min", scanl_min));
  }

  if (true) {
    int_arr_to_int_ops_.push_back(make_pair("access", access));
  }
  // int[] x int[] to int[] ops
  if (true) {
    int_arr_to_arr_ops_.push_back(make_pair("take", take));
    int_arr_to_arr_ops_.push_back(make_pair("drop", drop));
  }

  // int[] x int[] to int[] ops
  if (true) {
    arr_arr_to_arr_ops_.push_back(make_pair("zipwith_add", zipwith_add));
    arr_arr_to_arr_ops_.push_back(make_pair("zipwith_subtract", zipwith_subtract));
    arr_arr_to_arr_ops_.push_back(make_pair("zipwith_mult", zipwith_mult));
    arr_arr_to_arr_ops_.push_back(make_pair("zipwith_max", zipwith_max));
    arr_arr_to_arr_ops_.push_back(make_pair("zipwith_min", zipwith_min));
  }
}


void SuccessorIterator::InitSuccessors() {
  // Lots of copy construction here is ok, since Successor
  // is essentially a light-weight struct.
  for (auto &op : int_to_int_ops_) {
    Successor next_succ(op.first, op.second, Int);
    successors_.push_back(next_succ);
  }
  for (auto &op : arr_to_int_ops_) {
    Successor next_succ(op.first, op.second, Array);
    successors_.push_back(next_succ);
  }
  for (auto &op : arr_to_arr_ops_) {
    Successor next_succ(op.first, op.second, Array);
    successors_.push_back(next_succ);
  }
  for (auto &op : int_arr_to_int_ops_) {
    Successor next_succ(op.first, op.second, Int, Array);
    successors_.push_back(next_succ);
  }
  for (auto &op : int_arr_to_arr_ops_) {
    Successor next_succ(op.first, op.second, Int, Array);
    successors_.push_back(next_succ);
  }
  for (auto &op : arr_arr_to_arr_ops_) {
    Successor next_succ(op.first, op.second, Array, Array);
    successors_.push_back(next_succ);
  }


}


void SuccessorIterator::InitSuccessors(string order_filename, int sort_and_add_cutoff) {
  InitSuccessors();

  // TODO: move this somewhere more sensible
  string ZIPWITH = "ZIPWITH";
  string TIMES = "*";
  string MAP = "MAP";
  string SQR = "SQR";
  string MUL4 = "MUL4";
  string DIV4 = "DIV4";
  string MINUS = "-";
  string MUL3 = "MUL3";
  string DIV3 = "DIV3";
  string MIN = "MIN";
  string PLUS = "+";
  string SCANL = "SCANL";
  string SHR = "SHR";
  string SHL = "SHL";
  string MAX = "MAX";
  string HEAD = "HEAD";
  string DEC = "DEC";
  string SUM = "SUM";
  string doNEG = "doNEG";
  string isNEG = "isNEG";
  string INC = "INC";
  string LAST = "LAST";
  string MINIMUM = "MINIMUM";
  string isPOS = "isPOS";
  string SORT = "SORT";
  string FILTER = "FILTER";
  string isODD = "isODD";
  string REVERSE = "REVERSE";
  string ACCESS = "ACCESS";
  string isEVEN = "isEVEN";
  string COUNT = "COUNT";
  string TAKE = "TAKE";
  string MAXIMUM = "MAXIMUM";
  string DROP = "DROP";

  map<string, vector<string> > components;
  components["arr_min"] = vector<string>({MINIMUM});
  components["arr_max"] = vector<string>({MAXIMUM});
  components["arr_head"] = vector<string>({HEAD});
  components["arr_last"] = vector<string>({LAST});
  components["arr_sum"] = vector<string>({MAP, SUM});

  components["count_is_pos"] = vector<string>({COUNT, isPOS});
  components["count_is_neg"] = vector<string>({COUNT, isNEG});
  components["count_is_even"] = vector<string>({COUNT, isEVEN});
  components["count_is_odd"] = vector<string>({COUNT, isODD});

  components["filter_is_pos"] = vector<string>({FILTER, isPOS});
  components["filter_is_neg"] = vector<string>({FILTER, isNEG});
  components["filter_is_even"] = vector<string>({FILTER, isEVEN});
  components["filter_is_odd"] = vector<string>({FILTER, isODD});

  components["map_increment"] = vector<string>({MAP, INC});
  components["map_decrement"] = vector<string>({MAP, INC});
  components["map_mult2"] = vector<string>({MAP, SHL});
  components["map_div2"] = vector<string>({MAP, SHR});
  components["map_negate"] = vector<string>({MAP, doNEG});
  components["map_sqr"] = vector<string>({MAP, SQR});
  components["map_mult3"] = vector<string>({MAP, MUL3});
  components["map_div3"] = vector<string>({MAP, DIV3});
  components["map_mult4"] = vector<string>({MAP, MUL4});
  components["map_div4"] = vector<string>({MAP, DIV4});

  components["sort"] = vector<string>({SORT});
  components["reverse"] = vector<string>({REVERSE});

  components["take"] = vector<string>({TAKE});
  components["drop"] = vector<string>({DROP});
  components["access"] = vector<string>({ACCESS});


  components["scanl_add"] = vector<string>({SCANL, PLUS});
  components["scanl_subtract"] = vector<string>({SCANL, MINUS});
  components["scanl_mult"] = vector<string>({SCANL, TIMES});
  components["scanl_max"] = vector<string>({SCANL, MAX});
  components["scanl_min"] = vector<string>({SCANL, MIN});

  components["zipwith_add"] = vector<string>({ZIPWITH, PLUS});
  components["zipwith_subtract"] = vector<string>({ZIPWITH, MINUS});
  components["zipwith_mult"] = vector<string>({ZIPWITH, TIMES});
  components["zipwith_max"] = vector<string>({ZIPWITH, MAX});
  components["zipwith_min"] = vector<string>({ZIPWITH, MIN});


  ifstream f;
  f.open(order_filename);
  CHECK(f.is_open(), "Failed to open file " + order_filename);

  map<string, double> name_to_prob;

  double prob;
  string name;
  while (f >> prob) {
    f >> name;
    name_to_prob.insert(make_pair(name, prob));
  }

  vector<pair<double, Successor> > weighted_successors;
  for (auto &succ : successors_) {
    double prob = 1.0;
    for (auto &component : components[succ.Name()]) {
      prob = min(prob, name_to_prob[component]);
    }
    //if (prob >= 0)
    weighted_successors.push_back(make_pair(prob, succ));
  }
  successors_.clear();

  sort(weighted_successors.begin(), weighted_successors.end(), SuccPairCompare);

  int ii = 0;
  for (auto &p : weighted_successors) {
    //cout << p.first << " " << p.second.Name() << endl;
    if ((sort_and_add_cutoff == -1) || (ii < sort_and_add_cutoff))
      successors_.push_back(p.second);
    ii++;
  }
}


/**
 *
 *  Successor
 *
 **/

Successor::Successor() {

}


Successor::Successor(DatumOp1 op, DatumType arg1_type) {
  op_type_ = Op1;
  op1_ = op;
  op1_arg_type_ = arg1_type;
}


Successor::Successor(DatumOp2 op, DatumType arg1_type, DatumType arg2_type) {
  op_type_ = Op2;
  op2_ = op;
  op2_arg1_type_ = arg1_type;
  op2_arg2_type_ = arg2_type;
}


Successor::Successor(string name, DatumOp1 op, DatumType arg1_type) {
  op_type_ = Op1;
  op1_ = op;
  op1_arg_type_ = arg1_type;
  name_ = name;
}


Successor::Successor(string name, DatumOp2 op, DatumType arg1_type, DatumType arg2_type) {
  op_type_ = Op2;
  op2_ = op;
  op2_arg1_type_ = arg1_type;
  op2_arg2_type_ = arg2_type;
  name_ = name;
}


string Successor::Name() const {
  return name_;
}


void Successor::SetOp1Arg(int arg1) {
  op1_arg_ = arg1;
}


void Successor::SetOp2Args(int arg1, int arg2) {
  op2_arg1_ = arg1;
  op2_arg2_ = arg2;
}


bool Successor::IsOp1() const {
  return op_type_ == Op1;
}


DatumOp1 Successor::GetOp1() const {
  return op1_;
}


int Successor::GetOp1Arg() const {
  return op1_arg_;
}


DatumType Successor::GetOp1ArgType() const {
  return op1_arg_type_;
}


bool Successor::IsOp2() const {
  return op_type_ == Op2;
}


int Successor::GetOp2Arg1() const {
  return op2_arg1_;
}


int Successor::GetOp2Arg2() const {
  return op2_arg2_;
}


DatumType Successor::GetOp2Arg1Type() const {
  return op2_arg1_type_;
}


DatumType Successor::GetOp2Arg2Type() const {
  return op2_arg2_type_;
}


DatumOp2 Successor::GetOp2() const {
  return op2_;
}


#include "program_state.h"
#include <algorithm>
#include <iostream>


ProgramState::ProgramState(vector<Datum *> &input_data) {
  data_.assign(input_data.begin(), input_data.end());
  num_inputs_ = input_data.size();
  num_used_ = num_inputs_;
}


ProgramState::~ProgramState() {
  for (auto &datum : data_)  delete datum;
}


int ProgramState::NumUsed() const {
  return num_used_;
}


int ProgramState::NumInts() const {
  int counter = 0;
  for (int i = 0; i < NumUsed(); i++) {
    if(data_[i]->Type() == Int)  counter++;
  }
  return counter;
}


int ProgramState::NumArrays() const {
  int counter = 0;
  for (int i = 0; i < NumUsed(); i++) {
    if(data_[i]->Type() == Array)  counter++;
  }
  return counter;
}


Datum *ProgramState::GetDatum(int i) const{
  return data_[i];
}


Datum *ProgramState::GetLastDatum() const{
  return data_[num_used_ - 1];
}


DatumType ProgramState::GetDatumType(int i) const{
  return data_[i]->Type();
}


void ProgramState::ApplyOp1(DatumOp1 op, int arg1_idx) {
  int result_idx = EnsureSpaceForOpResult();

  Datum *arg1 = GetDatum(arg1_idx);

  op(arg1, data_[result_idx]);
  num_used_++;
}


void ProgramState::ApplyOp2(DatumOp2 op, int arg1_idx, int arg2_idx) {
  int result_idx = EnsureSpaceForOpResult();

  Datum *arg1 = GetDatum(arg1_idx);
  Datum *arg2 = GetDatum(arg2_idx);

  op(arg1, arg2, data_[result_idx]);
  num_used_++;
}

// Returns index to use to store next result
int ProgramState::EnsureSpaceForOpResult() {
  int num_allocated = data_.size();
  if (num_used_ == num_allocated) {
    data_.push_back(new Datum());  // TODO: make sure this allocation is rare
  }
  return num_used_;
}


void ProgramState::Pop() {
  num_used_ -= 1;
}


ostream &operator<<(ostream &os, const ProgramState &ps)
{
  for (int i = 0; i < ps.NumUsed(); i++) {
    os << "  Register " << i << ": " << *ps.GetDatum(i) << endl;
  }
  return os;
}

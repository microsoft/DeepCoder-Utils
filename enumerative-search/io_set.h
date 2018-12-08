#ifndef _IO_SET__
#define _IO_SET__

#include <vector>
#include <algorithm>
#include <ostream>
#include <string>
#include "definitions.h"


class ProgramState;
class Datum;

using namespace std;

class IOSet {
 public:
  IOSet(string dir_name, int problem_idx, int num_examples_per_program);
  IOSet(vector<ProgramState *> &inputs, vector<Datum *> &outputs);
  ~IOSet();

  ProgramState *GetInputs(int idx);
  Datum *GetOutput(int idx);
  int Size() const;

  void ApplyOp1(DatumOp1 op, int arg1_idx);
  void ApplyOp2(DatumOp2 op, int arg1_idx, int arg2_idx);
  void Pop();
  bool IsSolved();

 protected:
  vector<ProgramState *> inputs_;
  vector<Datum *> outputs_;

  void LoadData(string types_filename, string data_filename, int problem_idx,
                int num_examples_per_program, /*out*/vector<ProgramState *> &result);

  void LoadTypes(string filename, int problem_idx, vector<DatumType> &result);
  void LoadValues(string filename, int problem_idx, int num_examples_per_program,
                  vector<DatumType> &types, /*out*/ vector<ProgramState *> &inputs);

  friend ostream &operator<<(ostream &os, const IOSet &d);
};

#endif

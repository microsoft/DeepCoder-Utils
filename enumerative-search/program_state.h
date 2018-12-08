#ifndef _PROGRAM_STATE__
#define _PROGRAM_STATE__

#include "datum.h"
#include "definitions.h"

class ProgramState {
  /*
    ProgramState represents the state of memory for one run of a program.
    To construct a ProgramState object, give it a vector of Datum * objects,
    where each Datum corresponds to one input variable's value.
    When new intermediate results are created, they can be added by calling
    AddDatum(Datum *).
   */
 public:
  ProgramState(vector<Datum *> &input_data);
  ~ProgramState();

  Datum* GetDatum(int i) const;
  Datum* GetLastDatum() const;
  DatumType GetDatumType(int i) const;

  int NumUsed() const;
  int NumAllocated() const;
  int NumInts() const;
  int NumArrays() const;

  void ApplyOp1(DatumOp1 op, int arg1_idx);
  void ApplyOp2(DatumOp2 op, int arg1_idx, int arg2_idx);

  // Remove the last Datum but don't free memory. Used when backtracking in search
  void Pop();

 protected:
  int EnsureSpaceForOpResult();

  vector<Datum *> data_;
  int num_inputs_;
  int num_used_;

  friend ostream &operator<<(ostream &os, const ProgramState &d);
};

#endif

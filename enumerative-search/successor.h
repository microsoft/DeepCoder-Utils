#ifndef _SUCCESSOR__
#define _SUCCESSOR__


#include <utility>
#include <vector>
#include <string>

#include "definitions.h"


class IOSet;
class ProgramState;
class SuccessorIterator;

enum OpType {Op1, Op2};

class Successor {
  /*
   * Successor represents an op that can be applied to a ProgramState.
   * It hides the type signature of the op so that the same object can
   * be used to apply ops of different types.
   */

  friend class SuccessorIterator;

 public:
  Successor();
  Successor(DatumOp1 op, DatumType arg1_type);
  Successor(DatumOp2 op, DatumType arg1_type, DatumType arg2_type);
  Successor(string name, DatumOp1 op, DatumType arg1_type);
  Successor(string name, DatumOp2 op, DatumType arg1_type, DatumType arg2_type);

  string Name() const;
  bool IsOp1() const;
  DatumOp1 GetOp1() const;
  int GetOp1Arg() const;
  DatumType GetOp1ArgType() const;

  bool IsOp2() const;
  DatumOp2 GetOp2() const;
  int GetOp2Arg1() const;
  int GetOp2Arg2() const;
  DatumType GetOp2Arg1Type() const;
  DatumType GetOp2Arg2Type() const;

  void SetOp1Arg(int arg1);
  void SetOp2Args(int arg1, int arg2);

 protected:
  OpType op_type_;
  DatumOp1 op1_;
  int op1_arg_;
  DatumType op1_arg_type_;

  DatumOp2 op2_;
  int op2_arg1_;
  int op2_arg2_;
  DatumType op2_arg1_type_;
  DatumType op2_arg2_type_;

  string name_;
  double weight_;

  friend ostream &operator<<(ostream &os, const Successor &s);
};


class SuccessorIterator {
  /*
   * SuccessorIterator stores the logic of how to traverse the search tree.
   * It knows what are the valid ops to apply to a given program state,
   * and it decides how to order them.
   */
 public:
  SuccessorIterator();
  SuccessorIterator(IOSet *io);
  SuccessorIterator(string order_filename, IOSet *io, int sort_and_add_cutoff);

  ~SuccessorIterator();
  void InitOps();
  void InitSuccessors();
  void InitSuccessors(string order_filename, int sort_and_add_cutoff);

  void Init(IOSet *io);

  const Successor *Next();
  const Successor *Cur();

 protected:
  // ProgramState *ps_;
  int counter_;
  vector<Successor> successors_;
  vector<pair<double, Successor> > scores_and_successors_;
  vector<DatumType> types_;
  vector<int> cumulative_counts_;

  // TODO: distinguish between commutative and non-commutative ops
  vector<pair<string, DatumOp1> > int_to_int_ops_;
  vector<pair<string, DatumOp1> > arr_to_int_ops_;
  vector<pair<string, DatumOp1> > arr_to_arr_ops_;
  // vector<DatumOp1> int_to_arr_ops_;
  vector<pair<string, DatumOp2> > int_arr_to_arr_ops_;
  vector<pair<string, DatumOp2> > int_arr_to_int_ops_;
  vector<pair<string, DatumOp2> > arr_arr_to_arr_ops_;
  vector<pair<string, DatumOp2> > arr_int_to_arr_ops_;
};


#endif

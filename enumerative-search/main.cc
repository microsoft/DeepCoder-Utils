#include <ctime>
#include <fstream>
#include <ctime>

#include <iomanip>

#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <unistd.h>
#include <stack>

#include "utils.h"
#include "datum.h"
#include "program_state.h"
#include "ops.h"
#include "io_set.h"
#include "successor.h"


using namespace std;


#ifndef VERBOSE_MODE
#define VERBOSE_MODE 0
#endif


void ApplySuccessor(IOSet *io, const Successor *successor) {
  if (successor->IsOp1()) {
    io->ApplyOp1(successor->GetOp1(), successor->GetOp1Arg());
  } else if (successor->IsOp2()) {
    io->ApplyOp2(successor->GetOp2(), successor->GetOp2Arg1(),
                 successor->GetOp2Arg2());
  }
}

pair<bool, pair<int, double> > run(string test_set,
                                   int problem_idx,
                                   int num_examples_per_program,
                                   int max_program_length,
                                   int ordering_type,
                                   int sort_and_add_cutoff) {

  string order_filename;
  if (ordering_type == 0) {
    order_filename = "data/" + test_set + "/prior.txt";
  } else if (ordering_type == 1) {
    order_filename = "data/" + test_set + "/predictions/" + to_string(problem_idx) + ".txt";
  } else {
    order_filename = "data/" + test_set + "/random_ordering_" + to_string(-ordering_type) + ".txt";
  }

  IOSet *io = new IOSet("data/" + test_set + "/", problem_idx, num_examples_per_program);

#if VERBOSE_MODE  
  for (int example_idx = 0; example_idx < num_examples_per_program; ++example_idx) {
    ProgramState *ps = io->GetInputs(example_idx);
    Datum *target_datum = io->GetOutput(example_idx);
    cout << "Initial state " << example_idx << ":" << endl << *ps;
    cout << "Target value " << example_idx << ": " << *target_datum << endl;
  }
  cout << endl;
#endif

  // Do search
  int num_nodes_explored = 0;
  bool succeeded = false;

  MyTimer timer;
  timer.tic();

  vector<SuccessorIterator *> iterators;
  for (int i = 0; i < max_program_length; i++) {
    iterators.push_back(new SuccessorIterator(order_filename, io, sort_and_add_cutoff));
  }


#if VERBOSE_MODE
  int num_inputs = io->GetInputs(0)->NumUsed();
  vector<string> fun_name;
  vector<bool> fun_isUnary;
  vector<int> fun_operand1, fun_operand2;
#endif

  int depth = 0;
  while (depth >=  0) {
    const Successor *successor = iterators[depth]->Next();

    if (successor != NULL) {
      ApplySuccessor(io, successor);
      num_nodes_explored++;

#if VERBOSE_MODE
      fun_name.push_back(successor->Name());
      if (successor->IsOp1()) {
        fun_isUnary.push_back(true);
        fun_operand1.push_back(successor->GetOp1Arg());
        fun_operand2.push_back(-1);
      } else {
        fun_isUnary.push_back(false);
        fun_operand1.push_back(successor->GetOp2Arg1());
        fun_operand2.push_back(successor->GetOp2Arg2());
      }
#endif

      if (io->IsSolved()) {
        succeeded = true;
        break;
      }

      if (depth < max_program_length - 1) {
        depth += 1;
        iterators[depth]->Init(io);
      } else {
        io->Pop();
#if VERBOSE_MODE
        fun_name.pop_back();
        fun_isUnary.pop_back();
        fun_operand1.pop_back();
        fun_operand2.pop_back();
#endif
      }
    } else {
      depth -= 1;
      io->Pop();
#if VERBOSE_MODE
      fun_name.pop_back();
      fun_isUnary.pop_back();
      fun_operand1.pop_back();
      fun_operand2.pop_back();
#endif
    }
  }
  double secs_taken = timer.toc();

  // Print result
  if (succeeded) {
    cout << "Solved!" << endl;
  } else {
    cout << "Failed!" << endl;
  }
  cout << "Nodes explored: " << num_nodes_explored << endl;
  cout << secs_taken << endl;
  delete io;

#if VERBOSE_MODE
  // Print the solution found
  if (succeeded) {
    cout << "Solution:" << endl;
    for (int i = 0; i <= depth; ++i) {
      cout << " %" << i + num_inputs << " <- " << fun_name[i] << " %" << fun_operand1[i];
      if (!fun_isUnary[i]) {
        cout << " %" << fun_operand2[i];
      }
      cout << endl;
    }
  }
#endif

  // Only delete iterators after having printed the solution
  for (int i = 0; i < max_program_length; i++) {
    delete iterators[i];
  }
  iterators.clear();

  return make_pair(succeeded, make_pair(num_nodes_explored, secs_taken));
}


int main(int argc, char *argv[])
{
  if (argc != 7) {
    cout << "Usage:" << endl;
    cout << "  search TEST_SET_NAME NUM_EXAMPLES MAX_PROG_LEN PROB_IDX ORDER_TYPE SaA_CUTOFF" << endl;
    exit(1);
  }
  string test_set = argv[1];
  int num_examples_per_program = atoi(argv[2]);
  int max_program_length = atoi(argv[3]);
  int problem_idx = atoi(argv[4]);
  int ordering_type = atoi(argv[5]); // 1 nn, 0 prior, -1,-2,-3,-4,-5,... random orderings 
  int sort_and_add_cutoff = atoi(argv[6]);
  run(test_set, problem_idx, num_examples_per_program, max_program_length, ordering_type, sort_and_add_cutoff);
}

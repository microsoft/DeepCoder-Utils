#include "io_set.h"
#include "program_state.h"
#include "datum.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>


ostream &operator<<(ostream &os, const IOSet &io)
{
  os << "IOSet" << "( ";
  for (int i = 0; i < io.Size(); i++) {
    os << "State:  " << endl << *io.inputs_[i] << endl;
    os << "Target: " << endl << *io.outputs_[i] << endl << endl;
  }
  os << ")";
  return os;
}


IOSet::IOSet(string dir_name, int problem_idx, int num_examples_per_program) {

  vector<ProgramState *> inputs;
  LoadData(dir_name + "/input_types.txt", dir_name + "/input_values.txt",
           problem_idx, num_examples_per_program, inputs);

  vector<ProgramState *> output_program_states;
  LoadData(dir_name + "/output_types.txt", dir_name + "/output_values.txt",
           problem_idx, num_examples_per_program, output_program_states);

  // This is a bit ugly as typically program state owns the memory of
  // its included datums. However, here we're taking control of the datums
  // and discarding the program states. As of now, program state allocates
  // no other memory, so it's ok to not call delete. In the future if
  // program state will allocate other memory beyond the datums, then we should
  // rethink this.
  vector<Datum *> outputs;
  for (auto &ps : output_program_states) {
    outputs.push_back(ps->GetDatum(0));
  }

  inputs_.assign(inputs.begin(), inputs.end());
  outputs_.assign(outputs.begin(), outputs.end());
}


IOSet::IOSet(vector<ProgramState *> &inputs, vector<Datum *> &outputs) {
  inputs_.assign(inputs.begin(), inputs.end());
  outputs_.assign(outputs.begin(), outputs.end());
}


IOSet::~IOSet() {
  for (auto &ps : inputs_)  delete ps;
  for (auto &datum : outputs_)  delete datum;
}


ProgramState *IOSet::GetInputs(int idx) {
  return inputs_[idx];
}


int IOSet::Size() const {
  return inputs_.size();
}


Datum *IOSet::GetOutput(int idx) {
  return outputs_[idx];
}


void IOSet::ApplyOp1(DatumOp1 op, int arg1_idx) {
  for (auto &ps : inputs_)  ps->ApplyOp1(op, arg1_idx);
}


void IOSet::ApplyOp2(DatumOp2 op, int arg1_idx, int arg2_idx) {
  for (auto &ps : inputs_)  ps->ApplyOp2(op, arg1_idx, arg2_idx);
}


void IOSet::Pop() {
  for (auto &ps : inputs_)  ps->Pop();
}


bool IOSet::IsSolved() {
  for (int i = 0; i < inputs_.size(); i++) {
    ProgramState *ps = inputs_[i];
    Datum *target = outputs_[i];
    if (*ps->GetLastDatum() != *target)  return false;
  }
  return true;
}


void IOSet::LoadData(string types_filename, string data_filename, int problem_idx,
                     int num_examples_per_program, vector<ProgramState *> &result) {

  vector<DatumType> types;
  LoadTypes(types_filename, problem_idx, types);

  vector<ProgramState *> inputs;
  LoadValues(data_filename, problem_idx, num_examples_per_program, types, result);
}


void IOSet::LoadTypes(string filename, int problem_idx, vector<DatumType> &result) {
  result.clear();

  ifstream types_file;
  types_file.open(filename);

  string line;
  for (int i = 0; i < problem_idx + 1; i++)  getline(types_file, line);

  stringstream ss;
  ss << line;

  string type;
  while (ss >> type) {
    if (type == "Array")  result.push_back(Array);
    else if (type == "Int")  result.push_back(Int);
    else CHECK(false, "Unexpected type " + type);
  }
}


void IOSet::LoadValues(string filename, int problem_idx, int examples_per_program,
                       vector<DatumType> &types, vector<ProgramState *> &result) {
  ifstream file;
  file.open(filename);

  string line;
  for (int i = 0; i < problem_idx; i++) {
    for (int j = 0; j < examples_per_program; j++) {
      getline(file, line);
    }
  }

  for (int j = 0; j < examples_per_program; j++) {
    // read arguments into a vector<vector<int> > data structure
    getline(file, line);

    stringstream ss;
    ss << line;

    string element;
    int argument_idx = 0;
    vector<vector<int> > values(1);
    while (ss >> element) {
      if (element == "|") {
        argument_idx++;
        values.push_back(vector<int>());

      } else {
        int value = atoi(element.c_str());
        values[argument_idx].push_back(value);
      }
    }

    // make sure the read was consistent with types data loaded earlier
    CHECK_EQ(values.size(), types.size(), "inconsistency in values and types data");

    // construct ProgramState object for this example
    vector<Datum *> data;
    for (int i = 0; i < values.size(); i++) {
      if (types[i] == Array) {
        data.push_back(new Datum(values[i]));
      } else {
        data.push_back(new Datum(values[i][0]));
      }
    }
    ProgramState *ps = new ProgramState(data);
    // cout << "Loaded program state" << endl;
    // cout << *ps << endl;

    result.push_back(ps);
  }
}


/*
void IOSet::LoadOutputs(string filename, int problem_idx, int examples_per_program,
                        vector<Datum *> &outputs) {
  ifstream outputs_file;
  outputs_file.open(filename);

  string line;
  for (int i = 0; i < problem_idx; i++) {
    for (int j = 0; j < examples_per_program; j++) {
      getline(outputs_file, line);
    }
  }

  for (int j = 0; j < examples_per_program; j++) {
    // read arguments into a vector<vector<int> > data structure
    getline(inputs_file, line);
    stringstream ss;
    ss << line;

    string element;
    int argument_idx = 0;
    vector<vector<int> > input_values(1);
    while (ss >> element) {
      if (element == "|") {
        argument_idx++;
        input_values.push_back(vector<int>());

      } else {
        int value = atoi(element.c_str());
        input_values[argument_idx].push_back(value);
      }
    }

    // make sure the read was consistent with types data loaded earlier
    CHECK_EQ(input_values.size(), input_types_.size(), "inconsistency in input data");

    // construct ProgramState object for this example
    vector<Datum *> input_data;
    for (int i = 0; i < input_values.size(); i++) {
      if (input_types_[i] == Array) {
        input_data.push_back(new Datum(input_values[i]));
      } else {
        input_data.push_back(new Datum(input_values[i][0]));
      }
    }
    ProgramState *ps = new ProgramState(input_data);
    cout << "Loaded input program state" << endl;
    cout << *ps << endl;

    inputs.push_back(ps);
  }
}
*/

#include "datum.h"
#include "utils.h"


ostream &operator<<(ostream &os, const DatumType &dt) {
  string val = "";
  switch (dt) {
  case 0: val = "Int"; break;
  case 1: val = "Array"; break;
  }
  return os << val;
}


ostream &operator<<(ostream &os, const Datum &d)
{
  os << d.type_ << "( ";
  for (auto &value : d.values_)  os << value << " ";
  os << ")";
  return os;
}

bool operator==(const Datum &lhs, const Datum &rhs) {
  if (lhs.Type() != rhs.Type()) {
    return false;

  } else if (lhs.Type() == Int) {
    return lhs.GetIntValue() == rhs.GetIntValue();

  } else if (lhs.Size() != rhs.Size()) {
    return false;

  } else {
    for (int i = 0; i < lhs.Size(); i++) {
      if (lhs.GetArrayElementValue(i) != rhs.GetArrayElementValue(i)) {
        return false;
      }
    }
    return true;
  }
}


bool operator!=(const Datum &lhs, const Datum &rhs) {
  return !(lhs == rhs);
}


Datum::Datum()
{}


Datum::Datum(DatumType type) :
  type_(type)
{}


Datum::Datum(int value)
{
  SetIntValue(value);
}


Datum::Datum(const vector<int> &values)
{
  SetArrayValue(values);
}


DatumType Datum::Type() const {
  return type_;
}

void Datum::SetType(DatumType type) {
  type_ = type;
}


int Datum::GetIntValue() const {
  CHECK_EQ(Type(), Int, "Can't get Int value from non-Int Datum");
  return values_[0];
}


void Datum::SetIntValue(int value) {
  type_ = Int;
  values_.clear();
  values_.push_back(value);
}


void Datum::SetArrayValue(const vector<int> &values) {
  type_ = Array;
  values_.assign(values.begin(), values.end());
}


void Datum::SetArrayElementValue(int idx, int value) {
  CHECK_LT(idx, values_.size(), "Index out of bounds");
  CHECK_EQ(Type(), Array, "Can't set element value of non-array");

  values_[idx] = value;
}


int Datum::GetArrayElementValue(int idx) const {
  CHECK_LT(idx, values_.size(), "Index out of bounds");
  CHECK_EQ(Type(), Array, "Can't get element value of non-array");

  return values_[idx];
}


void Datum::Resize(int size) {
  CHECK_EQ(Type(), Array, "Can't resize non-array");
  values_.resize(size);
}


int Datum::Size() const {
  CHECK_EQ(Type(), Array, "Can't get size of non-array");
  return values_.size();
}

vector<int> *Datum::GetValues() {
  return &values_;
}

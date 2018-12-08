#ifndef _DATUM__
#define _DATUM__

#include <vector>
#include <algorithm>
#include <ostream>
#include "definitions.h"


using namespace std;


class Datum {
 public:
  Datum();
  Datum(DatumType type);
  Datum(int val);
  Datum(const vector<int> &val);

  void SetIntValue(int value);
  void SetArrayValue(const vector<int> &values);

  int GetIntValue() const;
  void SetArrayElementValue(int idx, int value);
  int GetArrayElementValue(int idx) const;

  void Resize(int size);
  int Size() const;

  DatumType Type() const;
  void SetType(DatumType type);

  vector<int> *GetValues();
  void SetValues(vector<int> &values);

 protected:
  vector<int> values_;
  DatumType type_;

  friend ostream &operator<<(ostream &os, const Datum &d);
};

bool operator==(const Datum &lhs, const Datum &rhs);
bool operator!=(const Datum &lhs, const Datum &rhs);

#endif

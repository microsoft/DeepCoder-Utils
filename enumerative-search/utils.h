// Macros and utilities to help with debugging and compiling.

#ifndef DANNY_UTILS__
#define DANNY_UTILS__

#include <stdio.h>
#include <stdarg.h>
#include "assert.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>
#include <sys/time.h>

// boost required for random number generator
//#include <boost/random.hpp>
//#include <boost/random/normal_distribution.hpp>

#include "definitions.h"

using namespace std;


#define ISEVEN(a) ((a) % 2 == 0)
#define ISODD(a) ((a) % 2 == 1)

#define DEBUG 1

#if DEBUG == 1

#define SHOW(x) \
  cout << #x " had value of " << x << " at "				\
  << __FILE__ << " line " << __LINE__ << endl << endl;

// Advanced versions of assert macros that give more informative error
// messages (CHECK family), and that allow the program to keep running
// if it's a minor assertion that failed (EXPECT family).
//
// These were inspired by Google.
#define CHECK(x, error_msg)						\
  if (!(x)) {								\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK(" << #x << ") failed at " << __FILE__		\
	 << " line " << __LINE__ << endl;				\
    assert(false);							\
  }

#define CHECK_NULL(x, error_msg)					\
  if ((x) != NULL) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_NULL(" << #x ") failed at "				\
  	 << __FILE__ << " line " << __LINE__				\
	 << " (had value of " << x << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_NAN(x, error_msg)					\
  if (std::isnan(x)) {                            \
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_NAN(" << #x ") failed at "				\
  	 << __FILE__ << " line " << __LINE__				\
	 << " (had value of " << x << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_SMALL(x, error_msg)					\
  if ((x) > 1e100 || (x) < -1e100 || isnan(x)) {			\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_SMALL(" << #x ") failed at "				\
  	 << __FILE__ << " line " << __LINE__				\
	 << " (had value of " << x << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_NOT_NULL(x, error_msg)   				\
  if ((x) == NULL) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_NOT_NULL(" << #x ") failed at "			\
  	 << __FILE__ << " line " << __LINE__ << endl;			\
    assert(false);							\
  }
    
#define CHECK_EQ(x,y, error_msg)					\
  if (!(x == y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_EQ(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_EPS_EQ(x,y,eps, error_msg)				\
  if (!(fabs(x-y) <= eps)) {						\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_EQ(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_LT(x,y, error_msg)					\
  if ((x >= y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_LT(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_GT(x,y, error_msg)					\
  if ((x <= y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_GT(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_GE(x,y, error_msg)					\
  if ((x < y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_GE(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_LE(x,y, error_msg)					\
  if ((x > y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_GE(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define CHECK_NEQ(x,y, error_msg)					\
  if ((x==y)) {								\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "CHECK_NEQ(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
    assert(false);							\
  }

#define EXPECT(x, error_msg)						\
  if (!(x)) {								\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "EXPECT(" << #x << ") failed at " << __FILE__		\
	 << " line " << __LINE__ << endl;				\
  }

#define EXPECT_NULL(x, error_msg)					\
  if ((x) != NULL) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "EXPECT_NULL(" << #x ") failed at "				\
  	 << __FILE__ << " line " << __LINE__				\
	 << " (had value of " << x << ")" << endl;			\
  }

#define EXPECT_EQ(x,y, error_msg)					\
  if (!(x==y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "EXPECT_EQ(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
  }

#define EXPECT_NEQ(x,y, error_msg)					\
  if ((x==y)) {								\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "EXPECT_NEQ(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
  }

#define EXPECT_GT(x,y, error_msg)					\
  if ((x <= y)) {							\
    cerr << "ERROR: " << error_msg << endl;				\
    cerr << "EXPECT_GT(" << #x << ", " << #y << ") failed at "		\
	 << __FILE__ << " line " << __LINE__				\
	 << " (" << x << " vs. " << y << ")" << endl;			\
  }

#endif  // DEBUG == 1


// Call this in the private section of a class definition to disallow
// default copy constructor and default assignment operator.  Can save
// troubles debugging some weird errors.
#define DISALLOW_COPY_CONSTRUCTORS(x)		\
  x(x &other);					\
  x &operator=(x &other);

// Like sprintf, but with std::strings and a nicer interface (no need
// to deal with annoying char[256] type buffer things).
string StringPrintf(const char *format, ...);

double Sigmoid(double z);

float Sigmoidf(float x);

double Square(double z);

double LogSum(double log_a, double log_b);

// Use dynamic programming to compute the edit distance between two
// strings.  Supported operations are:
//   Match
//   Insertion
//   Deletion
//   Swap
// To edit the costs, modify the inlined functions below.
double StringDistance(string s1, string s2);


// If costs are fixed, they should obey the following inequality to
// ensure that all types of operations are used:
//
//   (2 x match) <= swap <= (insertion + match + deletion)
//
double MatchCost(char a, char b);
double InsertionCost(char a);
double DeletionCost(char a);
// cost of matching [a1 a2] to [b1 b2]
double SwapCost(char a1, char a2, char b1, char b2);

bool PairCompareFirst(pair<double, int> a, pair<double, int> b);

// My implementation of Matlab's fullfile() function. 
// Concatenate two strings holding parts of a path, making sure that the pathseparator (/) 
// does not get repeated or missed. 
// TODO: Extend to variable inputs
string fullfile2(string path, string file);

// Class to hold cute matlab-style tic-toc functions to time things
class MyTimer {
    clock_t clk_start;
    timeval tm_start;
    
    double clk_diff; 
    double tm_diff;
    
public:
    
    MyTimer() : 
    clk_start(0), clk_diff(0), tm_diff(0)
    {}
    
    void tic();
    double toc(bool print=true, double* pclk_diff=NULL, double *ptm_diff=NULL);
};

// Split a path into dirname and basename
void splitpath(string path, string& dirname, string& basename);
// Split a filename into name and extension
void splitfname(string fname, string& fname_noext, string& ext);

real_t randn(real_t dummy = 0);


#endif  // DANNY_UTILS__

#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <math.h>
#include <iostream> 

string StringPrintf(const char *format, ...) {
  va_list arg;
  char buffer[512];  // TODO(medium): Should check for overflow
  va_start(arg, format);
  vsprintf(buffer, format, arg);
  va_end (arg);  

  return string(buffer);
}

double Sigmoid(double z) {
  return 1 / (1 + exp(-z));
}

double Square(double z) {
  return z*z;
}


float Sigmoidfb(float z) {
  return 1 / (1 + exp(-z));
}


double LogSum(double log_a, double log_b) {
  if (log_a < -10000000) return log_b;
  if (log_b < -10000000) return log_a;
  if (log_a < log_b) {
    return log_a + log(exp(log_a - log_b));
  } else {
    return log_b + log(exp(log_b - log_a));
  }
}

double StringDistance(string s1, string s2) {
  double costs[s1.length()][s2.length()];

  costs[0][0] = BIG_DOUBLE;
  for (uint i = 0; i < s1.length(); i++) {
    for (uint j = 0; j < s2.length(); j++) {
      // Cost of coming from each of the directions wrt current cell
      // (up & left, up, left, up & up & left & left)
      double ul, u, l, uull;
      ul = u = l = uull = BIG_DOUBLE;

      // Single character operations
      if (i > 0 && j > 0) {
	ul = costs[i-1][j-1] + MatchCost(s1[i], s2[j]);
      }
      if (i > 0) {
	u = costs[i-1][j] + DeletionCost(s1[i]);
      }
      if (j > 0) {
	l = costs[i][j-1] + InsertionCost(s2[j]);
      }
      
      // Multiple (2) character operations
      if (i > 1 && j > 1) {
	uull = costs[i-2][j-2] + SwapCost(s1[i-1], s1[i], s2[j-1], s2[j]);
      }

      double best_cost = min(ul,min(u, min(l, uull)));
      if (best_cost == BIG_DOUBLE) best_cost = 0;

      costs[i][j] = best_cost;

      if (i == 0 && j == 0) {
	costs[i][j] = MatchCost(s1[0], s2[0]);
      }
    }
  }

  bool print_matrix = false;
  if (print_matrix) {
    cout << "  ";
    for (uint j = 0; j < s2.length(); j++) {
      cout << s2[j] << " ";
    }
    cout << endl;
    for (uint i = 0; i < s1.length(); i++) {
      cout << s1[i] << " ";
      for (uint j = 0; j < s2.length(); j++) {
	cout << costs[i][j] << " ";
      }
      cout << endl;
    }
  }  

  return costs[s1.length()-1][s2.length()-1];
}

double MatchCost(char a, char b) { 
  return (a == b) ? 0 : 1.2;
}

double InsertionCost(char a) {
  a = 1;
  return 1.2;
}

double DeletionCost(char a) {
  a = 1;
  return 1.1;
}

// cost of matching [a1 a2] to [b1 b2]
double SwapCost(char a1, char a2, char b1, char b2) {
  return 1;
  return (a1 == b2 && a2 == b1) ? 1 : MatchCost(a1, b1) + MatchCost(a2, b2);
}

bool PairCompareFirst(pair<double, int> a, pair<double, int> b) {
  return (a.first > b.first);
}


// My implementation of Matlab's fullfile() function. 
// Concatenate two strings holding parts of a path, making sure that the pathseparator (/) 
// does not get repeated or missed. 
// TODO: Extend to variable inputs
string fullfile2(string path, string file)
{
    // check if file is empty
    int len = path.length();
    if (len == 0)
        path += '/';
    
    // check for file-separator at end of path. If not present, add.
    if ( !(path.at(len-1) == '/') && !(path.at(len-1) == '\\') )
        path = path + "/" + file;
    else
        path += file;
    
    len = path.length();
//    if ( !(path.at(len-1) == '/') && !(path.at(len-1) == '\\') )
//        path += "/";
    
    return path;
}

// Class to hold cute matlab-style tic-toc functions to time things
void MyTimer::tic()
{
    clk_start = clock();
    //tm_start = time(NULL);
    gettimeofday(&tm_start,NULL);
}

double MyTimer::toc(bool print, double* pclk_diff, double *ptm_diff)
{
    // CPU time
    clk_diff = (double) (clock() - clk_start);
    //printf("%lf %d\n", clk_diff, CLOCKS_PER_SEC);
    clk_diff /= CLOCKS_PER_SEC;
    //printf("%lf %d\n", clk_diff, CLOCKS_PER_SEC);
    
    
    // Real time
    timeval tm_end;
    gettimeofday(&tm_end,NULL);
    
    tm_diff = (double)(tm_end.tv_sec - tm_start.tv_sec) 
    + (double)(tm_end.tv_usec - tm_start.tv_usec)/1000000;
    
    if (print)
        printf("Time Elapsed: %lf (CPU), %lf (Real) secs\n",clk_diff,tm_diff);
    
    if (pclk_diff)
        *pclk_diff = clk_diff;
    if (ptm_diff)
        *ptm_diff = tm_diff;
    
    if (clk_diff > 0) {
        //printf("Returning clk_diff");
        return clk_diff;
    }
    else {
        //printf("Returning tm_diff");
        return tm_diff;
    }
}

// Split a path into dirname and basename
void splitpath(string path, string& dirname, string& basename)
{
    size_t pos = path.find_last_of("/\\");
    
    EXPECT_NEQ(pos, string::npos, "Basename empty");
    dirname = path.substr(0,pos);
    basename = path.substr(pos+1);
}

// Split a filename into name and extension
void splitfname(string fname, string& fname_noext, string& ext)
{
    size_t pos = fname.find_last_of(".");
    
    EXPECT_NEQ(pos, string::npos, "No Extention Found");
    fname_noext = fname.substr(0,pos);
    ext = fname.substr(pos);
}

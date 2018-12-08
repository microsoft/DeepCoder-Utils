#ifndef _DEPTH_FIRST_SEARCH__
#define _DEPTH_FIRST_SEARCH__

#include <queue>

class IOSet;

class DepthFirstSearch {
 public:
  DepthFirstSearch(IOSet *io);
  ~DepthFirstSearch();


 protected:
  IOSet *io;

};

#endif

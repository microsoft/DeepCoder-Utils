# Overview

This repository contains some utilities used in the evaluation of [DeepCoder: Learning to Write Programs](https://arxiv.org/abs/1611.01989). If you want to cite this, please use the following bibtex entry:

```
@InProceedings{balog2016deepcoder,
  author    = {Matej Balog and
               Alexander L. Gaunt and
               Marc Brockschmidt and
               Sebastian Nowozin and
               Daniel Tarlow},
  title     = {DeepCoder: Learning to Write Programs},
  booktitle = {International Conference on Representation Learning (ICLR)},
  year      = {2017}
}
```

This repository contains the code we used to generate input-output examples for programs in our custom language, as well as an implementation of enumerative search to find programs given input-output examples. We currently do not provide the implementation of the actual learned model; but alternative (and more advanced implementations) are available from other authors, e.g., [PCCoder](https://github.com/amitz25/PCCoder).

## Generating Input-Output Examples

This functionality is provided by the Python script `generate_io_samples.py` (mainly authored by Matej Balog). The code is meant to be used as a library, but can also be used as a command line tool:

```
$ ./generate_io_samples.py --number 3 --length 7 --value-range 50 "a <- [int] | b <- int | c <- TAKE b a | d <- COUNT isEVEN c | e <- TAKE d a"
[[-25, -2, -21, -11, 46, -41, 27], 2] -> [-25]
[[30, -23, 17, 29, 46, 13, 46], 2] -> [30]
[[44, 15, -49, -28, 22, 9, -38], 5] -> [44, 15, -49]
```

Programs are specified by a string, where individual statements are separated by " | ".

## Searching for Programs

This functionality is provided by the C++ code in `enumerative-search` (mainly authored by Danny Tarlow). It is unpolished research-quality code, but was sufficient for the experiments in our paper.

The program search tool can be built using the provided Makefile (here in verbose mode that reports the found program; if you do not specify the `VERBOSE_MODE` flag, the search is a bit faster):
```
$ make CFLAGS="-DVERBOSE_MODE"
g++ -std=c++11 -O3  -DVERBOSE_MODE successor.cc -c -o successor.o
g++ -std=c++11 -O3  -DVERBOSE_MODE ops.cc -c -o ops.o
g++ -std=c++11 -O3  -DVERBOSE_MODE program_state.cc -c -o program_state.o
g++ -std=c++11 -O3  -DVERBOSE_MODE main.cc -c -o main.o
g++ -std=c++11 -O3  -DVERBOSE_MODE datum.cc -c -o datum.o
g++ -std=c++11 -O3  -DVERBOSE_MODE depth_first_search.cc -c -o depth_first_search.o
g++ -std=c++11 -O3  -DVERBOSE_MODE utils.cc -c -o utils.o
g++ -std=c++11 -O3  -DVERBOSE_MODE io_set.cc -c -o io_set.o
g++ -std=c++11 -O3 -DVERBOSE_MODE successor.o ops.o program_state.o main.o datum.o depth_first_search.o utils.o io_set.o -o search
```

The resulting binary can then be tested on the provided test data points:
```
$ ./search example 3 3 0 0 -1
Initial state 0:
  Register 0: Int( 8 )
  Register 1: Array( 7 9 3 1 3 0 7 2 3 1 )
Target value 0: Int( 7 )
Initial state 1:
  Register 0: Int( 7 )
  Register 1: Array( 9 5 1 6 7 2 8 8 6 2 )
Target value 1: Int( 9 )
Initial state 2:
  Register 0: Int( 3 )
  Register 1: Array( 4 2 2 1 1 3 5 8 1 8 )
Target value 2: Int( 2 )

Time Elapsed: 0.062500 (CPU), 0.066912 (Real) secs
Solved!
Nodes explored: 53084
0.0625
Solution:
 %2 <- sort %1
 %3 <- arr_head %1
 %4 <- access %3 %2
```

`search` takes 6 arguments (see below for a precise description of all involved data files):
* `TEST_SET_NAME`: The name of the dataset. Used to find files, as we are looking in `data/${TEST_SET_NAME}`.
* `NUM_EXAMPLES`: Number of examples per program.
* `MAX_PROG_LEN`: Maximum length of programs to consider.
* `PROB_IDX`: Index of problem to test on (index is used to find example data in the data files).
* `ORDER_TYPE`: Ordering type to use. `0` indicates using the prior located in `data/${TEST_SET_NAME}/prior.txt` and `1` makes the search use the data in `data/${TEST_SET_NAME}/predictions/${PROB_IDX}.txt`.
* `SaA_CUTOFF`: Cutoff used by "sort & and"-style search (i.e., we only use the top `SaA_CUTOFF` most likely functions). `-1` indicates that all functions can be used.

We can repeat the example from below with a carefully selected ordering of functions and observe a substantial speedup:
```
$ ./search example 3 3 0 1 -1
Initial state 0:
  Register 0: Int( 8 )
  Register 1: Array( 7 9 3 1 3 0 7 2 3 1 )
Target value 0: Int( 7 )
Initial state 1:
  Register 0: Int( 7 )
  Register 1: Array( 9 5 1 6 7 2 8 8 6 2 )
Target value 1: Int( 9 )
Initial state 2:
  Register 0: Int( 3 )
  Register 1: Array( 4 2 2 1 1 3 5 8 1 8 )
Target value 2: Int( 2 )

Time Elapsed: 0.015625 (CPU), 0.007318 (Real) secs
Solved!
Nodes explored: 55
0.015625
Solution:
 %2 <- arr_head %1
 %3 <- sort %1
 %4 <- access %2 %3
```

### I/O Data Format
Assume a dataset of name `NAME`, P problems with N I/O samples each, and that a_p is the number of arguments for the p-th program.
We use the following data files:
 * `data/NAME/input_types.txt`: File of P lines, each line a space-separated list of types. Example:
   ```
   Int Array
   Array Array
   ```
 * `data/NAME/input_values.txt`: File of P * N lines, each line one input sample. Example (for P=2, N=3 and types "Int Array" and "Array Array")
   ```
   8 | 7 9 3 1 3 0 7 2 3 1
   7 | 9 5 1 6 7 2 8 8 6 2
   3 | 4 2 2 1 1 3 5 8 1 8
   5 9 5 5 8 1 5 9 3 1 | 272 125 -101 -461 -381 452 -28 6 -164 132
   1 0 5 0 5 6 5 5 6 4 | 342 -399 506 -254 16 430 67 233 -385 -381
   5 1 6 8 4 8 1 6 1 3 | -320 249 -55 236 -215 -455 68 -438 -31 -468
   ```
 * `data/NAME/output_types.txt`: File of P lines, each line a single type. Example:
   ```
   Int
   Array
   ```
 * `data/NAME/output_values.txt`: File of P * N lines, each line one output sample. Example:
   ```
   7
   9
   2
   272 125 -101 -461 -381 452 -28 6 -164
   342 -399 506 -254 16 430
   -320 249 -55 236 -215 -455 68 -438
   ```
 * `data/NAME/prior.txt`: File with one line per supported function, for example reflecting dataset statistics. Example:
   ```
   0.1 ZIPWITH
   0.01 *
   0.3 MAP
   ...
   ```
 * `data/NAME/predictionds/ID.txt`: File with one line per supported function, for example reflecting predictions for example. Example:
   ```
   0.001 ZIPWITH
   0.4 *
   0.9 MAP
   ...
   ```

### How Search Works

We maintain a single `IOSet` and a stack of iterators. We initialize
by putting a `SuccessorIterator` on the stack, then using the following
logic:

```
while true:
  if top_iterator is not finished:
     successor = top_iterator.Next()
     apply successor
     check if finished

     if stack.size() < MAX_PROGRAM_LENGTH:
        // move to a longer program
        stack.push(new SuccessorIterator(io))
     else:
       // undo the last operation
       io.Pop()
  else:
     stack.pop()
     io.pop()
```

# Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.microsoft.com.

When you submit a pull request, a CLA-bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., label, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

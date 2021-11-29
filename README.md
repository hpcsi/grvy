![libGRVY](https://raw.githubusercontent.com/hpcsi/grvy/master/doxygen/images/grvy-small.png)
[![Doxygen](https://github.com/hpcsi/grvy/actions/workflows/docs.yaml/badge.svg)](https://github.com/hpcsi/grvy/actions/workflows/docs.yaml)

===================

[![build](https://github.com/hpcsi/grvy/workflows/CI//badge.svg?branch=main)](https://github.com/hpcsi/grvy/actions)


The GRVY Toolkit is a utility library used to house various support
functions often required for application development of
high-performance, scientific applications. The library is written in
C++, but provides an API for development in C and Fortran. The primary
functionality provided by the library is categorized as follows:

* a flexible method for parsing keyword driven input files (with
  backwards-compatibility support)
* a performance timing mechanism to provide a simple summary of
  individual code section performance along with basic statistics
* an HDF5 based historical performance mechanism for logging
  application performance over time on various compute resources
* a simple priority-based logging mechanism to control application
  messages
* utilities for offloading out-of-core read/write operations to a pool
  of distributed shared-memory nodes using MPI for data transfer
  miscellaneous 
* file handling and math utilities

For more detailed API information, please refer to the
[documentation](https://hpcsi.github.io/grvy/index.html).

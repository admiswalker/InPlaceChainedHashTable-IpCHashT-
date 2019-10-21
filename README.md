# InPlaceChainedHashTable-IpCHashT-

This is an implementation plan for **IpCHashT (In-place Chained Hash Table)**.

## Usage
```
$ git clone git@github.com:admiswalker/InPlaceChainedHashTable-IpCHashT-
$ cd ./InPlaceChainedHashTable-IpCHashT-
```
### Compile
```
$ make
```
### Run test
```
$ ./exe_t
```
In a test, assert() will be enabled.
### Run benchmark
```
$ ./exe_bm
```
In a benchmark, assert() will be disabled using -DNDEBUG complie option.

### Run statistical process
```
$ ./exe_sProc
```

## File descriptions

| File or Directory                         | Description                                                                            | Origin |
|-------------------------------------------|----------------------------------------------------------------------------------------|--------|
| bench                                     | Results of the benchmarks                                                              |        |
| tmpBench                                  | Output directory of the benchmarks                                                     |        |
| tmpDir                                    | Temporary directory for graph plotting                                                 |        |
| CHashT.hpp                                | Inplimentation of "sstd::CHashT"                                                       |        |
| FNV\_Hash.cpp                             | Hash function for only implimentation test                                             | [qiita.com/Ushio/items/a19083514d087a57fc72](https://qiita.com/Ushio/items/a19083514d087a57fc72)           |
| FNV\_Hash.hpp                             | Hash function for only implimentation test                                             | [qiita.com/Ushio/items/a19083514d087a57fc72](https://qiita.com/Ushio/items/a19083514d087a57fc72)           |
| IpCHashT.hpp                              | Inplimentation of "sstd::IpCHashT" (Proposing method)                                  |        |
| Makefile                                  | Makefile                                                                               |        |
| README.md                                 | This file                                                                              |        |
| ```SubStandardLibrary-SSTD--master.zip``` | Convenient functions set                                                               | [github.com/admiswalker/SubStandardLibrary](https://github.com/admiswalker/SubStandardLibrary)             |
| bench.hpp                                 | Benchmark                                                                              |        |
| exe\_bench                                | Binary file for benchmark                                                              |        |
| exe\_bench\_uM                            | Binary file for benchmarking allocated memory size called from exe\_bench              |        |
| exe\_sProc                                | Binary file for statistical process of "main\_sProc.cpp"                               |        |
| exe\_test                                 | Binary file for "test_CHashT.hpp" and "test_IpCHashT.hpp" called from "main\_test.cpp" |        |
| ```flat_hash_map-master.zip```            | Inplimentation of "ska::flat\_hash\_map"                                               | [github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map) |
| ```googletest-master.zip```               | Google's C++ test framework                                                            | [github.com/google/googletest](https://github.com/google/googletest) |
| main\_bench.cpp                           | Entry potion for "bench.hpp"                                                           |        |
| main\_bench\_usedMemory.cpp               | Entry point of exe\_bench\_uM for benchmarking allocated memory size                   |        |
| main\_sProc.cpp                           | Entry potion for generating merget graph.                                              |        |
| main\_test.cpp                            | Entry potion for "test\_CHashT.hpp" and "test\_IpCHashT.hpp"                           |        |
| plots.py                                  | Plotting functions for benchmark                                                       |        |
| ```sparsehash-master.zip```               | Inplimentation of "google::dense\_hash\_map"                                           | [github.com/sparsehash/sparsehash](https://github.com/sparsehash/sparsehash) |
| test\_CHashT.hpp                          | Test code for "CHashT.hpp"                                                             |        |
| test\_IpCHashT.hpp                        | Test code for "IpCHashT.hpp"                                                           |        |
| typeDef.h                                 | Type definitions for integer                                                           |        |



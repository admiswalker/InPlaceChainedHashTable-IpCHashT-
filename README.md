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

## File descriptions

| File name                  | Description                                  | Origin |
|----------------------------|----------------------------------------------|--------|
| flat\_hash\_map-master     | Extract file of "flat\_hash\_map-master.zip" |        |
| googletest-master          | Extract file of "googletest-master.zip"      |        |
| sparsehash-master          | Extract file of "sparsehash-master.zip"      |        |
| sstd                       | Extract file of "sstd.zip"                   |        |
| CHashT.hpp                 | Inplimentation of "sstd::CHashT"             |        |
| FNV\_Hash.cpp              | Light claculation weight hash function       | https://qiita.com/Ushio/items/a19083514d087a57fc72 |
| FNV\_Hash.hpp              | Light claculation weight hash function       | https://qiita.com/Ushio/items/a19083514d087a57fc72 |
| IpCHashT.hpp               | Inplimentation of "sstd::IpCHashT" (Proposing method) |        |
| Makefile                   | Makefile                                     |        |
| README.md                  | Read me file                                 |        |
| bench.hpp                  | Benchmark                                    |        |
| bench\_main.cpp            | Entry potion for "bench.hpp"                 |        |
| exe\_bm                    | Binary file for benchmark                    |        |
| exe\_t                     | Binary file for test of "test\_CHashT.hpp" and "test\_IpCHashT.hpp" |        |
| flat\_hash\_map-master.zip | Inplimentation of "ska::flat\_hash\_map"     | https://github.com/skarupke/flat_hash_map |
| googletest-master.zip      | Google's C++ test framework                  | https://github.com/google/googletest |
| plots.py                   | Plotting funcrions for benchmark             |        |
| sparsehash-master.zip      | Inplimentation of "google::dense\_hash\_map" | https://github.com/sparsehash/sparsehash |
| sstd.zip                   | Convenient functions set                     | https://github.com/admiswalker/SubStandardLibrary |
| test\_CHashT.hpp           | Test code for "CHashT.hpp"                   |        |
| test\_IpCHashT.hpp         | Test code for "IpCHashT.hpp"                 |        |
| test\_main.cpp             | Entry potion for "test\_CHashT.hpp" and "test\_IpCHashT.hpp" |        |
| typeDef.h                  | Type definitions for integer |        |



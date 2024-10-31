## RocksDB에서 pread, pwrite함수를 io_uring 구현

------------------------
env/io_posix.cc파일에서 pread, pwrite 함수 구현 후 호출하는 부분을 교체.
코드는 간단하게 gpt한테 받은거로 테스트를 진행함.


큐 길이 32-4096까지 설정해서 db_bench 테스트를 진행한 결과를 요약하면


읽기는 2.3-2.5초 사이.


쓰기는 51-52초 사이.


기존 결과는


읽기 0.23-0.3초 사이.


쓰기는 19-21초 사이.

컴파일 및 빌드는 다음 명령어로 진행함.(해당 build 디렉토리에서 실행)

cmake .. -DWITH_SNAPPY=ON

make -j$(nproc)

테스트 명령어는 다음과 같음.(해당 build 디렉토리에서 실행)

./db_bench --benchmarks=fillrandom --num=1000000 --value_size=1024

./db_bench --benchmarks=readrandom --num=1000000 --value_size=1024












## RocksDB: A Persistent Key-Value Store for Flash and RAM Storage

[![CircleCI Status](https://circleci.com/gh/facebook/rocksdb.svg?style=svg)](https://circleci.com/gh/facebook/rocksdb)

RocksDB is developed and maintained by Facebook Database Engineering Team.
It is built on earlier work on [LevelDB](https://github.com/google/leveldb) by Sanjay Ghemawat (sanjay@google.com)
and Jeff Dean (jeff@google.com)

This code is a library that forms the core building block for a fast
key-value server, especially suited for storing data on flash drives.
It has a Log-Structured-Merge-Database (LSM) design with flexible tradeoffs
between Write-Amplification-Factor (WAF), Read-Amplification-Factor (RAF)
and Space-Amplification-Factor (SAF). It has multi-threaded compactions,
making it especially suitable for storing multiple terabytes of data in a
single database.

Start with example usage here: https://github.com/facebook/rocksdb/tree/main/examples

See the [github wiki](https://github.com/facebook/rocksdb/wiki) for more explanation.

The public interface is in `include/`.  Callers should not include or
rely on the details of any other header files in this package.  Those
internal APIs may be changed without warning.

Questions and discussions are welcome on the [RocksDB Developers Public](https://www.facebook.com/groups/rocksdb.dev/) Facebook group and [email list](https://groups.google.com/g/rocksdb) on Google Groups.

## License

RocksDB is dual-licensed under both the GPLv2 (found in the COPYING file in the root directory) and Apache 2.0 License (found in the LICENSE.Apache file in the root directory).  You may select, at your option, one of the above-listed licenses.

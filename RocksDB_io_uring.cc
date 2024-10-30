#include <liburing.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstring>
#include <assert.h>
#include "rocksdb/db.h"
#include <iostream>

rocksdb::DB* db;
rocksdb::Options options;

int main()
{
	options.create_if_missing = true;
	rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/test_io_RUring", &db);
	assert(status.ok());
	if(status.ok()){//DB Ready
		cout<<"DB Running!"<<endl;
	}

  struct io_uring ring;//io_uring Ready
  io_uring_queue_init(32, &ring, 0);//io_uring 초기화 엔트리 개수는 32개 sq와
									//cq의 크기를 의미한다. 마지막 0은 특별한
									//옵션이 없으면 0으로 설정한다.

  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);//io_uring에서 sqe를
													 //얻는다. 

	if(!sqe){
		return rocksdb::Status::IOError("failed to get io_uring_sqe");
	}

	int wal_fd = open("/tmp/WAL", O_WRONLY | O_APPEND | O_CREAT, 0644);

	if(wal_fd < 0){
		return rocksdb::Status::IOError("Failed to open WAL file");
	}

	io_uring_prep_write(sqe, wal_fd, value.c_str(), value.size(), 0);

	io_uring_submit(&ring);

	struct io_uring_cqe *cqe;



  int fd = open("test_io_RUring.txt", O_WRONLY | O_CREAT);//write하는 파일.
  
  struct iovec iov = {//작업을 submit할 때 사용하는 iovec 구조체이다.
					  //.iov_base는 메모리 버퍼의 주소를 쓰면되고, .iov_len은
					  //길이를 나타낸다.
    .iov_base = (void*)"Hello world",
    .iov_len = strlen("Hello world"),
  };
  io_uring_prep_writev(sqe, fd, &iov, 1, 0);
  io_uring_submit(&ring);

  struct io_uring_cqe *cqe;

  for (;;) {
    io_uring_peek_cqe(&ring, &cqe);
    if (!cqe) {
	  puts("Waiting...");
    } else {
      puts("Finished.");
      break;
    }
  }
  io_uring_cqe_seen(&ring, cqe);
  io_uring_queue_exit(&ring);
  close(fd);
}

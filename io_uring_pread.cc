#include "io_uring_pread.h"
#include <fcntl.h>
#include <cstring>
#include <liburing.h>
#include <stdio.h>

ssize_t io_uring_pread(int __fd, char *__buf, size_t __nbytes, off_t __offset){
	io_uring ring;
	io_uring_queue_init(32, &ring, 0);

	if( __fd < 0 || __buf == NULL || __nbytes == 0){
		errno = EINVAL;
		return -1;
	}

	

	struct io_uring_sqe *sqe = io_uring_get_seq(&ring);
	if(!sqe){
		io_uring_queue_exit(&ring);
		return -1;
	}
	
	struct io_uring_cqe *cqe;
	if(io_uring_wait_cqe(&ring, &cqe) < 0){
		io_uring_queue_exit(&ring);
		return -1;
	}

	if(cqe->res < 0){
		io_uring_queue_exit(&ring);
		return -1;
	}

	ssize_t bytes_read = cqe->res;
	//io_uring

	io_uring_prep_read(sqe, __fd, __buf, __nbytes, __offset);

	io_uring_submit(&ring);

	return bytes_read;
}

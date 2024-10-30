#include <stddef.h>
#include <bits/types.h>
#include <bits/environments.h>
#include <bits/posix_opt.h>
#include <unistd.h>
#include <liburing.h>

//class IoUringPread{
//	public:
//		ssize_t io_uring_pread(int __fd, void *__buf, size_t __nbytes, __off_t __offset);
//}

ssize_t io_uring_pread(int __fd, char *__buf, size_t __nbytes, off_t __offset);


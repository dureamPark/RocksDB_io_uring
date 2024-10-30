#include <liburing.h>
#include <fcntl.h>
#include <unistd.h>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <cstring>
#include <stdio.h>
#include <cassert>

using namespace std;

class testDB{
	public:
		testDB(){
			io_uring_queue_init(32, &ring, 0);
		}

		~testDB(){
			io_uring_queue_exit(&ring);
		}

		rocksdb::Status syncPut(const string& key, const string& value) {
			struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
			if(!sqe) {
				return rocksdb::Status::IOError("failed to get io_uring_sqe_entry");

				int wal_fd = open("/tmp/wal", O_WRONLY | O_APPEND | O_CREAT, 0644);
				if(wal_fd < 0) {
					return rocksdb::Status::IOError("failed to open wal file");
				}

				io_uring_prep_write(sqe, wal_fd, value.c_str(), value.size(),
						0);

				io_uring_submit(&ring);

				struct io_uring_cqe *cqe;
				int ret = io_uring_wait_cqe(&ring, &cqe);
				if(ret < 0) {
					return rocksdb::Status::IOError("failed to wait for io_uring_wait_cque");
				}
				if(cqe->res < 0) {
					return rocksdb::Status::IOError("failed to write to wal");
				}

				memtable[key] = value;

				io_uring_cqe_seen(&ring, cqe);

				return rocksdb::Status::OK();
			}

		private:
			struct io_uring ring;
			unordered_map<string, string> memtable;
		}
};

int main(){
	testDB db;
	struct io_uring ring;

	rocksdb::Status status = db.syncPut("key1", "value");
	assert(status.ok());

	return 0;
}


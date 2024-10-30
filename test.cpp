#include <assert.h>
#include <iostream>
#include "rocksdb/db.h"

int main(){
	rocksdb::DB* db;
	rocksdb::Options options;
	options.create_if_missing = true;

	rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/test1db", &db);
	assert(status.ok());

	std::string key = "test_key";
	std::string value = "test_value";

	status = db->Put(rocksdb::WriteOptions(), key, value);
	assert(status.ok());
	std::cout<< "insert key: " << key << "insert value: " << value << std::endl;

	std::string get_value;
	status = db->Get(rocksdb::ReadOptions(), key, &get_value);
	if(status.ok()){
		std::cout << "get key: " << key << "value: " << get_value << std::endl;
	}
	else{
		std::cout << "error" << std::endl;
	}

	status = db->Delete(rocksdb::WriteOptions(), key);
	assert(status.ok());
	std::cout << "delete key: " << key << std::endl;

	delete db;
	return 0;
}

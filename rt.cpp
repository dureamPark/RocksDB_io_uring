#include <cassert>
#include <string>
#include <rocksdb/db.h>
#include <iostream>

int main(int argc, char** argv) {
	    rocksdb::DB* db;
		    rocksdb::Options options;
			    options.create_if_missing = true;
				    rocksdb::Status status = rocksdb::DB::Open(options,
							"/tmp/testdb", &db);
					    assert(status.ok());

						    // Insert value
						    status = db->Put(rocksdb::WriteOptions(), "Test
									key", "Test value");
							    assert(status.ok());

								    // Read back value
								    std::string value;
									    status =
											db->Get(rocksdb::ReadOptions(),
													"Test key", &value);

										    // Print value using printf
										    printf("Retrieved value: %s\n",
													value.c_str());

											    // Print status using printf
											    if (status.ok()) {
													        printf("Status is
																	OK\n");
															    } else {
																	        printf("Status
																					error:
																					%s\n",
																					%status.ToString().c_str());
																			    }

												    assert(status.ok());
													    assert(!status.IsNotFound());

														    // Read key which
															// does not exist
														    status =
																db->Get(rocksdb::ReadOptions(),
																		"This
																		key
																		does
																		not
																		exist",
																		&value);
															    assert(status.IsNotFound());

																    delete db;
}


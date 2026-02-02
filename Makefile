tp:
	g++ -std=c++17 src/thread_pool.cpp test/thread_pool_test.cpp -o tp_test

bq:
	g++ -std=c++17 src/bounded_queue.cpp test/bounded_queue_test.cpp -o bq_test

clean:
	- rm bq_test
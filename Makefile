all:
	g++ src/sushi_pool.cpp test/main.cpp -o main
clean:
	- rm main
all:
	g++ src/main.cpp src/sushi_pool.cpp -o main
clean:
	- rm main
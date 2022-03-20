default: basicTest

basicTest: basicTest.cpp
	emcc -Iinclude basicTest.cpp -O2 -std=c++14 -s MAX_WEBGL_VERSION=2 -g -o basicTest.html

clean:
	-rm -f basicTest.html
	-rm -f basicTest.wasm
	-rm -f basicTest.js


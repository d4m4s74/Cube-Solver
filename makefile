solver: ./src/solver.c ./src/utils.c ./src/cube.c ./src/cross.c ./src/f2l.c ./src/lastlayer.c
	gcc  -o ./bin/solver ./src/solver.c ./src/utils.c ./src/cube.c ./src/cross.c ./src/f2l.c ./src/lastlayer.c
library: ./src/solver_library.c ./src/utils.c ./src/cube.c ./src/cross.c ./src/f2l.c ./src/lastlayer.c
	gcc -fPIC -shared -o bin/libcubesolver.so ./src/solver_library.c ./src/utils.c ./src/cube.c ./src/cross.c ./src/f2l.c ./src/lastlayer.c
randomtests: ./test/randomtests.c ./src/utils.c ./src/cube.c ./src/cross.c ./src/f2l.c ./src/lastlayer.c ./src/solver_library.c
	gcc -g -o ./test/randomtests ./test/randomtests.c ./src/utils.c ./src/cube.c ./src/cross.c ./src/f2l.c ./src/lastlayer.c ./src/solver_library.c
clean:
	rm -f bin/*
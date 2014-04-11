
gcc -std=c99 -o src/iterate/mem_test.o -I libs/DBWT libs/DBWT/*.c src/utils.c src/bwt/*.c src/array/bit_vector.c src/backward-search/*.c  src/iterate/*.c tests/iterate/check_mem.c tests/utils_for_tests/utils_for_tests.c `pkg-config --cflags --libs check` -lm

./src/iterate/mem_test.o
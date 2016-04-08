CONFIG   =  Config.sh
PYTHON   =  /usr/bin/python
CFLAGS   =
CC       = mpicc
INCL	 = include/allvars.h include/proto.h include/mc_options.h
SRC 	 = src/main.c src/allvars.c src/init.c src/proc.c src/linalg.c src/stats.c src/finalize.c src/ising.c src/metropolis.c

make:
	python make_macros.py
	$(CC) $(CFLAGS) $(SRC) -Wall -lm -O3 -fopenmp -g -o bin/montecarlo

clean:
	rm *~ src/*.o src/*~ include/*.o include/*~ bin/* \#* src/\#* include/\#* bin/\#* \
	python/*~ python/\#* OUTPUTS/*~ OUTPUTS/\#* OUTPUTS/*

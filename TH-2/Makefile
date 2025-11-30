all: demo-application

lib_sch.o: lib_sch.cpp lib_sch.h
	g++ -fPIC -c lib_sch.cpp -o lib_sch.o -pthread

lib_sch.so: lib_sch.o
	g++ -shared -o lib_sch.so lib_sch.o -pthread

demo-application.o: demo-application.cpp lib_sch.h
	g++ -c demo-application.cpp -o demo-application.o -pthread

demo-application: demo-application.o lib_sch.so
	g++ demo-application.o -L. -l_sch -o demo-application -pthread -Wl,-rpath=.

clean:
	rm -f *.o *.so demo-application

.PHONY: all clean


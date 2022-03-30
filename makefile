all: build/test.out

build/test.out: dir build/test.o build/cJSON.o build/cjsonp.o build/cjsonp_cmd_parser.o 
	gcc -o $@  build/test.o build/cJSON.o build/cjsonp.o build/cjsonp_cmd_parser.o

dir: 
	mkdir -p build

build/test.o: test.c
	gcc -o $@ -c $^ -Icjsonp/include -IcJSON

build/cjsonp.o:  cjsonp/cjsonp.c
	gcc -o $@ -c $^ -Icjsonp/include -IcJSON

build/cjsonp_cmd_parser.o: cjsonp/cjsonp_cmd_parser.c
	gcc -o $@ -c $^ -Icjsonp/include

build/cJSON.o: cJSON/cJSON.c
	gcc -o $@ -c $^ -IcJSON

.PHONY: clean run

run:
	./build/test.out

clean:
	rm -rf build
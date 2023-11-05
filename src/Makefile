.PHONY: all tests install uninstall gcov_report dist dvi run linter vg clean_dvi clean clean_dist

all: install tests
	./bin/smartCalc2

install:
	mkdir -p build
	cd build; cmake ..; make;
	mkdir -p bin
	mv build/smartCalc2 bin/
	mv build/model_test bin/

uninstall:
	rm -rf bin/ build/

tests:
	./bin/model_test

gcov_report:
	gcovr -r ./ --object-directory ./build --exclude 'tests/.*' --html --html-details -o build/coverage_report.html
	open build/coverage_report.html

dist:
	mkdir -p "SmartCalc-2.0"
	cp bin/smartCalc2 SmartCalc-2.0/
	tar -zcf SmartCalc-2.0.tar.gz SmartCalc-2.0
	rm -rf SmartCalc-2.0

dvi:
	mkdir -p "dvi"
	doxygen Doxyfile
	cd dvi; make;

run:
	./bin/smartCalc2

linter:
	find ./ -iname "*.h" -o -iname "*.cc" | xargs clang-format --style=google -n

vg:
	valgrind --quiet --gen-suppressions=yes ./bin/model_test

clean_dist:
	rm -f SmartCalc-2.0.tar.gz

clean_dvi:
	rm -rf dvi

clean: uninstall clean_dist clean_dvi
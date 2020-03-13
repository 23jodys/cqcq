UNAME_S := $(shell uname -s)
HEADERS := $(wildcard *.h)
VPATH := src:headers:tests

ifeq ($(UNAME_S),Darwin)
GCOV := xcrun llvm-cov gcov
else
GCOV := llvm-cov gcov
endif

CC   := clang

CFLAGS += $(if $(COVERAGE), -fprofile-arcs -ftest-coverage )
CFLAGS += $(if $(DEBUG), -DDEBUG=1 )
CFLAGS += -Werror -Iheaders -g

LDLIBS += $(if $(or $(COVERAGE),$(DEBUG)), -g )
LDLIBS += $(if $(COVERAGE), --coverage )

test_cqcq: LDLIBS += -lcmocka
test_cqcq: cqcq.o test_cqcq.o

test_list: LDLIBS += -lcmocka
test_list: list.o test_list.o

.PHONY: test
test: test_cqcq test_list
	./test_cqcq

list.o: list.h

cqcq.o: cqcq.h

valgrind_%: %
	valgrind --leak-check=full --suppressions=valgrind_suppressions.supp --error-exitcode=1 ./$*

coverage: COVERAGE=1
coverage: test
	$(GCOV) $(SRCS)

TAGS: $(SRCS) cqcq.h test_*.[ch]
	etags $^

docs: $(HEADERS)
	doxygen

.PHONY: clean
clean:
	rm -rf *.o *.gcda *.gcno test_cqcq *.dSYM html/ latex/

include ../Config
LIBS = -L../lib -lsz

TARGETS = ../bin/sz
		

all: $(TARGETS)

$(TARGETS): ../lib/libsz.a
	mkdir -p ../bin
	$(CXX) $(CFLAGS) sz.c -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
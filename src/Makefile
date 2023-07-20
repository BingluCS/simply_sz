include ../Config

OBJECTS = rw.o sz.o sz_float.o dataCompression.o ByteToolkit.o DynamicByteArray.o DynamicIntArray.o CompressElement.o \
 TightDataPointStorageF.o conf.o TypeManager.o
TARGETS = $(LIBDIR)/libsz.a 
		

all: $(TARGETS)

$(LIBDIR)/libsz.a : $(OBJECTS)
	mkdir -p $(LIBDIR)
	rm -f $@
	ar rc $@ $^

.c.o: # %.o: %.c
	$(CXX) $(CFLAGS) -c $< 

clean:
	rm  -f $(TARGETS) $(OBJECTS)
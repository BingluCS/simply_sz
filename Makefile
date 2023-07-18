include Config
all:
	$(MAKE) clean
	@cd src; $(MAKE) all
	@cd utils; $(MAKE) all

clean:
	@cd src; $(MAKE) clean
	@cd utils; $(MAKE) clean
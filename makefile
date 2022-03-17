SUBDIRS= chip8

.PHONY: clean all chip8

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

chip8:
	$(MAKE) -C chip8

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done



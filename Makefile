.PHONY: clean build test run

build:
	$(MAKE) -C src/BRSRC13 build
	$(MAKE) -C src/DETHRACE build

clean:
	$(MAKE) -C src/BRSRC13 clean
	$(MAKE) -C src/DETHRACE clean

run: build
	$(src/DETHRACE/build/c1)

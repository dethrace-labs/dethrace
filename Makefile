.PHONY: clean build test run

clean:
	$(MAKE) -C src/BRSRC13 clean
	$(MAKE) -C src/DETHRACE clean

build:
	$(MAKE) -C src/BRSRC13 build
	$(MAKE) -C src/DETHRACE build

run: build
	$(src/DETHRACE/build/c1)

.PHONY: build clean test run

all: build test

build:
	@echo "Building brender"
	@$(MAKE) -C src/BRSRC13 build
	@echo "Building dethrace"
	@$(MAKE) -C src/DETHRACE build
	@echo "Building tests"
	@$(MAKE) -C test build

clean:
	@$(MAKE) -C src/BRSRC13 clean
	@$(MAKE) -C src/DETHRACE clean
	@$(MAKE) -C test clean

test: build
	@test/build/c1tests

run: build
	src/DETHRACE/build/c1

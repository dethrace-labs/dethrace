.PHONY: build clean test run

all: build test

build:
	@echo "Building fw"
	@$(MAKE) -C src/framework build
	@echo "Building brender"
	@$(MAKE) -C src/BRSRC13 build
	@echo "Building dethrace"
	@$(MAKE) -C src/DETHRACE build

clean:
	@$(MAKE) -C src/framework clean
	@$(MAKE) -C src/BRSRC13 clean
	@$(MAKE) -C src/DETHRACE clean
	@$(MAKE) -C test clean

test: build
	@echo "Building tests"
	@$(MAKE) -C test build
	@cp -r test/assets/DATA test/build
	@(cd test/build && ./c1tests)

run: build
	@echo "Running dethrace"
	@./run.sh

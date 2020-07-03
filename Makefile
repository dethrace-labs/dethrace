.PHONY: build clean test install-deps run

all: build test

build:
	@echo "Building harness"
	@$(MAKE) -C src/harness build
	@echo "Building brender"
	@$(MAKE) -C src/BRSRC13 build
	@echo "Building dethrace"
	@$(MAKE) -C src/DETHRACE build

clean:
	@$(MAKE) -C src/harness clean
	@$(MAKE) -C src/BRSRC13 clean
	@$(MAKE) -C src/DETHRACE clean
	@$(MAKE) -C test clean

test: build
	@echo "Building tests"
	@$(MAKE) -C test build
	@cp -r test/assets/DATA test/build
	@(cd test/build && ./c1tests $$DR_TEST_ARGS)

install-deps:
	apt-get install libsdl2-dev


run: build
	@echo "Running dethrace"
	@./run.sh

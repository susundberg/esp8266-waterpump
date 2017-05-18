SHELL:=/bin/bash


build:
	( \
	source ../ENV//bin/activate; \
	platformio run; \
	)


upload:
	( \
	source ../ENV//bin/activate; \
	platformio run --verbose --target upload; \
	)


test_build:
	scons -Y tests/tools/

test_run:
	./tests/tools/run_all.sh

test_coverage:
	./tests/tools/run_coverage.sh



# For now, only part_1.

.PHONY: all part_1 run_part1 clean

all: part_1

part_1:
	$(MAKE) -C part_1

run_part1:
	$(MAKE) -C part_1 run

clean:
	$(MAKE) -C part_1 clean || true

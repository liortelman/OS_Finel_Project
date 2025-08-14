# Root Makefile – parts dispatcher
.PHONY: all part_1 run_part1 part_2 run_part2 clean

all: part_1

part_1:
	$(MAKE) -C part_1

run_part1:
	$(MAKE) -C part_1 run

part_2:
	$(MAKE) -C part_2

run_part2:
	$(MAKE) -C part_2 run

clean:
	-$(MAKE) -C part_1 clean
	-$(MAKE) -C part_2 clean

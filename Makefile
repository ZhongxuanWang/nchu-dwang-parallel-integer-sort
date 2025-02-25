n ?= 1000000
path ?= sort/sort_numbers_serial
# usage: make n=[whatever] path=[path to ur sorting executable]
serial: sort/sort_numbers_serial
	python3 check.py $(n) $(path)

sort/sort_numbers: sort/sort_numbers_serial.c
	gcc -o sort/sort_numbers sort/sort_numbers_serial.c

.PHONY: clean

clean:
	rm check/check_numbers **/*.class sort/sort_numbers_serial **/*.cmo **/*.cmi generate/NumberGenerator.class

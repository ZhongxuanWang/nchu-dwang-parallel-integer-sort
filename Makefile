n ?= 1000000
path ?= sort/sort_numbers

.PHONY: clean

serial: sort/sort_numbers_serial
	python3 check.py $(n) $(path)

main: sort/sort_numbers
	python3 check.py $(n) $(path)

sort/sort_numbers: sort/sort_numbers.c
	gcc -o sort/sort_numbers sort/sort_numbers.c

sort/sort_numbers_serial: sort/sort_numbers_serial.c
	gcc -o sort/sort_numbers_serial sort/sort_numbers_serial.c

clean:
	rm -f check/check_numbers **/*.class sort/sort_numbers* **/*.cmo **/*.cmi generate/NumberGenerator.class
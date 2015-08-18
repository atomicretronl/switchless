CC=xc8
SRC=switchless.c
CHIP=16F630

all: saturn megadrive

megadrive: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_MEGA_DRIVE -o$@ $(SRC)

saturn: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_SATURN -o$@ $(SRC)

clean:
	rm -f *.as *.cmf *.cof *.d *.hex *.hxl *.lst *.obj *.p1 *.pre *.rlf *.sdb *.sym

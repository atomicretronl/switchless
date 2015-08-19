CC=xc8
SRC=switchless.c
CHIP=16F630

all: saturn megadrive snes_ntsc snes_pal test

megadrive: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_MEGA_DRIVE -o$@ $(SRC)

saturn: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_SATURN -o$@ $(SRC)

snes_ntsc: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_SNES_NTSC -o$@ $(SRC)

snes_pal: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_SNES_PAL -o$@ $(SRC)

test: $(SRC)
	$(CC) --chip=$(CHIP) -DCONSOLE_TEST -o$@ $(SRC)

clean:
	rm -f *.as *.cmf *.cof *.d *.hex *.hxl *.lst *.obj *.p1 *.pre *.rlf *.sdb *.sym

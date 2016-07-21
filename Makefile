CC= C:\SysGCC\Raspberry\bin\arm-linux-gnueabihf-gcc.exe
SRC = GShell.c platforms/raspberrypi/*.c


all: clean
	mkdir ./build
	$(CC) $(SRC) -o build/GShell
clean:
	rm -rf build

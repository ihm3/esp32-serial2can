
pio:= Platformio.exe

all:
	${pio} run

upload:
	${pio} run -t upload

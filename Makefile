blink.o: blink.c
	msp430-gcc -mmcu=msp430g2553 blink.c -o blink.o

micro.o: micro.c
	msp430-gcc -mmcu=msp430g2553 micro.c -o micro.o

blink5.o: blink5.c
	msp430-gcc -mmcu=msp430g2553 blink5.c -o blink5.o

sw.o: sw.c
	msp430-gcc -mmcu=msp430g2553 sw.c -o sw.o

siren.o: siren.c
	msp430-gcc -mmcu=msp430g2553 siren.c -o siren.o

sw: sw.o
	mspdebug rf2500 "prog sw.o"

siren: siren.o
	mspdebug rf2500 "prog siren.o"

micro: micro.o
	mspdebug rf2500 "prog micro.o"

blink5: blink5.o
	mspdebug rf2500 "prog blink5.o"

uart.o: uart.c
	msp430-gcc -mmcu=msp430g2553 uart.c -o uart.o

uart: uart.o
	mspdebug rf2500 "prog uart.o"

%:
	msp430-gcc -mmcu=msp430g2553 $@.c -o $@.o
	mspdebug rf2500 "prog $@.o"

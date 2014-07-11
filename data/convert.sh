cat data.txt | python2 -c "import sys; print '\n'.join([str(int('0x' + x.strip(), 16)) for x in sys.stdin.readlines()])" > dataDec.txt

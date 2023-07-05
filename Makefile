all:
	g++ -O2 -pipe broff.cpp -o broff

install:
	cp broff /usr/local/bin/broff

uninstall:
	rm /usr/local/bin/broff

clean:
	rm broff

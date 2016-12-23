all: httpd client test_cgi
LIBS = -lpthread #-lsocket

httpd: httpd.c
	gcc -g -W -Wall $(LIBS) -o $@ $<

client: simpleclient.c
	gcc -W -Wall -o $@ $<
	
test_cgi: test_cgi.c
	gcc -W -Wall -o $@ $<
	
clean:
	rm httpd client

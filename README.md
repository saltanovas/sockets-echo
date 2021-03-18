# Unix Sockets

Sockets are communication points on the same or different computers to exchange data. Although sockets can operate on many operating systems, **the program supports Unix only**. 

## Compilation
Either `Makefile` or the following command can be used:
```
gcc client.c server.c helpers.c main.c -o Program -lm -lpthread
```

## Execution
Please use `-s` prefix to run a server. Use `-c` prefix to run a client. A port number after the prefix is optional:
```
./program [{-s | -c} [port_number]]
```
Take a note that a client won't be able to open a connection if the server is not running.

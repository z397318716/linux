all: tcp_cli thread_srv
tcp_cli:tcp_cli.cpp
	g++  $^ -o $@
thread_srv:thread_srv.cpp
	g++ $^ -o $@ -lpthread

.PHONY:all
all1: udp_cli udp_srv
udp_cli:udp_cli.cpp
	g++  $^ -o $@
udp_srv:udp_srv.c
	gcc $^ -o $@

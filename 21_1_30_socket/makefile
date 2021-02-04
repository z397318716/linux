.PHONY:all
all: tcp_cli process_srv
tcp_cli:tcp_cli.cpp
	g++  $^ -o $@
process_srv:process_srv.cpp
	g++ $^ -o $@

all1: udp_cli udp_srv
udp_cli:udp_cli.cpp
	g++  $^ -o $@
udp_srv:udp_srv.c
	gcc $^ -o $@

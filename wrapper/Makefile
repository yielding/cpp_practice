all: 
	g++ -std=c++11 EchoServ.cpp SocketAddress.cpp SocketUtil.cpp StringUtils.cpp TCPSocket.cpp -o echo_serv
	g++ -std=c++11 EchoClnt.cpp SocketAddress.cpp SocketUtil.cpp StringUtils.cpp TCPSocket.cpp -o echo_clnt 
	g++ -std=c++11 EchoSelectServ.cpp SocketAddress.cpp SocketUtil.cpp StringUtils.cpp TCPSocket.cpp -o echo_select_serv 

clean:
	rm echo_serv
	rm echo_clnt
	rm echo_select_serv
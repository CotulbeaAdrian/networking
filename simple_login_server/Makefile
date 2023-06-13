CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -lmysqlcppconn

all: server

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp $(LDFLAGS)

clean:
	rm -f server
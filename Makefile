build:
	@g++ -o ./bin/auto-addon \
	    ./src/main.cpp \
	    ./src/config/config.cpp \
	    ./src/config/parser.cpp \
	    ./src/config/item.cpp \
	    ./src/config/handler.cpp \
	    ./src/io/io.cpp \
		-lstdc++fs

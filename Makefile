LDFLAGS  += $(shell pkg-config --libs menuw ncursesw)
CXXFLAGS += -std=c++11 -ggdb -Wall -Wextra -Werror $(shell pkg-config --cflags menuw ncursesw)

interface_test: interface.o test.o window.o ncurses.o error.o
	${CXX} ${CPPFLAGS} ${LDFLAGS} $^ -o $@

LDFLAGS  += `pkg-config --libs menuw ncursesw`
CXXFLAGS += -std=c++11 -ggdb `pkg-config --cflags menuw ncursesw`

interface_test: interface.o test.o
	${CXX} ${CPPFLAGS} ${LDFLAGS} $^ -o $@

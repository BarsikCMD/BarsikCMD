CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS  = -lcurl -lreadline -lncursesw
TARGET   = barsikcmd
SRCS     = main.cpp shell.cpp commands.cpp input.cpp \
           commands/meow.cpp commands/update.cpp commands/file.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
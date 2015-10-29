LDFLAGS=-lGL -lGLU -lglut
CXXFLAGS =

PROG = hw5
HDRS = 
SRCS = $(PROG).cc

OBJS = $(SRCS:.cc=.o)

all: $(PROG)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

$(PROG).o: $(PROG).cc

clean:
	-rm -f $(PROG) $(OBJS)


LDFLAGS=-lGL -lGLU -lglut
#CXXFLAGS = -m32

PROG = hw4
HDRS = 
SRCS = $(PROG).cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

$(PROG).o: $(PROG).cc

clean:
	-rm -f $(PROG) $(OBJS)


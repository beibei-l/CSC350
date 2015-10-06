LDFLAGS=-lGL -lGLU -lglut
#LDFLAGS=-lGL -lGLU -lglut -m32
#CXXFLAGS = -m32

PROG = demo3
HDRS = 
SRCS = $(PROG).cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

$(PROG).o: $(PROG).cc

clean:
	-rm -f $(PROG) $(OBJS)


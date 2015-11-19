LDFLAGS=-lGL -lGLU -lglut -lGLEW
CXXFLAGS =

PROG = hw7
HDRS = 
SRCS = $(PROG).cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

$(PROG).o: $(PROG).cc

clean:
	-rm -f $(PROG) $(OBJS)

colorCube.o: colorCube.cc shaderUtils.h

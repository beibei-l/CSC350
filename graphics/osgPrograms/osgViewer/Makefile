LDFLAGS=-losg -losgDB -losgViewer -lglut

PROG = osgviewerGLUT
HDRS = 
SRCS = osgviewerGLUT.cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

osgviewerGLUT.o: osgviewerGLUT.cc

clean:
	-rm -f $(PROG) $(OBJS)


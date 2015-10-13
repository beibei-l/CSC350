LDFLAGS=-losg -losgDB -losgViewer -lglut

PROG = assembleAndView
HDRS = 
SRCS = assembleAndView.cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

clean:
	-rm -f $(PROG) $(OBJS)


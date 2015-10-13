LDFLAGS=-losg -losgDB

PROG = assembleRobot
HDRS = 
SRCS = assembleRobot.cc

OBJS = $(SRCS:.cc=.o)

$(PROG):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

assembleRobot.o: assembleRobot.cc

clean:
	-rm -f $(PROG) $(OBJS)


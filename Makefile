# Makefile for HW6, ecs36b, f2021
#

CC = g++ -std=c++14
CFLAGS = -g -I/usr/include/jsoncpp

# CFLAGS = -g -D_ECS35B_DEBUG_ -I/usr/include/jsoncpp
# CFLAGS = -g -Wall -Wstrict-prototypes
# CFLAGS = -O3

CORE_INCS =	Core.h Person.h Post.h Comment.h Reaction.h	\
		Action.h Link.h Message.h JvTime.h Tag.h
CORE_OBJS =	Core.o Person.o Post.o Comment.o Reaction.o	\
	 	Action.o Link.o JvTime.o Tag.o

LDFLAGS =	-ljsoncpp

# rules.
all: 	hw6parse hw6produce hw6merge

#
#

Core.o:			Core.cpp Core.h
	$(CC) -c $(CFLAGS) Core.cpp

Tag.o:			Tag.cpp Tag.h
	$(CC) -c $(CFLAGS) Tag.cpp

Person.o:		Person.cpp Person.h Core.h
	$(CC) -c $(CFLAGS) Person.cpp

Post.o:			Post.cpp Post.h
	$(CC) -c $(CFLAGS) Post.cpp

Comment.o:		Comment.cpp Comment.h
	$(CC) -c $(CFLAGS) Comment.cpp

Reaction.o:		Reaction.cpp Reaction.h
	$(CC) -c $(CFLAGS) Reaction.cpp

JvTime.o:		JvTime.cpp JvTime.h
	$(CC) -c $(CFLAGS) JvTime.cpp

Link.o:			Link.cpp Link.h
	$(CC) -c $(CFLAGS) Link.cpp

Action.o:		Action.cpp Action.h
	$(CC) -c $(CFLAGS) Action.cpp

hw6parse.o:		hw6parse.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw6parse.cpp

hw6produce.o:		hw6produce.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw6produce.cpp

hw6merge.o:		hw6merge.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw6merge.cpp

hw6parse:		$(CORE_OBJS) hw6parse.o
	$(CC) -o hw5parse $(CORE_OBJS) hw6parse.o $(LDFLAGS)

hw6produce:		$(CORE_OBJS) hw6produce.o
	$(CC) -o hw6produce $(CORE_OBJS) hw6produce.o $(LDFLAGS)

hw6merge:		$(CORE_OBJS) hw6merge.o
	$(CC) -o hw5merge $(CORE_OBJS) hw6merge.o $(LDFLAGS)

clean:
	rm -f *.o *~ core hw6parse hw6produce hw6merge

target = Scheduler

Scheduler :	Participant.o Subject.o Driver.o Period.o DepGraph.o Global.o \
		Scanner.o Timer.o Schedule.o Parser.o Scheduler.o
	g++ Participant.o Subject.o DepGraph.o Period.o Global.o Scanner.o\
		Driver.o Timer.o Parser.o Schedule.o Scheduler.o -o  Scheduler -lfl;

loc:
	cat DepGraph.h Global.h Participant.h Period.h Schedule.h Subject.h \
	Timer.h DepGraph.cc Global.cc Period.cc Subject.cc test_scan.cc\
	Driver.cc Participant.cc Schedule.cc Timer.cc Scanner.flex \
	Parser.cc Scheduler.cc Scheduler.h > loc

Driver.o :	Driver.cc Participant.h Subject.h DepGraph.h Global.h Schedule.h Scheduler.h
	g++ -c Driver.cc;

Participant.o :	Participant.cc Participant.h Period.h
	g++ -c Participant.cc;

Subject.o :	Subject.cc Subject.h Period.h
	g++ -c Subject.cc;

Period.o :	Period.h Period.cc
	g++ -c Period.cc;

DepGraph.o : DepGraph.cc DepGraph.h Participant.h Subject.h Global.h
	g++ -c DepGraph.cc;

Global.o : Global.h Global.cc Participant.h Subject.h
	g++ -c Global.cc;

Scanner.o : Scanner.flex Global.h
	flex Scanner.flex; \
	mv lex.yy.c Scanner.c; \
	g++ -c Scanner.c;

Timer.o : Timer.cc Timer.h Period.h
	g++ -c Timer.cc;

Schedule.o :	Schedule.cc Schedule.h Global.h DepGraph.h
	g++ -c Schedule.cc

Scheduler.o :	Participant.h Subject.h DepGraph.h Global.h Schedule.h \
	Scheduler.h
	g++ -c Scheduler.cc

Parser.o :	Parser.cc
	g++ -c Parser.cc;

clean :
	rm *.o Scheduler Scanner.c

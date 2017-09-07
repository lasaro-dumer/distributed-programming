LIBS = -lpthread
all:			revstr_stream_clnt revstr_stream_svc

revstr_stream_clnt:	revstr_stream_clnt.c revstr_stream.h
			@gcc -o revstr_stream_clnt revstr_stream_clnt.c $(LIBS)

revstr_stream_svc:	revstr_stream_svc.c revstr_stream.h
			@gcc -o revstr_stream_svc revstr_stream_svc.c $(LIBS)

clean:
			@rm -f revstr_stream_clnt revstr_stream_svc *~ *.gch

info:
			@echo "(c) Lasaro Dumer (29 ago. 2017)"

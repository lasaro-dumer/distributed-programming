all:			fat_stream_clnt fat_stream_svc

fat_stream_clnt:	fat_stream_clnt.c fat_stream.h
			@gcc -o fat_stream_clnt fat_stream_clnt.c

fat_stream_svc:	fat_stream_svc.c fat_stream.h
			@gcc -o fat_stream_svc fat_stream_svc.c

clean:
			@rm -f fat_stream_clnt fat_stream_svc *~

info:
			@echo "(c) Roland Teodorowitsch (23 ago. 2013)"


all: compile_show compile_copy compile_append compile_count compile_delete compile_info compile_list clean

compile_show: show.o functions.o
	gcc -Wall show.o functions.o  -o bin/show

compile_copy: copy.o functions.o
	gcc -Wall functions.o copy.o  -o bin/copy

compile_append: append.o functions.o
	gcc -Wall functions.o append.o  -o bin/append

compile_count: count.o functions.o
	gcc -Wall functions.o count.o  -o bin/count
compile_delete: delete.o functions.o
	gcc -Wall functions.o delete.o  -o bin/delete

compile_info: info.o functions.o
	gcc -Wall functions.o info.o  -o bin/info

compile_list: list.o functions.o
	gcc -Wall functions.o list.o  -o bin/list

funcoes.o: functions.c functions.h
	gcc -Wall -c functions.c functions.h

mostra.o: show.c
	gcc -Wall -c show.c

copia.o: copy.c
	gcc -Wall -c copy.c

acrescenta.o: append.c
	gcc -Wall -c append.c

conta.o: count.c
	gcc -Wall -c count.c

apaga.o: delete.c
	gcc -Wall -c delete.c

informa.o: info.c
	gcc -Wall -c info.c

lista.o: list.c
	gcc -Wall -c list.c

clean:
	rm *.o

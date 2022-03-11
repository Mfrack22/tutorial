all:
	gcc -pthread question1.c -o question1 -g
	gcc -pthread question2.c -o question2 -g
	gcc -pthread question3.c -o question3 -g
	gcc -pthread question4.c -o question4 -g
	gcc -pthread question5.c -o question5 -g

clean:
	rm -f question1 question2 question3 question4 question5 bellcurve.txt || true

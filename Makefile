all:
	g++ -Wall -pedantic -std=c++11 projekt.cpp -o projekt -lsimlib -lm
run:
	./projekt -t 1000 -z 5
# Cíl pro vyčištění projektu (smazání vytvořených souborů)
clean:
	rm -f *.o projekt

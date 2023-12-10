all:
	g++ -Wall -pedantic -std=c++11 projekt.cpp -o projekt -lsimlib -lm
run:
	./projekt
# Cíl pro vyčištění projektu (smazání vytvořených souborů)
clean:
	rm -f *.o projekt

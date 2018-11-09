
CC = gcc -O3 -std=c99 -lm

all: bin/AODjustify #AODrapport-modele.pdf

doc: src/AODjustify.c  # A COMPLETER
	doxygen src/AODjustify.c

eval: bin/AODjustify
	cd ..; source /matieres/4MMAOD6/install/AODjustify-calcul-note.bash # 2&> /tmp/foo

bin/AODjustify: src/AODjustify.c  # A COMPLETER
	$(CC) -o bin/AODjustify src/AODjustify.c
	# chmod 0111 bin/AODjustify

AODrapport-modele.pdf: rapport/AODrapport-modele.tex
	cd rapport; pdflatex AODrapport-modele; pdflatex AODrapport-modele
	mv rapport/AODrapport-modele.pdf .

clean:
	rm bin/AODjustify  rapport/AODrapport-modele.aux rapport/AODrapport-modele.log AODrapport-modele.pdf
	rm -R TESTS

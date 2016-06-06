##
## @file Makefile
## Betriebssysteme myFind Makefile
## Beispiel 3
##
## @author Benjamin Petterle <ic15b077@technikum-wien.at>
## @author Matthias Greil <ic15b006@technikum-wien.at>
## @author Oguz Kaya <ic15b055@technikum-wien.at>
## @date 2016/05/9
##
## @version $Revision: 1 $
##
##
## Last Modified: $Author: Benjamin Petterle $
##

##
## ------------------------------------------------------------- variables --
##

CC=gcc52
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3 -std=gnu11
CP=cp
CD=cd
MV=mv
GREP=grep
DOXYGEN=doxygen

EMPFAENGER=empfaenger.o
SENDER=sender.o

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $< #Kompiliert linkt aber noch nicht Output: *.o file

##
## --------------------------------------------------------------- targets --
##

all: empfaenger sender

empfaenger: $(EMPFAENGER)
	$(CC) $(CFLAGS) -o $@ $^ -lsem182

sender: $(SENDER)
	$(CC) $(CFLAGS) -o $@ $^ -lsem182

clean:
	$(RM) *.o *~ sender empfaenger

distclean: clean
	$(RM) -r doc

doc: html pdf

html:
	$(DOXYGEN) doxygen.dcf

pdf: html
	$(CD) doc/pdf && \
	$(MV) refman.tex refman_save.tex && \
	$(GREP) -v $(EXCLUDE_PATTERN) refman_save.tex > refman.tex && \
	$(RM) refman_save.tex && \
	make && \
	$(MV) refman.pdf refman.save && \
	$(RM) *.pdf *.html *.tex *.aux *.sty *.log *.eps *.out *.ind *.idx \
	      *.ilg *.toc *.tps Makefile && \
	$(MV) refman.save refman.pdf

##
## ---------------------------------------------------------- dependencies --
##

##
## =================================================================== eof ==
##

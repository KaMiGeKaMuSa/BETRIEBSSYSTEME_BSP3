##
## @file Makefile
## sharedmemory
## Beispiel 3
##
## @author Karin Kalman <karin.kalman@technikum-wien.at>
## @author Michael Mueller <michael.mueller@technikum-wien.at>
## @author Gerhard Sabeditsch <gerhard.sabeditsch@technikum-wien.at>
## @date 2016/05/28
##
## @version $Revision: 2.1 $
##
##
## Last Modified: $Author: Gerhard $
##

##
## ------------------------------------------------------------- variables --
##

CC=gcc
CFLAGS=-DDEBUG -Wall -Werror -Wextra -Wstrict-prototypes -pedantic -fno-common -g -O3
CD=cd
CP=cp
MV=mv
GREP=grep
DOXYGEN=doxygen

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

all: sender empfaenger

sender: sender.o sharedfunctions.o
	$(CC) $(OPTFLAGS) sender.o sharedfunctions.o -lsem182 -o sender

empfaenger: empfaenger.o sharedfunctions.o
	$(CC) $(OPTFLAGS) empfaenger.o sharedfunctions.o -lsem182 -o empfaenger

clean:
	$(RM) *.o *~  sender empfaenger

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

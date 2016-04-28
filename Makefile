## @author Karin Kalman
## @author Michael Mueller
## @author Gerhard Sabeditsch
##
## ------------------------------------------------------------- variables --
##

CC=gcc
##CFLAGS=-DDEBUG -Wall -Wextra -Wstrict-prototypes -fno-common -g -O3 -std=gnu11

OBJECTS=sender empfaenger

EXCLUDE_PATTERN=footrulewidth

CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3 -std=gnu11

RM=rm
CP=cp
CD=cd
MV=mv
GREP=grep
DOXYGEN=doxygen


##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

.PHONY: all
all: sender empfaenger

mypopen: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
##	$(RM) *.o
 
.PHONY: clean
clean:
	$(RM) *.o


.PHONY: distclean
distclean: clean
	$(RM) -rf doc

doc: html pdf

.PHONY: html
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

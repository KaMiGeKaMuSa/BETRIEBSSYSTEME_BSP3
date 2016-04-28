#*******************************************************************************
#                                                                              *
#     Author:      G. Leber                                                    *
#                  Technische Universitaet Wien                                *
#                  Institut fuer Technische Informatik E182/1                  *
#                  Treitlstrasse 3                                             *
#                  1040 Wien                                                   *
#                  Tel.: (0222) 58801 / 8176                                   *
#                  E-Mail: guenther@vmars.tuwien.ac.at                         *
#     File:        Makefile						       *
#     Version:     1.9							       *
#     Date:        6/27/94						       *
#                                                                              *
#******************************************************************************/
# Makefile ported by bernd into the 21st century. and CFLAGS contains the ones
# which must by used by students

LIB	=	/usr/local/lib
INCLUDE	=	/usr/local/include
MAN3	=	/usr/local/man/man3
CIS	=	../../CIS/lu

OBJS	=	sem182.o msem182.o

CC	=	gcc52
CFLAGS	=	-Wall -Werror -Wextra -Wstrict-prototypes -pedantic -fno-common -std=gnu11 -O3
CPPFLAGS=	-I.

INSTALL	=	install

all: libsem182.a

libsem182.a: $(OBJS) Makefile
	ar cru libsem182.a $?
	ranlib libsem182.a

.PHONY: clean
clean:
	-rm libsem182.a $(OBJS)

.PHONY: install
install: $(INCLUDE)/sem182.h  $(INCLUDE)/msem182.h $(LIB)/libsem182.a \
	$(MAN3)/sem182.3 $(MAN3)/msem182.3 Makefile

.PHONY: cis
cis: $(CIS)/sem182.tar.gz Makefile

$(CIS)/sem182.tar.gz: Makefile
	tar -czf "$@" -- *.[ch3] Makefile doxygen.dcf

$(INCLUDE)/sem182.h: sem182.h Makefile
	$(INSTALL) -c -m 644 sem182.h $(INCLUDE)

$(INCLUDE)/msem182.h: msem182.h Makefile
	$(INSTALL) -c -m 644 msem182.h $(INCLUDE)

$(LIB)/libsem182.a: libsem182.a Makefile
	$(INSTALL) -c -m 644 libsem182.a $(LIB)

$(MAN3)/sem182.3: sem182.3 Makefile
	$(INSTALL) -c -m 644 sem182.3 $(MAN3)
	-rm -f $(MAN3)/P.3
	-rm -f $(MAN3)/V.3
	-rm -f $(MAN3)/seminit.3
	-rm -f $(MAN3)/semgrab.3
	-rm -f $(MAN3)/semrm.3
	ln $(MAN3)/sem182.3 $(MAN3)/P.3
	ln $(MAN3)/sem182.3 $(MAN3)/V.3
	ln $(MAN3)/sem182.3 $(MAN3)/seminit.3
	ln $(MAN3)/sem182.3 $(MAN3)/semgrab.3
	ln $(MAN3)/sem182.3 $(MAN3)/semrm.3

$(MAN3)/msem182.3: msem182.3 Makefile
	$(INSTALL) -c -m 644 msem182.3 $(MAN3)
	-rm -f $(MAN3)/mP.3
	-rm -f $(MAN3)/mV.3
	-rm -f $(MAN3)/mseminit.3
	-rm -f $(MAN3)/msemgrab.3
	-rm -f $(MAN3)/msemrm.3
	ln $(MAN3)/msem182.3 $(MAN3)/mP.3
	ln $(MAN3)/msem182.3 $(MAN3)/mV.3
	ln $(MAN3)/msem182.3 $(MAN3)/mseminit.3
	ln $(MAN3)/msem182.3 $(MAN3)/msemgrab.3
	ln $(MAN3)/msem182.3 $(MAN3)/msemrm.3

.PHONY: uninstall
uninstall: Makefile
	rm -f $(INCLUDE)/sem182.h  $(INCLUDE)/msem182.h $(LIB)/libsem182.a \
		$(MAN3)/sem182.3 $(MAN3)/P.3 $(MAN3)/V.3 $(MAN3)/seminit.3 \
		$(MAN3)/semgrab.3 $(MAN3)/semrm.3 $(MAN3)/msem182.3 \
		$(MAN3)/mP.3 $(MAN3)/mV.3 $(MAN3)/mseminit.3 \
		$(MAN3)/msemgrab.3 $(MAN3)/msemrm.3

DEPDIR  = Deps
OBJDIR  = Objs
TARGET  = resources resweb

OBJ =	main.o \
	resinfo.o \
	conffile.o \
	resfunctions.o \
	rrdtool.o \
	args.o \
	log.o \
	mainfunc.o

WOBJ =	resweb.o \
	resinfo.o \
	conffile.o \
	log.o

CC     = gcc
FLAGS  = -Wall -g
CFLAGS = $(FLAGS) -c
VERSION=$(shell git tag|tail -n1)

# GCC flags for dependencies auto generation
DEPOPTS = -MP -MD -MF ${DEPDIR}/$(notdir $@).d

all: ${TARGET}

resources: $(addprefix ${OBJDIR}/, ${OBJ})
	$(CC) ${FLAGS} $^ -o $@

resweb: $(addprefix ${OBJDIR}/, ${WOBJ})
	$(CC) ${FLAGS} $^ -o $@

cscope:
	$(eval FILES := $(shell sed -n '/:$$/s/:$$//p' ${DEPDIR}/* | sort -u) \
		${OBJ:.o=.c} ${WOBJ:.o=.c})
	cscope -k -b -q ${FILES}

ctags:
	$(eval FILES := $(shell sed -n '/:$$/s/:$$//p' ${DEPDIR}/* | sort -u) \
		${OBJ:.o=.c} ${WOBJ:.o=.c})
	ctags ${FILES}

# Implicit rules with pattern rules
# On the first go, without dependencies in ./${DEPDIR}, this implicit rule will apply
# and dependency file will be generated.
${OBJDIR}/%.o: %.c makefile | ${DEPDIR} ${OBJDIR}
	$(CC) $< ${DEPOPTS} -DVERSION=\"$(VERSION)\" ${CFLAGS} -c -o $@

# Generate directory if doesn't exists
${OBJDIR} ${DEPDIR}:
	test -d $@ || mkdir $@

clean:
	rm -rf ${OBJDIR} ${DEPDIR} ${TARGET}

# Include automatic dependencies
-include $(wildcard ${DEPDIR}/*)

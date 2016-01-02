export HIARCH?=hi3531

ifeq ($(HIARCH),hi3531)
#export CROSS		:=arm-hisiv100nptl-linux-
endif

export CC			:=$(CROSS)gcc
export AR			:=$(CROSS)ar
export STRIP	:=$(CROSS)strip
#CFLAGS				= -Wall -O3 -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16
#CFLAGS				= -Wall -O3 
CFLAGS				= -g
OBJ					  = bin/mp4-convert-tool.bin
CUSTOM_INC	  =	-I./src/sdk/include -I./include
LIB					  = -lmp4v2 -lstdc++ -lm
LIBS			  	= -L./src/sdk/lib
SRCS				=	src/sdk/src/aes.c                                                  \
					    src/td_mp4.c                                                       \

COMPILE	= $(CC) $(SRCS) $(CFLAGS) $(CUSTOM_INC) $(LIBS) $(LIB) -o $(OBJ)

all:
	mkdir -p bin
	$(COMPILE)
#	$(STRIP) $(OBJ)
clean:
	\rm $(OBJ) -f


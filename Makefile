# Generic GNUMakefile

# Just a snippet to stop executing under other make(1) commands
# that won't understand these lines
ifneq (,)
This makefile requires GNU Make.
endif

PROGRAM = foo
C_FILES := $(wildcard *.c )
CC_FILES := $(wildcard *.cc)
OBJS := $(patsubst %.c, %.o, $(C_FILES)) $(patsubst %.cc, %.o, $(CC_FILES))


ifeq ($(PROGRAM),foo_x86)

CXX=g++ 
# If for raspberry pi 2
CXXFLAGS = -Wall -pedantic -std=c++11
##CXXFLAGS += -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
CXXFLAGS +=-I /home/parallels/Documents/linux/boost_1_58_0
CFLAGS = -Wall -pedantic -std=c++11
LDFLAGS = -Bstatic
LDLIBS =  /home/parallels/Documents/linux/boost_1_58_0/stage/lib/libboost_regex.a
LDLIBS += /home/parallels/Documents/linux/boost_1_58_0/stage/lib/libboost_system.a 
LDLIBS += -Bdynamic -lpthread 
else 

# If for raspberry pi 2
CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -Wall -pedantic -std=c++11
##CXXFLAGS += -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi

# Everything must not be compiled with this setting because it leads to virtual funciton error.
#CXXFLAGS +=-mcpu=cortex-a7
CXXFLAGS +=-I /home/parallels/Documents/rpi/boost/boost_1_58_0
CXXFLAGS +=-I /home/parallels/Documents/rpi/wiringPi/local/include
CFLAGS = -Wall -pedantic -std=c++11
LDFLAGS = -Bstatic
LDLIBS =  /home/parallels/Documents/rpi/boost/boost_1_58_0/stage/lib/libboost_regex.a
LDLIBS += /home/parallels/Documents/rpi/boost/boost_1_58_0/stage/lib/libboost_system.a 
LDLIBS += /home/parallels/Documents/rpi/pwm/pwm.a
LDLIBS += /home/parallels/Documents/rpi/wiringPi/local/lib/libwiringPiDev.a
LDLIBS += /home/parallels/Documents/rpi/wiringPi/local/lib/libwiringPi.a
LDLIBS += -Bdynamic -lm -lpthread  

endif

all: $(PROGRAM)

$(PROGRAM): .depend $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM) $(LDLIBS)

depend: .depend_c

.depend: cmd = gcc -MM -MF depend $(var); cat depend >> .depend;
.depend:
	@echo "Generating dependencies..."
	@$(foreach var, $(C_FILES, CC_FILES), $(cmd))
	@rm -f depend

-include .depend

# These are the pattern matching rules. In addition to the automatic
# variables used here, the variable $* that matches whatever % stands for
# can be useful in special cases.
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%: %.c
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f .depend *.o

.PHONY: clean depend

#/bin/bash
#
#		build.sh
#		Copyright (c) 2012 Ohm Force
#
#Tab=3########################################################################

# configuration

#OhmstudiodArch="Linux64"
OhmstudiodArch="Linux"
#OhmstudiodArch="MacOSX"

#
# test a return val and exit on error
# arguments : error_val [message in case of failure, optional]
#

exit_on_failure ()
{	
	if [ $# -gt 2 -o $# -lt 1 ]; then
		echo "exit_on_failure : wrong number of arguments"
		exit 1
	fi
	if [ $1 -ne 0 ]; then
		echo "Command exited with value $1. (${2:-'No Message'})"
		exit $1
	fi
}


# main

if [[ ${OhmstudiodArch} = "Linux64" ]]; then
echo "Compiling for Linux64..."
ArchArgs='-m64 -mcx16'
LibFlags='-lz -lrt'

elif [[ ${OhmstudiodArch} = "Linux" ]]; then
echo "Compiling for Linux..."
ArchArgs='-msse -mmmx -march=i686'
LibFlags='-lz -lrt'

elif [[ ${OhmstudiodArch} = "MacOSX" ]]; then
echo "Compiling for MacOSX..."
ArchArgs='-msse -mmmx -march=core2'
LibFlags='-lz -framework Carbon'

else

echo "Error: set OhmstudiodArch variable"
exit 1

fi

#OptimArgs='-O0 -Dohm_util_TRACE_DEBUG_FLAG'
OptimArgs='-O3 -DNDEBUG'
# -g : debugging symbols for core dumps
FlagsArgs='-fno-strict-aliasing -g'

gcc \
	${OptimArgs} \
	server.cpp \
	-iquote../../../../../src/lib \
	-iquote../../../../../src/samples/ohm/flip/skeleton \
	-iquote../../../../../src/samples/ohm/flip \
	${ArchArgs} \
	${FlagsArgs} \
	${LibFlags} \
	-lstdc++ \
	-o SkeletonServer
exit_on_failure $? "gcc compilation failed"



##### EOF ####################################################################


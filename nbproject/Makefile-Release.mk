#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-linux-gnueabi-gcc
CCC=arm-none-linux-gnueabi-g++
CXX=arm-none-linux-gnueabi-g++
FC=gfortran
AS=arm-none-linux-gnueabi-as

# Macros
CND_PLATFORM=WinAVR-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/d_lcd.o \
	${OBJECTDIR}/src/libEV3.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libev3-c.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libev3-c.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libev3-c.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libev3-c.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libev3-c.a

${OBJECTDIR}/lib/d_lcd.o: lib/d_lcd.c 
	${MKDIR} -p ${OBJECTDIR}/lib
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DRELEASE -Iinclude -Ilib -Ires -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/d_lcd.o lib/d_lcd.c

${OBJECTDIR}/src/libEV3.o: src/libEV3.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DRELEASE -Iinclude -Ilib -Ires -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/libEV3.o src/libEV3.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libev3-c.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

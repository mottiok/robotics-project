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
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Behaviors/Behavior.o \
	${OBJECTDIR}/Behaviors/GoForward.o \
	${OBJECTDIR}/Behaviors/TurnLeft.o \
	${OBJECTDIR}/Behaviors/TurnRight.o \
	${OBJECTDIR}/Libraries/lodepng.o \
	${OBJECTDIR}/Manager.o \
	${OBJECTDIR}/Map.o \
	${OBJECTDIR}/MapSearchNode.o \
	${OBJECTDIR}/PathPlanner.o \
	${OBJECTDIR}/Plans/ObstacleAvoidancePlan.o \
	${OBJECTDIR}/Plans/Plan.o \
	${OBJECTDIR}/Robot.o \
	${OBJECTDIR}/SDL2Wrapper.o \
	${OBJECTDIR}/WaypointManager.o \
	${OBJECTDIR}/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/robotics-project

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/robotics-project: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/robotics-project ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Behaviors/Behavior.o: Behaviors/Behavior.cpp 
	${MKDIR} -p ${OBJECTDIR}/Behaviors
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Behaviors/Behavior.o Behaviors/Behavior.cpp

${OBJECTDIR}/Behaviors/GoForward.o: Behaviors/GoForward.cpp 
	${MKDIR} -p ${OBJECTDIR}/Behaviors
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Behaviors/GoForward.o Behaviors/GoForward.cpp

${OBJECTDIR}/Behaviors/TurnLeft.o: Behaviors/TurnLeft.cpp 
	${MKDIR} -p ${OBJECTDIR}/Behaviors
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Behaviors/TurnLeft.o Behaviors/TurnLeft.cpp

${OBJECTDIR}/Behaviors/TurnRight.o: Behaviors/TurnRight.cpp 
	${MKDIR} -p ${OBJECTDIR}/Behaviors
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Behaviors/TurnRight.o Behaviors/TurnRight.cpp

${OBJECTDIR}/Libraries/lodepng.o: Libraries/lodepng.cpp 
	${MKDIR} -p ${OBJECTDIR}/Libraries
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Libraries/lodepng.o Libraries/lodepng.cpp

${OBJECTDIR}/Manager.o: Manager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Manager.o Manager.cpp

${OBJECTDIR}/Map.o: Map.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Map.o Map.cpp

${OBJECTDIR}/MapSearchNode.o: MapSearchNode.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MapSearchNode.o MapSearchNode.cpp

${OBJECTDIR}/PathPlanner.o: PathPlanner.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PathPlanner.o PathPlanner.cpp

${OBJECTDIR}/Plans/ObstacleAvoidancePlan.o: Plans/ObstacleAvoidancePlan.cpp 
	${MKDIR} -p ${OBJECTDIR}/Plans
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Plans/ObstacleAvoidancePlan.o Plans/ObstacleAvoidancePlan.cpp

${OBJECTDIR}/Plans/Plan.o: Plans/Plan.cpp 
	${MKDIR} -p ${OBJECTDIR}/Plans
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Plans/Plan.o Plans/Plan.cpp

${OBJECTDIR}/Robot.o: Robot.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Robot.o Robot.cpp

${OBJECTDIR}/SDL2Wrapper.o: SDL2Wrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SDL2Wrapper.o SDL2Wrapper.cpp

${OBJECTDIR}/WaypointManager.o: WaypointManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WaypointManager.o WaypointManager.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/robotics-project

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

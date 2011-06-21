# Target
TARGET = nxtway_gs++

# include path
USER_INC_PATH= ../ecrobot/nxtway_gs_balancer

# library
USER_LIB = nxtway_gs_balancer

# C source file
TARGET_SOURCES = \
	balancer_param.c

# C++(.cpp) source files
TARGET_CPP_SOURCES = \
	NXTway_GS.cpp \
	sample.cpp \
	Calibrator.cpp \
	PidController.cpp \
	Balancer.cpp \
	BalancingRun.cpp \
	Distance.cpp \
	NormalLineSection.cpp \
	GoalStopSection.cpp \
	TreasureHuntSection.cpp \
	DashedLineSection.cpp \
	NewDashedLineSection.cpp \
	DolphinSection.cpp \
	NewTreasureHuntSection.cpp \
	TwinloopSection.cpp \
	InDolphinCourse.cpp \
	NewGoalStopSection.cpp

# TOPPERS/ATK(OSEK) config file
TOPPERS_OSEK_OIL_SOURCE = sample.oil

# don't change this macro
O_PATH ?= build

# makefile for C++(.cpp) build
include ../ecrobot/ecrobot++.mak

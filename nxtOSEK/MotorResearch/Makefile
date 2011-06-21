# Target
TARGET = motor_research

# include path
USER_INC_PATH= ../ecrobot/nxtway_gs_balancer

# library
USER_LIB = nxtway_gs_balancer

# C source file
TARGET_SOURCES = \
	balancer_param.c

# C++(.cpp) source files
TARGET_CPP_SOURCES = \
	main.cpp 

# TOPPERS/ATK(OSEK) config file
TOPPERS_OSEK_OIL_SOURCE = main.oil

# don't change this macro
O_PATH ?= build

# makefile for C++(.cpp) build
include ../ecrobot/ecrobot++.mak

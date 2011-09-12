# BUILD_MODE
BUILD_MODE = ROM_ONLY

# nxtOSEKルートディレクトリ
NXTOSEK_ROOT = ../../nxtOSEK

# ターゲット実行形式ファイル名
TARGET = main_cpp

# インクルードパス
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer

# ライブラリ
USER_LIB = nxtway_gs_balancer

# Cソースファイル
TARGET_SOURCES = \
	balancer_param.c

# C++(.cpp)ソースファイル
TARGET_CPP_SOURCES = \
	constants.cpp \
	coordinates.cpp \
	factory.cpp \
	main.cpp \
	Activator.cpp \
	TripodActivator.cpp \
	Pid.cpp \
	Gps.cpp \
	NormalDriver.cpp \
	TestDriver.cpp \
	SeesawDriver.cpp \
	StairwayDriver.cpp \
	ETsumoDriver.cpp \
	OutGarageDriver.cpp \
	InGarageDriver.cpp \
	InCourse.cpp \
	OutCourse.cpp \
	History.cpp \
	WallDetector.cpp \
	DownSlopeDetector.cpp \
	LineDetector.cpp \
	StraightDetector.cpp \
	FailDetector.cpp \
	LineTrace.cpp \
	TripodLineTrace.cpp \
	AngleTrace.cpp \
	CoordinateTrace.cpp \
	VirtualLineTrace.cpp \
	CoordinateStop.cpp \
	SlowdownSkill.cpp \
	StopSkill.cpp \
	StandUpSkill.cpp \
        SitDownSkill.cpp \
        LookUpGateDriver.cpp \
        StandupDriver.cpp \
        MarkerDetector.cpp

# TOPPERS/ATK(OSEK)設定ファイル
TOPPERS_OSEK_OIL_SOURCE = main.oil

# 下記のマクロは変更しないでください
O_PATH ?= build

# C++(.cpp)ビルド用makefile
include $(NXTOSEK_ROOT)/ecrobot/ecrobot++.mak

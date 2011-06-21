# nxtOSEKルートディレクトリ
NXTOSEK_ROOT = ../../nxtOSEK

# ターゲット実行形式ファイル名
TARGET = sample_cpp

# インクルードパス
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer

# ライブラリ
USER_LIB = nxtway_gs_balancer

# Cソースファイル
TARGET_SOURCES = \
	balancer_param.c

# C++(.cpp)ソースファイル
TARGET_CPP_SOURCES = \
	NXTway_GS.cpp \
	LineTrace.cpp \
	sample.cpp

# TOPPERS/ATK(OSEK)設定ファイル
TOPPERS_OSEK_OIL_SOURCE = sample.oil

# 下記のマクロは変更しないでください
O_PATH ?= build

# C++(.cpp)ビルド用makefile
include $(NXTOSEK_ROOT)/ecrobot/ecrobot++.mak

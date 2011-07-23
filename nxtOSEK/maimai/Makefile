# リアルタイムOSにTOPPERS/JSPを指定
TOPPERS_KERNEL = NXT_JSP

# nxtOSEKルートディレクトリ
NXTOSEK_ROOT = ../../nxtOSEK

# ターゲット実行形式ファイル名
TARGET = blinker

# インクルードパス
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer

# ライブラリ
USER_LIB = nxtway_gs_balancer

# Cソースファイル
TARGET_SOURCES = \
	blinker.c \
	balancer_param.c

# TOPPERS/JSP設定ファイル
TOPPERS_JSP_CFG_SOURCE = ./jsp.cfg

# 下記のマクロは変更しないでください
O_PATH ?= build

# C(.c)ビルド用 MAKEFILE
include $(NXTOSEK_ROOT)/ecrobot/ecrobot.mak

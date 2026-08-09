# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2023-2024

BLOCKSDS    ?= /opt/blocksds/core

# User config

NAME		:= ffx-runner-ds
GAME_TITLE	:= FFX Runner DS
#GAME_SUBTITLE	:=
NITROFSDIR  := fat

SOURCEDIRS      := \
	source/*.* \
	source/engine/nds \
	source/engine/*.* \
	source/game/entities \
	source/game/screens \
	source/game/mesh/nds \
	source/game/mesh/*.* \
	source/game/*.* \
	source/thirdparty

INCLUDEDIRS     := \
	source \
	source/thirdparty \
	source/thirdparty/box3d/include

LIBS            := -lnds9d

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile

CFLAGS		:= -Wall -Wno-implicit-function-declaration -Wno-incompatible-pointer-types $(INCLUDEFLAGS) $(DEFINES) \
		   $(ARCH) -O2 -ffunction-sections -fdata-sections -ffast-math \
		   -specs=$(SPECS)

CXXFLAGS	:= -Wall $(INCLUDEFLAGS) $(DEFINES) \
		   $(ARCH) -O2 -ffunction-sections -fdata-sections -ffast-math \
		   -fno-exceptions -fno-rtti \
		   -specs=$(SPECS)

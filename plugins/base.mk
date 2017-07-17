##This is public Makefile.
##include $(STAR_PATH)/base.mk

##0:debug  1:release
DEBUG_FLAG=0
CPPFLAGS=
CC=g++
MAKE=gmake
#BIN LIB



ifeq ($(DEBUG_FLAG),1)
	CPPFLAGS += -isystem -D_DB_POOL_\
	-D_DIC_POOL_ -D_REENTRANT -D_GNU_SOURCE -DUSR_LOG_ERROR -DUSR_LOG_MSG\
	-DNET_WORK -DHEAD_SHORT -DFCGI_STD 
else
	CPPFLAGS += -isystem -D_DB_POOL_ -D_DEBUG\
	-D_DIC_POOL_ -D_REENTRANT -D_GNU_SOURCE -DUSR_LOG_ERROR -DUSR_LOG_MSG\
	-DNET_WORK -DHEAD_SHORT -DFCGI_STD -Wall 
endif

ifeq ($(DEBUG_FLAG),1)
	CXXFLAGS += -Wextra -pthread 
else
	CXXFLAGS += -g  -Wextra -pthread 
endif

ifeq ($(PLUGIN_TYPE), "BIN")
endif

ifeq ($(PLUGIN_TYPE), "LIB")
	CPPFLAGS += -shared -fPIC
	CXXFLAGS += -shared -fPIC
endif


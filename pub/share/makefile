

OUT_PUT = ${SO_DIR}/data.so 

SRC_DIR = ../../
LIB_DIR = ./../../../library
SRC_PLUGINS = ${SRC_DIR}plugins
SO_DIR = ${SRC_DIR}bin/
SRC_PUB_NET = ${SRC_DIR}pub/net
SRC_PUB_UTIL = ${SRC_DIR}pub/util
SRC_PUB_STORAGE = ${SRC_DIR}pub/storage
SRC_PUB_SHARE = ${SRC_DIR}pub/share
SRC_PUB_COMM = ${SRC_DIR}pub/comm

LINK_DYNAMIC = -Wl,-Bdynamic -lpthread
LINK_STATIC = -Wl,-Bstatic -L${LIB_DIR}/third_party/glog/lib/libglog.a
LINK_PATH = -L/usr/lib64/mysql

CXXFLAGS += -g -Wextra -w -shared -fPIC 

INCLUDE_PATHS = -I${LIB_DIR} \
		-I${SRC_PLUGINS} \
		-I${SRC_DIR} \
		-I${LIB_DIR}/mylib/public \
		-I${LIB_DIR}/mylib/base \
		-I../\
		

all : ${OUT_PUT} del-temp-files

del-temp-files:
	rm -rf *.o
	
clean :
	rm -f ${OUT_PUT}  del-temp-files

data_share_mgr.o : ${SRC_PUB_SHARE}/data_share_mgr.cc  ${SRC_PUB_SHARE}/data_share_mgr.h 
	${CXX} ${CXXFLAGS} -c ${SRC_PUB_SHARE}/data_share_mgr.cc ${INCLUDE_PATHS}

${OUT_PUT} :  data_share_mgr.o 
	${CXX} ${CXXFLAGS} $^ -o ${OUT_PUT} ${LINK_PATH} -lpthread -lmysqlclient \
	${LIB_DIR}/mylib/base/lib/mybase.a \
	${LIB_DIR}/mylib/third_party/dmg_fp/lib/dmg_fp.a



###############################################################################
## 编译器
COMPILER = g++
COMPILERC = gcc

## 编译选项
## for debug -DROOM_DEBUG_MCACHE
CPP_FLAG = -g -Wall -w -fPIC -c -D_LINUX -DDEBUG -D_DEBUG -D_REENTRANT -D_GNU_SOURCE -D__DEBUG_CORE__ -DFCGI_STD  #-DCHECK_HEADER-DTEST
RCPP_FLAG = -g -Wall -w -fPIC -c -D_LINUX -DDEBUG -D_DEBUG -D_REENTRANT -D_GNU_SOURCE -DFCGI_STD #-DTEST
## for release
##CPP_FLAG = -O3 -Wall -s -w -fPIC -c -D_LINUX -DNDEBUG -D_REENTRANT -D_GNU_SOURCE -DFCGI_STD

## 链接选项
## for debug
LINK_FLAGS = -g  -fPIC
## for release
## LINK_FLAGS = -s -O3 -shared -fPIC

LIB_DIR = ../../library
BASE_DIR = ./../pub
PLUGIN_DIR = ../plugins
CUSTOM_THIRD_PATH = $(LIB_DIR)/mylib/third_party
## 头文件
INCLUDE_PATHS = -I./\
		-I../\
		-I$(BASE_DIR)/\
		-I$(PLUGIN_DIR)\
		-I$(LIB_DIR)/mylib/public/\
		-I$(LIB_DIR)/mylib/base\
		-I/usr/local/lib\
		-I$(LIB_DIR)/third_party/breakpad/src\
		-I$(CUSTOM_THIRD_PATH)\
		

## 库
LINK_PATHS =  -L./
LINK_LIBRARYS = -lrt -lpthread -lfcgi++ \


# 编译
COMPILE = $(COMPILER) $(CPP_FLAG) $(INCLUDE_PATHS) -o $@
RCOMPILE = $(COMPILER) $(RCPP_FLAG) $(INCLUDE_PATHS) -o $@

# 链接
LINK = $(COMPILER) $(LINK_FLAGS) $(LINK_PATHS) $? -o $(OUTPUT_FILE) $(LINK_LIBRARYS)
LINKALL = $(COMPILER) $(LINK_FLAGS) $(LINK_PATHS) $?  $(LINK_LIBRARYS)

POBJ =  mig_log.o core_interface.o http_formate.o fcgimodule.o  packet_processing.o\
	$(LIB_DIR)/mylib/third_party/dmg_fp/lib/dmg_fp.a\
	$(LIB_DIR)/mylib/base/lib/mybase.a\
	/usr/local/lib/lib/libiconv.a

OUTPUT_FILE = /var/www/cgi-bin/star
#############################################################################################################################
##all: clean proc delete-temp-file
all: proc 

proc: historyproc quotationsproc tradesproc payproc usersproc bankproc imcouldproc flashsale

######################bankproc###############################################
BANKNAME = bank_
BANKPROC_PATH = /bank
BANK_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
bankproc:$(BANKPROC_PATH)/bind_bank_card.fcgi $(BANKPROC_PATH)/get_bank_card_info.fcgi $(BANKPROC_PATH)/get_bind_bank_card.fcgi\
		$(BANKPROC_PATH)/unbind_bank_card.fcgi $(BANKPROC_PATH)/withdrawals.fcgi
  
$(BANKPROC_PATH)/bind_bank_card.fcgi: $(BANK_OBJECT_PATH)/$(BANKNAME)bind_bank_card.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BANKPROC_PATH)$(OTJERPROC_VERSION)/bind_bank_card.fcgi

$(BANKPROC_PATH)/get_bank_card_info.fcgi: $(BANK_OBJECT_PATH)/$(BANKNAME)get_bank_card_info.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BANKPROC_PATH)$(OTJERPROC_VERSION)/get_bank_card_info.fcgi

$(BANKPROC_PATH)/get_bind_bank_card.fcgi: $(BANK_OBJECT_PATH)/$(BANKNAME)get_bind_bank_card.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BANKPROC_PATH)$(OTJERPROC_VERSION)/get_bind_bank_card.fcgi

$(BANKPROC_PATH)/unbind_bank_card.fcgi: $(BANK_OBJECT_PATH)/$(BANKNAME)unbind_bank_card.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BANKPROC_PATH)$(OTJERPROC_VERSION)/unbind_bank_card.fcgi

$(BANKPROC_PATH)/withdrawals.fcgi: $(BANK_OBJECT_PATH)/$(BANKNAME)withdrawals.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BANKPROC_PATH)$(OTJERPROC_VERSION)/withdrawals.fcgi	
### 编译
$(BANK_OBJECT_PATH)/$(BANKNAME)bind_bank_card.o:
	$(COMPILE) .$(BANKPROC_PATH)$(OTJERPROC_VERSION)/bind_bank_card/bind_bank_card.cc
	
$(BANK_OBJECT_PATH)/$(BANKNAME)get_bank_card_info.o:
	$(COMPILE) .$(BANKPROC_PATH)$(OTJERPROC_VERSION)/get_bank_card_info/get_bank_card_info.cc

$(BANK_OBJECT_PATH)/$(BANKNAME)get_bind_bank_card.o:
	$(COMPILE) .$(BANKPROC_PATH)$(OTJERPROC_VERSION)/get_bind_bank_card/get_bind_bank_card.cc

$(BANK_OBJECT_PATH)/$(BANKNAME)unbind_bank_card.o:
	$(COMPILE) .$(BANKPROC_PATH)$(OTJERPROC_VERSION)/unbind_bank_card/unbind_bank_card.cc
	
$(BANK_OBJECT_PATH)/$(BANKNAME)withdrawals.o:
	$(COMPILE) .$(BANKPROC_PATH)$(OTJERPROC_VERSION)/withdrawals/withdrawals.cc
######################usersproc###############################################
USERSNAME = users_
USERSPROC_PATH = /users
USERS_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
usersproc:$(USERSPROC_PATH)/register.fcgi $(USERSPROC_PATH)/login.fcgi $(USERSPROC_PATH)/checktoken.fcgi\
  $(USERSPROC_PATH)/asset.fcgi $(USERSPROC_PATH)/register_ver.fcgi
  
$(USERSPROC_PATH)/register.fcgi: $(USERS_OBJECT_PATH)/$(USERSNAME)register.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERSPROC_PATH)$(OTJERPROC_VERSION)/register.fcgi

$(USERSPROC_PATH)/login.fcgi: $(USERS_OBJECT_PATH)/$(USERSNAME)login.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERSPROC_PATH)$(OTJERPROC_VERSION)/login.fcgi

$(USERSPROC_PATH)/checktoken.fcgi: $(USERS_OBJECT_PATH)/$(USERSNAME)checktoken.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERSPROC_PATH)$(OTJERPROC_VERSION)/checktoken.fcgi

$(USERSPROC_PATH)/asset.fcgi: $(USERS_OBJECT_PATH)/$(USERSNAME)asset.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERSPROC_PATH)$(OTJERPROC_VERSION)/asset.fcgi

$(USERSPROC_PATH)/register_ver.fcgi: $(USERS_OBJECT_PATH)/$(USERSNAME)register_ver.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERSPROC_PATH)$(OTJERPROC_VERSION)/register_ver.fcgi	
### 编译
$(USERS_OBJECT_PATH)/$(USERSNAME)register.o:
	$(COMPILE) .$(USERSPROC_PATH)$(OTJERPROC_VERSION)/register/register.cc
	
$(USERS_OBJECT_PATH)/$(USERSNAME)login.o:
	$(COMPILE) .$(USERSPROC_PATH)$(OTJERPROC_VERSION)/login/login.cc

$(USERS_OBJECT_PATH)/$(USERSNAME)checktoken.o:
	$(COMPILE) .$(USERSPROC_PATH)$(OTJERPROC_VERSION)/checktoken/checktoken.cc

$(USERS_OBJECT_PATH)/$(USERSNAME)asset.o:
	$(COMPILE) .$(USERSPROC_PATH)$(OTJERPROC_VERSION)/asset/asset.cc
	
$(USERS_OBJECT_PATH)/$(USERSNAME)register_ver.o:
	$(COMPILE) .$(USERSPROC_PATH)$(OTJERPROC_VERSION)/register_ver/register_ver.cc
	
	
######################payproc###############################################
PAYNAME = pay_
PAYPROC_PATH = /pay
PAY_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
payproc:$(PAYPROC_PATH)/wx_callback.fcgi $(PAYPROC_PATH)/wx_order.fcgi $(PAYPROC_PATH)/alipay_callback.fcgi
$(PAYPROC_PATH)/wx_callback.fcgi: $(PAY_OBJECT_PATH)/$(PAYNAME)wx_callback.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(PAYPROC_PATH)$(OTJERPROC_VERSION)/wx_callback.fcgi

$(PAYPROC_PATH)/wx_order.fcgi: $(PAY_OBJECT_PATH)/$(PAYNAME)wx_order.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(PAYPROC_PATH)$(OTJERPROC_VERSION)/wx_order.fcgi

$(PAYPROC_PATH)/alipay_callback.fcgi: $(PAY_OBJECT_PATH)/$(PAYNAME)alipay_callback.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(PAYPROC_PATH)$(OTJERPROC_VERSION)/alipay_callback.fcgi
### 编译
$(PAY_OBJECT_PATH)/$(PAYNAME)wx_callback.o:
	$(COMPILE) .$(PAYPROC_PATH)$(OTJERPROC_VERSION)/wx_callback/wx_callback.cc

$(PAY_OBJECT_PATH)/$(PAYNAME)wx_order.o:
	$(COMPILE) .$(PAYPROC_PATH)$(OTJERPROC_VERSION)/wx_order/wx_order.cc

$(PAY_OBJECT_PATH)/$(PAYNAME)alipay_callback.o:
	$(COMPILE) .$(PAYPROC_PATH)$(OTJERPROC_VERSION)/alipay_callback/alipay_callback.cc
######################accountproc###############################################
ACCOUNTNAME = account_
ACCOUNTPROC_PATH = /account
ACCOUNT_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
accountproc:$(ACCOUNTPROC_PATH)/accinfo.fcgi $(ACCOUNTPROC_PATH)/bankcard.fcgi $(ACCOUNTPROC_PATH)/bankname.fcgi\
	$(ACCOUNTPROC_PATH)/bindcard.fcgi $(ACCOUNTPROC_PATH)/login.fcgi $(ACCOUNTPROC_PATH)/register.fcgi\
	$(ACCOUNTPROC_PATH)/unbindcard.fcgi $(ACCOUNTPROC_PATH)/userinfo.fcgi $(ACCOUNTPROC_PATH)/verifycode.fcgi

$(ACCOUNTPROC_PATH)/accinfo.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)accinfo.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/accinfo.fcgi

$(ACCOUNTPROC_PATH)/bankcard.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)bankcard.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/bankcard.fcgi

$(ACCOUNTPROC_PATH)/bankname.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)bankname.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/bankname.fcgi
	
$(ACCOUNTPROC_PATH)/bindcard.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)bindcard.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/bindcard.fcgi
	
$(ACCOUNTPROC_PATH)/login.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)login.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/login.fcgi

$(ACCOUNTPROC_PATH)/register.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)register.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/register.fcgi
	
$(ACCOUNTPROC_PATH)/unbindcard.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)unbindcard.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/unbindcard.fcgi

$(ACCOUNTPROC_PATH)/userinfo.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)userinfo.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/userinfo.fcgi

$(ACCOUNTPROC_PATH)/verifycode.fcgi: $(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)verifycode.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/verifycode.fcgi
	
### 编译
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)accinfo.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/accinfo/accinfo.cc

$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)bankcard.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/bankcard/bankcard.cc
	
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)bankname.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/bankname/bankname.cc
	
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)bindcard.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/bindcard/bindcard.cc
	
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)login.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/login/login.cc
	
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)register.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/register/register.cc

$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)unbindcard.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/unbindcard/unbindcard.cc
	
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)userinfo.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/userinfo/userinfo.cc
	
$(ACCOUNT_OBJECT_PATH)/$(ACCOUNTNAME)verifycode.o:
	$(COMPILE) .$(ACCOUNTPROC_PATH)$(OTJERPROC_VERSION)/verifycode/verifycode.cc
	

######################wealthyproc###############################################
WEALTHNAME = wealth_
WEALTHPROC_PATH = /wealth
WEALTH_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
wealthyproc:$(WEALTHPROC_PATH)/creditlist.fcgi $(WEALTHPROC_PATH)/withdrawcash.fcgi $(WEALTHPROC_PATH)/withdrawlist.fcgi

$(WEALTHPROC_PATH)/creditlist.fcgi: $(WEALTH_OBJECT_PATH)/$(WEALTHNAME)creditlist.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(WEALTHPROC_PATH)$(OTJERPROC_VERSION)/creditlist.fcgi

$(WEALTHPROC_PATH)/withdrawcash.fcgi: $(WEALTH_OBJECT_PATH)/$(WEALTHNAME)withdrawcash.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(WEALTHPROC_PATH)$(OTJERPROC_VERSION)/withdrawcash.fcgi
	
$(WEALTHPROC_PATH)/withdrawlist.fcgi: $(WEALTH_OBJECT_PATH)/$(WEALTHNAME)withdrawlist.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(WEALTHPROC_PATH)$(OTJERPROC_VERSION)/withdrawlist.fcgi
	
### 编译
$(WEALTH_OBJECT_PATH)/$(WEALTHNAME)creditlist.o:
	$(COMPILE) .$(WEALTHPROC_PATH)$(OTJERPROC_VERSION)/creditlist/creditlist.cc

$(WEALTH_OBJECT_PATH)/$(WEALTHNAME)withdrawcash.o:
	$(COMPILE) .$(WEALTHPROC_PATH)$(OTJERPROC_VERSION)/withdrawcash/withdrawcash.cc
	
$(WEALTH_OBJECT_PATH)/$(WEALTHNAME)withdrawlist.o:
	$(COMPILE) .$(WEALTHPROC_PATH)$(OTJERPROC_VERSION)/withdrawlist/withdrawlist.cc
	
	
	
######################historyproc###############################################
HISTORYNAME = history_
HISTORYPROC_PATH = /history
HISTORY_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
historyproc:$(HISTORYPROC_PATH)/his_position_list.fcgi $(HISTORYPROC_PATH)/his_recharge.fcgi\
	$(HISTORYPROC_PATH)/his_withdrawals.fcgi $(HISTORYPROC_PATH)/handle_trades.fcgi $(HISTORYPROC_PATH)/his_handle_trades.fcgi\
	$(HISTORYPROC_PATH)/handle_withdrawals.fcgi

$(HISTORYPROC_PATH)/his_position_list.fcgi: $(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_position_list.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_position_list.fcgi

$(HISTORYPROC_PATH)/his_recharge.fcgi: $(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_recharge.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_recharge.fcgi

$(HISTORYPROC_PATH)/his_withdrawals.fcgi: $(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_withdrawals.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_withdrawals.fcgi
	
$(HISTORYPROC_PATH)/handle_trades.fcgi: $(HISTORY_OBJECT_PATH)/$(HISTORYNAME)handle_trades.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/handle_trades.fcgi

$(HISTORYPROC_PATH)/his_handle_trades.fcgi: $(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_handle_trades.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_handle_trades.fcgi
	
$(HISTORYPROC_PATH)/handle_withdrawals.fcgi: $(HISTORY_OBJECT_PATH)/$(HISTORYNAME)handle_withdrawals.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/handle_withdrawals.fcgi
		
### 编译
$(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_position_list.o:
	$(COMPILE) .$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_position_list/his_position_list.cc

$(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_recharge.o:
	$(COMPILE) .$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_recharge/his_recharge.cc
	
$(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_withdrawals.o:
	$(COMPILE) .$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_withdrawals/his_withdrawals.cc
	
$(HISTORY_OBJECT_PATH)/$(HISTORYNAME)handle_trades.o:
	$(COMPILE) .$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/handle_trades/handle_trades.cc

$(HISTORY_OBJECT_PATH)/$(HISTORYNAME)his_handle_trades.o:
	$(COMPILE) .$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/his_handle_trades/his_handle_trades.cc

$(HISTORY_OBJECT_PATH)/$(HISTORYNAME)handle_withdrawals.o:
	$(COMPILE) .$(HISTORYPROC_PATH)$(OTJERPROC_VERSION)/handle_withdrawals/handle_withdrawals.cc

######################quotationsproc###############################################
QUOTATIONSNAME = quotations_
QUOTATIONSPROC_PATH = /quotations
QUOTATIONS_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
quotationsproc:$(QUOTATIONSPROC_PATH)/kchart.fcgi $(QUOTATIONSPROC_PATH)/realtime.fcgi $(QUOTATIONSPROC_PATH)/timeline.fcgi

$(QUOTATIONSPROC_PATH)/kchart.fcgi: $(QUOTATIONS_OBJECT_PATH)/$(QUOTATIONSNAME)kchart.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(QUOTATIONSPROC_PATH)$(OTJERPROC_VERSION)/kchart.fcgi

$(QUOTATIONSPROC_PATH)/realtime.fcgi: $(QUOTATIONS_OBJECT_PATH)/$(QUOTATIONSNAME)realtime.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(QUOTATIONSPROC_PATH)$(OTJERPROC_VERSION)/realtime.fcgi

$(QUOTATIONSPROC_PATH)/timeline.fcgi: $(QUOTATIONS_OBJECT_PATH)/$(QUOTATIONSNAME)timeline.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(QUOTATIONSPROC_PATH)$(OTJERPROC_VERSION)/timeline.fcgi
		
### 编译
$(QUOTATIONS_OBJECT_PATH)/$(QUOTATIONSNAME)kchart.o:
	$(COMPILE) .$(QUOTATIONSPROC_PATH)$(OTJERPROC_VERSION)/kchart/kchart.cc

$(QUOTATIONS_OBJECT_PATH)/$(QUOTATIONSNAME)realtime.o:
	$(COMPILE) .$(QUOTATIONSPROC_PATH)$(OTJERPROC_VERSION)/realtime/realtime.cc
	
$(QUOTATIONS_OBJECT_PATH)/$(QUOTATIONSNAME)timeline.o:
	$(COMPILE) .$(QUOTATIONSPROC_PATH)$(OTJERPROC_VERSION)/timeline/timeline.cc
	

######################tradesproc###############################################
TRADESNAME = trades_
TRADESPROC_PATH = /trades
TRADES_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
OTJERPROC_VERSION = /v1
##tradesproc:$(TRADESPROC_PATH)/cur_position_list.fcgi $(TRADESPROC_PATH)/open_position.fcgi $(TRADESPROC_PATH)/goods.fcgi $(TRADESPROC_PATH)/flight_info.fcgi
tradesproc:$(TRADESPROC_PATH)/cur_position_list.fcgi $(TRADESPROC_PATH)/open_position.fcgi 

$(TRADESPROC_PATH)/cur_position_list.fcgi: $(TRADES_OBJECT_PATH)/$(TRADESNAME)cur_position_list.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/cur_position_list.fcgi

$(TRADESPROC_PATH)/open_position.fcgi: $(TRADES_OBJECT_PATH)/$(TRADESNAME)open_position.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/open_position.fcgi

$(TRADESPROC_PATH)/goods.fcgi: $(TRADES_OBJECT_PATH)/$(TRADESNAME)goods.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/goods.fcgi

$(TRADESPROC_PATH)/flight_info.fcgi: $(TRADES_OBJECT_PATH)/$(TRADESNAME)flight_info.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/flight_info.fcgi
		
### 编译
$(TRADES_OBJECT_PATH)/$(TRADESNAME)cur_position_list.o:
	$(COMPILE) .$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/cur_position_list/cur_position_list.cc

$(TRADES_OBJECT_PATH)/$(TRADESNAME)open_position.o:
	$(COMPILE) .$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/open_position/open_position.cc
	
$(TRADES_OBJECT_PATH)/$(TRADESNAME)goods.o:
	$(COMPILE) .$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/goods/goods.cc

$(TRADES_OBJECT_PATH)/$(TRADESNAME)flight_info.o:
	$(COMPILE) .$(TRADESPROC_PATH)$(OTJERPROC_VERSION)/flight_info/flight_info.cc


######################imcouldproc###############################################
IMCOULDNAME = imcould_
IMCOULDPROC_PATH = /imcloud
IMCOULD_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
#OTJERPROC_VERSION = /v1 
imcouldproc:$(IMCOULDPROC_PATH)/getcloudtoken.fcgi 

$(IMCOULDPROC_PATH)/getcloudtoken.fcgi: $(IMCOULD_OBJECT_PATH)/$(IMCOULDNAME)getcloudtoken.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(IMCOULDPROC_PATH)$(OTJERPROC_VERSION)/getcloudtoken.fcgi

### 编译
$(IMCOULD_OBJECT_PATH)/$(IMCOULDNAME)getcloudtoken.o:
	$(COMPILE) .$(IMCOULDPROC_PATH)$(OTJERPROC_VERSION)/getcloudtoken/getcloudtoken.cc


######################flashsale###############################################
FLASHNAME = flashsale_
FLASHPROC_PATH = /flashsale
FLASH_OBJECT_PATH = ./obj$(NEWSPROC_PATH)
#OTJERPROC_VERSION = /v1 
flashsale:$(FLASHPROC_PATH)/remaintime.fcgi $(FLASHPROC_PATH)/pubstarinfo.fcgi $(FLASHPROC_PATH)/confirmorder.fcgi 

$(FLASHPROC_PATH)/remaintime.fcgi: $(FLASH_OBJECT_PATH)/$(FLASHNAME)remaintime.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FLASHPROC_PATH)$(OTJERPROC_VERSION)/remaintime.fcgi

$(FLASHPROC_PATH)/pubstarinfo.fcgi: $(FLASH_OBJECT_PATH)/$(FLASHNAME)pubstarinfo.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FLASHPROC_PATH)$(OTJERPROC_VERSION)/pubstarinfo.fcgi

$(FLASHPROC_PATH)/confirmorder.fcgi: $(FLASH_OBJECT_PATH)/$(FLASHNAME)confirmorder.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FLASHPROC_PATH)$(OTJERPROC_VERSION)/confirmorder.fcgi

### 编译
$(FLASH_OBJECT_PATH)/$(FLASHNAME)remaintime.o:
	$(COMPILE) .$(FLASHPROC_PATH)$(OTJERPROC_VERSION)/remaintime/remaintime.cc

$(FLASH_OBJECT_PATH)/$(FLASHNAME)pubstarinfo.o:
	$(COMPILE) .$(FLASHPROC_PATH)$(OTJERPROC_VERSION)/pubstarinfo/pubstarinfo.cc

$(FLASH_OBJECT_PATH)/$(FLASHNAME)confirmorder.o:
	$(COMPILE) .$(FLASHPROC_PATH)$(OTJERPROC_VERSION)/confirmorder/confirmorder.cc




clean:
	-rm -f *.o
	-rm -f $(OUTPUT_FILE)./*.fcgi
	-rm -f obj/*.o

delete-temp-file:
	-rm -f obj/*.o

############public###############################



packet_processing.o :
	$(COMPILE) $(BASE_DIR)/net/packet_processing.cc

base_values.o:
	$(COMPILE) $(LIB_DIR)/mylib/base/logic/base_values.cc

http_serializer.o:
	$(COMPILE) $(LIB_DIR)/mylib/base/logic/http_serializer.cc
	
json_serializer.o:
	$(COMPILE) $(LIB_DIR)/mylib/base/logic/json_serializer.cc
	
jsonp_serializer.o:
	$(COMPILE) $(LIB_DIR)/mylib/base/logic/jsonp_serializer.cc
		
logic_comm.o:
	$(COMPILE) $(LIB_DIR)/mylib/base/logic/logic_comm.cc
	
mig_log.o:
	$(COMPILE) $(LIB_DIR)/mylib/public/log/mig_log.cc

basic_util.o:
	$(COMPILE) $(LIB_DIR)/mylib/public/basic/basic_util.cc
	
core_interface.o:
	$(COMPILE) $(LIB_DIR)/mylib/public/comm/core_interface.cc
	
http_formate.o:
	$(COMPILE) $(LIB_DIR)/mylib/public/http/http_formate.cc
	
data_packet.o:
	$(COMPILE) $(LIB_DIR)/mylib/public/protocol/data_packet.cc

fcgimodule.o:
	$(COMPILE) $(BASE_DIR)/fcgimodule/fcgimodule.cc

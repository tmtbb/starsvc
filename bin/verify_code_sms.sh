#!/usr/bin/env bash

RECEIVER=$1      #接收方手机号
SIGN="云巅"            #签名
if [ $3 -eq 0 ];then
  TEMP_CODE="SMS_14221547"      #注册账号
fi
if [ $3 -eq 1 ];then
  TEMP_CODE="SMS_14251903"     #验证码登录
fi
PARAMS="{\"no\":\"$2\"}" #模板参数（json格式）

K="23447975" #AppKey，从管理控制台获取，下同
S="152cee1b237b87e62210f55ea85840c8" #AppSecret

NL="
"
[ "x`uname`" = "xDarwin" ] && {
NONCE="`uuidgen`"
TIMESTAMP="`date +%s`500"
} || {
NONCE="`uuidgen`"
TIMESTAMP="`date +%s%3N`"
}
STR_HEADER="X-Ca-Key:$K${NL}X-Ca-Nonce:$NONCE${NL}X-Ca-Timestamp:$TIMESTAMP"
STR_URI="/singleSendSms?ParamString=$PARAMS&RecNum=$RECEIVER&SignName=$SIGN&TemplateCode=$TEMP_CODE"
STR_TO_SIGN="GET${NL}${NL}${NL}${NL}${NL}$STR_HEADER${NL}$STR_URI"
SIGN="`/bin/echo -n "$STR_TO_SIGN" | openssl dgst -sha256 -hmac "$S" | sed 's/.* //g' | xxd -r -p | base64`"
STR_URI="`echo "$STR_URI" | sed 's#{#\\\\{#g;s#}#\\\\}#g'`"
curl -H 'Accept:' \
    -H "X-Ca-Key: $K" \
    -H "X-Ca-Nonce: $NONCE" \
    -H "X-Ca-Timestamp: $TIMESTAMP" \
    -H "X-Ca-Signature-Headers: X-Ca-Key,X-Ca-Nonce,X-Ca-Timestamp" \
    -H "X-Ca-Signature: $SIGN" \
    "http://sms.market.alicloudapi.com$STR_URI"

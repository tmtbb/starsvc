#!/usr/bin/env bash
while [ 1 ]
do
if [  -z `pgrep loader.out` ]
then
	cd /home/liweizhao/starsvc/bin
	./verify_code_sms.sh 17706401941 crash 0
	./loader.out
else
	echo "continue loop"
fi
	sleep 10
done 

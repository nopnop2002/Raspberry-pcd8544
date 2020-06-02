#/bin/bash
set -x

argc=$#
if [ $argc -eq 1 ]; then
  sleep=$1
fi

./nokia r

./nokia c 59
hostname=`hostname`
./nokia +a $hostname

. /etc/os-release
IFS=' '
set -- $VERSION_CODENAME
./nokia +2 "$1"

ipaddr=`sudo ifconfig eth0 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
if [ -z ${ipaddr} ]; then
  ./nokia +c "No eth0"
else
  ./nokia +c "Ethernet"
  ./nokia +4 $ipaddr
fi

ipaddr=`sudo ifconfig wlan0 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
if [ -z ${ipaddr} ]; then
  ipaddr=`sudo ifconfig wlan1 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
fi

if [ -z ${ipaddr} ]; then
  ./nokia +e "No wlan1"
else
  ./nokia +e "WiFi"
  ./nokia +6 $ipaddr
fi

sudo ./nokia s

if [ $argc -eq 1 ]; then
  sleep $sleep
  ./nokia r
  sudo ./nokia s
fi

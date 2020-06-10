#/bin/bash
#set -x

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

eth0=`sudo ifconfig -a | grep -c eth0`
if [ $eth0 == "1" ]; then
  ipaddr=`sudo ifconfig eth0 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
  ./nokia +c "Ethernet"
  ./nokia +4 $ipaddr
else
  ./nokia +c "No eth0"
fi

ipaddr=""
wlan=`sudo ifconfig -a | grep -c wlan0`
if [ $wlan == "1" ]; then
  ipaddr=`sudo ifconfig wlan0 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
fi

wlan=`sudo ifconfig -a | grep -c wlan1`
if [ $wlan == "1" ]; then
  ipaddr=`sudo ifconfig wlan1 |grep "inet" |awk {'print $2'} |cut -f2 -d:`
fi

if [ -z ${ipaddr} ]; then
  ./nokia +e "No wlan"
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

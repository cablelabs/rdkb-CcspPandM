#! /bin/sh

#export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
#export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

#source /fss/gw/etc/utopia/service.d/log_env_var.sh
source /etc/utopia/service.d/log_capture_path.sh

#loop=1

#while [ $loop -eq 1 ]
#do
#       sleep 10
	echo_t "Obtaining the MoCA status.."
	moca_status=`dmcli eRT getv Device.MoCA.Interface.1.Status | grep string | awk '{print $5}'`
	echo_t "MoCA is $moca_status"
#done
        echo_t "Checking for MoCA Enable.."
	moca_enable=`dmcli eRT getv Device.MoCA.Interface.1.Enable | grep bool | awk '{print $5}'`
	if [ $moca_enable == "true" ];then 
        echo_t "MoCA is Enabled"
        else 
        echo_t "MoCA is Disabled"
        fi
        
    echo_t "Obtaining the MoCA CurrentOperFreq.."
	moca_freq=`dmcli eRT getv Device.MoCA.Interface.1.CurrentOperFreq | grep uint | awk '{print $5}'`
	echo_t "MoCA_FREQ:$moca_freq"
	
	echo_t "Obtaining the MoCA PreferredNC.."
	moca_pnc=`dmcli eRT getv Device.MoCA.Interface.1.PreferredNC | grep bool | awk '{print $5}'`
	echo_t "MoCA_PNC:$moca_pnc"
	
	echo_t "Obtaining the MoCA ChannelScanning.."
	moca_scan=`dmcli eRT getv Device.MoCA.Interface.1.X_CISCO_COM_ChannelScanning | grep bool | awk '{print $5}'`
	echo_t "MoCA_SCAN:$moca_scan"

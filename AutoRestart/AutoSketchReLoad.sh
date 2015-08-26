#!/bin/bash
#
# AutoSketchReload: Starts the AutoSketchReload Daemon
#
# chkconfig: 345 99 02
# description: Auto Reload for Arduino Sketch which updated by lsyncd
#              over a network.
# processname: AutoSketchReload

. /etc/init.d/functions

autorestart="/home/root/intel_edison/AutoRestart/autoRestart.sh"
pidfile="/var/run/autorestart.pid"
prog="autoRestart.sh"
RETVAL=0

start() {
        pid=`/bin/pidof autoRestart.sh`
        if [ $? -eq 0 ] && [ ! -z "$pid" ]; then
        echo "autoRestart.sh (pid $pid) is already running..."
        exit
        fi

        eval $autorestart

        pid=`/bin/pidof autoRestart.sh`
        if [ $? -eq 0 ] && [ ! -z "$pid" ]; then
        echo "Starting autoRestaert.sh (pid $pid)"
        else
        echo "Error autoRestart is not running"
        fi
}

stop()
{
        /bin/kill -9 `/bin/pidof autoRestart.sh`
        until [ -z "$(/bin/pidof autoRestart.sh)" ]; do :; done
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        start
        ;;
    status)
        pid=`/bin/pidof autoRestart.sh`
        if [ $? -eq 0 ] && [ ! -z "$pid" ]; then
        echo "autoRestart.sh (pid $pid) is running..."
        else
        echo "autoRestart.sh is not running"
        fi
        ;;
    *)
        echo "Usage: AutoSketchReLoad.sh {start|stop|restart|status}"
        exit 1
esac


exit $?

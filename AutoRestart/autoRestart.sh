#!/bin/sh
sketchFile="/home/root/Sketch/sketch.elf"
sketchCopy='systemctl stop clloader ; cp /home/root/Sketch/sketch.elf /sketch/sketch.elf ; systemctl start clloader'

INTERVAL=1 # watch interval
last=`ls -e $sketchFile | awk '{print $8"-"$9}'`
while true; do
        sleep $INTERVAL
        current=`ls -e $sketchFile | awk '{print $8"-"$9}'`
        if [ "$last" != "$now" ]; then
                echo "updated: $now"
                last=$now
                eval $sketchCopy
        fi
done

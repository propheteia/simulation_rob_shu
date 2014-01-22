#!/bin/bash
#
# IranOpen 2013 sample start script for 3D soccer simulation
#
AGENT_BINARY="strive3d"
BINARY_DIR="./"
NUM_PLAYERS=11
killall -9 "$AGENT_BINARY" &> /dev/null
for ((i=1;i<=$NUM_PLAYERS;i++)); do
echo "Running agent No. $i"
"$BINARY_DIR/$AGENT_BINARY" --host=$1 -u=$i > log/stdout$i 2> log/stderr$i &
sleep 2
done


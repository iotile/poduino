#!/bin/bash

# workon poduino
iotile-sgcompile sensor-graph.sgf -f snippet | iotile hw --port=bled112 connect $1 controller sensor_graph

# Send an event to reset counter to 0
iotile hw --port=bled112 connect $1 get 11 send_event 0

# Just for confirmation, check both that the last event from the tile is 0,
iotile hw --port=bled112 connect $1 get 11 last_event
# and that the last value in the sensor graph's input 10 is zero
iotile hw --port=bled112 connect $1 controller sensor_graph inspect_virtualstream 'input 10'
# and that the streamer report has one event (set at boot time)
iotile hw --port=bled112 connect $1 controller sensor_graph download_stream 'output 1'

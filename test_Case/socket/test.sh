#!/bin/bash

# Run receiver_c, then sender_c
echo "//////////// receiver_c, then sender_c //////////"
./receiver_c &  # Run receiver_c in the background
sleep 0.1         # Give it time to start up
./sender_c      # Run sender_c

sleep 0.5
# Run receiver_c, then sender_p
echo "////////// receiver_c, then sender_p //////////"
./receiver_c &  # Run receiver_c in the background
sleep 0.1         # Give it time to start up
python3 sender.py  # Run sender_p

sleep 0.5
# Run receiver_p, then sender_p
echo "////////// receiver_p, then sender_p //////////"
python3 receiver.py &  # Run receiver_p in the background
sleep 0.1                 # Give it time to start up
python3 sender.py     # Run sender_p

sleep 0.5
# Run receiver_p, then sender_c
echo "////////// receiver_p, then sender_c //////////"
python3 receiver.py &  # Run receiver_p in the background
sleep 0.1                 # Give it time to start up
./sender_c              # Run sender_c

echo "All programs executed sequentially!"


#!/bin/bash
echo "//////////// receiver_c, then sender_c //////////"
./receiver_c & ./sender_c
sleep 0.5

echo "////////// receiver_c, then sender_p //////////"  # Run receiver_c in >
./receiver_c & python3 sender.py
sleep 0.5

echo "////////// receiver_p, then sender_p //////////"
python3 receiver.py & python3 sender.py
sleep 0.5

# Run receiver_p, then sender_c
echo "////////// receiver_p, then sender_c //////////"
python3 receiver.py & ./sender_c

echo "All programs executed sequentially!"

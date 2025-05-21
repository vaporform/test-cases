import time
import sys
import os
import struct
from datetime import datetime

# Define the message queue name
QUEUE_NAME = "/my_time_queue"

def sender():
    """Sends the current time to the message queue."""
    try:
        # Create the message queue if it doesn't exist, or open it if it does
        mq = os.open(QUEUE_NAME, os.O_WRONLY | os.O_CREAT, 0o666)
        print(f"Sender: Opened message queue {QUEUE_NAME}")

        while True:
            # Get the current time
            now = datetime.now()
            timestamp = now.timestamp()

            # Pack the timestamp into a float
            message = struct.pack('f', timestamp)

            # Send the message
            os.write(mq, message)
            print(f"Sender: Sent current time: {now.strftime('%Y-%m-%d %H:%M:%S.%f')}")

            # Wait for a bit before sending the next time
            time.sleep(2)

    except FileExistsError:
        print(f"Sender: Message queue {QUEUE_NAME} already exists.")
    except OSError as e:
        print(f"Sender: Error opening/writing to message queue: {e}")
    finally:
        if 'mq' in locals():
            os.close(mq)
            print("Sender: Closed message queue.")

def receiver():
    """Receives the time from the message queue and calculates the delay."""
    try:
        # Open the existing message queue for reading
        mq = os.open(QUEUE_NAME, os.O_RDONLY)
        print(f"Receiver: Opened message queue {QUEUE_NAME}")

        while True:
            # Receive the message (size of a float)
            message = os.read(mq, 4)
            if not message:
                # Queue might be empty or closed
                print("Receiver: No message received. Exiting.")
                break

            # Unpack the received timestamp
            received_timestamp = struct.unpack('f', message)[0]
            received_time = datetime.fromtimestamp(received_timestamp)
            now = datetime.now()
            delay = now - received_time

            print(f"Receiver: Received time: {received_time.strftime('%Y-%m-%d %H:%M:%S.%f')}")
            print(f"Receiver: Time taken to receive: {delay}")

    except FileNotFoundError:
        print(f"Receiver: Message queue {QUEUE_NAME} not found.")
    except OSError as e:
        print(f"Receiver: Error opening/reading from message queue: {e}")
    finally:
        if 'mq' in locals():
            os.close(mq)
            print("Receiver: Closed message queue.")

if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] not in ['sender', 'receiver']:
        print("Usage: python your_script_name.py <sender|receiver>")
        sys.exit(1)

    if sys.argv[1] == 'sender':
        sender()
    elif sys.argv[1] == 'receiver':
        receiver()

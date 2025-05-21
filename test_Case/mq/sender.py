# sender.py
import time
import sys
import posix_ipc

QUEUE_NAME = "/my_test_queue"

try:
    mq = posix_ipc.MessageQueue(QUEUE_NAME)
except posix_ipc.ExistentialError:
    print(f"Queue {QUEUE_NAME} not found. Run receiver.py first.")
    sys.exit(1)

try:
    current_time = str(time.time())
    mq.send(current_time.encode('utf-8'))
    print(f"Sender sent: {current_time}")
finally:
    mq.close()

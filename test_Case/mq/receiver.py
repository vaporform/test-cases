# receiver.py
import time
import posix_ipc

QUEUE_NAME = "/my_test_queue"

try:
    mq = posix_ipc.MessageQueue(QUEUE_NAME, flags=posix_ipc.O_CREAT)
    print(f"Queue {QUEUE_NAME} created.")
except posix_ipc.ExistentialError:
    mq = posix_ipc.MessageQueue(QUEUE_NAME)
    print(f"Queue {QUEUE_NAME} already exists.")

try:
    message, priority = mq.receive()
    receive_time = time.time()
    sent_time = float(message.decode('utf-8'))
    print(f"Sent time from sender: {sent_time} Receive time: {receive_time}")
finally:
    mq.close()
    # Optionally uncomment this line to remove the queue after testing
    posix_ipc.unlink_message_queue(QUEUE_NAME)

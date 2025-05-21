
#include <iostream>
#include <mqueue.h>
#include <fcntl.h> // For O_CREAT, O_RDONLY
#include <sys/stat.h> // For S_IRUSR, S_IWUSR
#include <string>
#include <iomanip> // STD fixed
#include <chrono> // For high-resolution time

const char* QUEUE_NAME = "/my_test_queue";
const long MAX_MSG_SIZE = 8192; // Max message size, needs to be consistent with sender

int main() {
    mqd_t mq; // Message queue descriptor
    struct mq_attr attr;

    // Set up message queue attributes (optional, but good practice)
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; // Maximum number of messages in the queue
    attr.mq_msgsize = MAX_MSG_SIZE; // Maximum message size
    attr.mq_curmsgs = 0; // Current number of messages in queue (ignored for mq_open)

    // Try to open the message queue
    // O_CREAT: Create the queue if it doesn't exist
    // O_RDONLY: Open for reading only
    // 0644: Permissions (read/write for owner, read for group/others)
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);

    if (mq == (mqd_t)-1) {
        // If mq_open with O_CREAT failed, try opening without O_CREAT
        // This handles the case where the queue already exists
        if (errno == EEXIST) {
            mq = mq_open(QUEUE_NAME, O_RDONLY);
            if (mq == (mqd_t)-1) {
                perror("mq_open (existing queue) failed");
                return 1;
            }
            std::cout << "Queue " << QUEUE_NAME << " already exists." << std::endl;
        } else {
            perror("mq_open failed");
            return 1;
        }
    } else {
        std::cout << "Queue " << QUEUE_NAME << " created." << std::endl;
    }

    // Buffer to hold the received message
    char* buffer = new char[MAX_MSG_SIZE];
    unsigned int priority;
    ssize_t bytes_read;

    try {
        // Receive the message
        bytes_read = mq_receive(mq, buffer, MAX_MSG_SIZE, &priority);
        if (bytes_read == -1) {
            perror("mq_receive failed");
            // Clean up resources even on error
            mq_close(mq);
            mq_unlink(QUEUE_NAME); // Optionally unlink on error as well
            delete[] buffer;
            return 1;
        }

        // Get receive time
        auto receive_time_chrono = std::chrono::high_resolution_clock::now();
        double receive_time = std::chrono::duration<double>(receive_time_chrono.time_since_epoch()).count();

        // Null-terminate the received message to treat it as a string
        buffer[bytes_read] = '\0';
        std::string message_str(buffer);

        // Convert the received string to a double
        double received_message = std::stod(message_str);
	std::cout << std::fixed << std::setprecision(10);
        std::cout << "Sent time from sender: " << received_message << std::endl;
	std::cout << "Receive time: " << receive_time << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error processing message: " << e.what() << std::endl;
    }

    // Clean up
    mq_close(mq);
    // Optionally uncomment this line to remove the queue after testing
    // mq_unlink(QUEUE_NAME);

    delete[] buffer;
    return 0;
}

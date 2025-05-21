#include <iostream>
#include <mqueue.h>
#include <fcntl.h>   // For O_WRONLY
#include <string>
#include <chrono>    // For high-resolution time
#include <exception> // For std::exception

const char* QUEUE_NAME = "/my_test_queue";
const long MAX_MSG_SIZE = 8192; // Max message size, consistent with receiver

int main() {
    mqd_t mq; // Message queue descriptor

    // Try to open the message queue for writing only
    // O_WRONLY: Open for writing only
    mq = mq_open(QUEUE_NAME, O_WRONLY);

    if (mq == (mqd_t)-1) {
        if (errno == ENOENT) {
            std::cerr << "Queue " << QUEUE_NAME << " not found. Run receiver first." << std::endl;
        } else {
            perror("mq_open failed");
        }
        return 1; // Exit with an error code
    }

    try {
        // Get current time with high resolution
        auto current_time_chrono = std::chrono::high_resolution_clock::now();
        double current_time_double = std::chrono::duration<double>(current_time_chrono.time_since_epoch()).count();

        // Convert the double time to a string
        std::string current_time_str = std::to_string(current_time_double);

        // Send the message
        // mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio)
        // msg_ptr: pointer to the message buffer
        // msg_len: length of the message (including null terminator if you wish, but not strictly required by mq_send)
        // msg_prio: message priority (0 is lowest)
        int send_status = mq_send(mq, current_time_str.c_str(), current_time_str.length(), 0);

        if (send_status == -1) {
            perror("mq_send failed");
            // Clean up resources even on error
            mq_close(mq);
            return 1;
        }

        std::cout << "Sender sent: " << current_time_str << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error preparing or sending message: " << e.what() << std::endl;
        // Ensure queue is closed even if an exception occurs
        mq_close(mq);
        return 1;
    }

    // Close the message queue
    mq_close(mq);

    return 0;
}

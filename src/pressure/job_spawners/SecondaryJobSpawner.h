//
// Created by capstone on 2/12/25.
//

#ifndef JOBTHREADSPAWNER_H
#define JOBTHREADSPAWNER_H
#include <future>
#include <mpi.h>
#include <thread>
#include <string>

#include "PressureJobConstants.h"
#include "../DecryptJob.h"
#include "../TowerPressureDecrypt.h"

[[noreturn]] inline void secDecryptSpawner() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        while (activeCores > currentOperations && !toDecrypt.empty()) {
            const auto [toHash, pattern] = decryptNext();
            if (!toHash.empty() && !pattern.empty()) {
                std::packaged_task decryptTask(decrypt);
                std::thread task_td(std::move(decryptTask), activeCores, toHash, pattern, std::ref(decryptedPins));
                task_td.detach();
                currentOperations++;
            }
        }
        while (anyCompletedJobs()) {
            const int nodeFullness = activeCores - currentOperations;
            const int nodeBacklog = static_cast<int>(toDecrypt.size()) + currentOperations;

            const auto [pin, toHash] = consumeDecrypted();
            std::string msg;
            msg.append(pin);
            msg.append(PRESSURE_JOB_MSG_DELIMITER);
            msg.append(toHash);
            msg.append(PRESSURE_JOB_MSG_DELIMITER);
            msg.append(std::to_string(nodeFullness));
            msg.append(PRESSURE_JOB_MSG_DELIMITER);
            msg.append(std::to_string(nodeBacklog));

            // Return the following to the main node:
            // 1: The decrypted PIN,
            // 2: The string the pin was for,
            // 3: The amount of space that this node has for active decrypting
            // 4: The amount of decrypting jobs stuck in the backlog
            MPI_Send(msg.c_str(), static_cast<int>(msg.size()) + 1, MPI_UNSIGNED_CHAR, 0, 2, MPI_COMM_WORLD);
            currentOperations--;
        }
    }
}

[[noreturn]] inline void secPressureConsumer() {
    int msgReceived = 0;
    MPI_Status status;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // Adjust the number of messages as needed
        MPI_Iprobe(0, 1, MPI_COMM_WORLD, &msgReceived, &status);
        if (msgReceived) {
            // Probe to find out the incoming message size
            MPI_Probe(0, 1, MPI_COMM_WORLD, &status);

            int message_size;
            MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &message_size);

            // Allocate just enough space
            std::vector<char> buffer(message_size);

            // Receive the message
            MPI_Recv(buffer.data(), message_size, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::cout << "Secondary Received: " << buffer.data() << " from: " << status.MPI_SOURCE << std::endl;

            std::string potentialJob = buffer.data();
            const auto jobInfo = split(potentialJob, PRESSURE_JOB_MSG_DELIMITER);

            if (jobInfo.size() == 3) {
                setActiveCoresTo(std::stoi(jobInfo[0]), 0);
                addToDecrypt(jobInfo[1], jobInfo[2]);
            } else if (jobInfo.size() == 2) {
                addToDecrypt(jobInfo[0], jobInfo[1]);
            }
        }
    }
}

#endif //JOBTHREADSPAWNER_H

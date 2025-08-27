//
// Created by capstone on 2/12/25.
//

#ifndef MAINJOBSPAWNER_H
#define MAINJOBSPAWNER_H
#include <future>
#include <iostream>
#include <list>
#include <mpi.h>
#include <thread>

#include "PressureJobConstants.h"
#include "../../helper/Accumulator.h"
#include "../DecryptJob.h"
#include "../TowerPressureDecrypt.h"

inline std::chrono::steady_clock::time_point completionStart = std::chrono::steady_clock::now();
inline Accumulator completionRate;
inline std::map<int, std::pair<int, int>> nodeStateMap;

inline int leastStressedNode() {
    int bestNode = 0;
    int bestNodeFreeSpace = 0;
    int bestNodeBacklogAmount = 0;
    for (const auto &node : nodeStateMap) {
        const auto [nodeNumber, nodeDetails] = node;
        const auto [freeSpace, backlogAmount] = nodeDetails;
        if (bestNode == 0) {
            bestNode = nodeNumber;
            bestNodeFreeSpace = freeSpace;
            bestNodeBacklogAmount = backlogAmount;
            continue;
        }
        if (nodeNumber <= activeCores && (freeSpace > bestNodeFreeSpace || (freeSpace == bestNodeFreeSpace && backlogAmount < bestNodeBacklogAmount))) {
            if (backlogAmount == 0) {
                bestNode = nodeNumber;
                bestNodeFreeSpace = freeSpace;
                bestNodeBacklogAmount = backlogAmount;
                continue;
            }
            if (backlogAmount <= bestNodeBacklogAmount) {
                bestNode = nodeNumber;
                bestNodeFreeSpace = freeSpace;
                bestNodeBacklogAmount = backlogAmount;
            }
        }
    }
    return bestNode;
}

[[noreturn]] inline void priPressureConsumer() {
    int completions = 0;
    int msgReceived = 0;
    MPI_Status status;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // Adjust the number of messages as needed
        MPI_Iprobe(MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &msgReceived, &status);
        if (msgReceived) {
            // Probe to find out the incoming message size
            MPI_Probe(MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);

            int message_size;
            MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &message_size);

            // Allocate just enough space
            std::vector<char> buffer(message_size);

            // Receive the message
            MPI_Recv(buffer.data(), message_size, MPI_UNSIGNED_CHAR, status.MPI_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::cout << "Main Received: " << buffer.data() << " from: " << status.MPI_SOURCE << std::endl;

            std::string potentialJob = buffer.data();
            const auto completionInfo = split(potentialJob, PRESSURE_JOB_MSG_DELIMITER);

            if (completionInfo.size() == 4) {
                if (nodeStateMap.find(status.MPI_SOURCE) != nodeStateMap.end()) {
                    nodeStateMap.find(status.MPI_SOURCE)->second.first = std::stoi(completionInfo[2]);
                    nodeStateMap.find(status.MPI_SOURCE)->second.second = std::stoi(completionInfo[3]);
                } else {
                    nodeStateMap.insert({status.MPI_SOURCE, {std::stoi(completionInfo[2]), std::stoi(completionInfo[3])}});
                }

                job_mutex.lock();
                completions++;
                currentOperations--;
                job_mutex.unlock();
            }
        }
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - completionStart).count();
        if (timeDiff >= 1) {
            std::cout << std::flush;
            completionRate.accumulate(completions);
            completions = 0;
            completionStart = std::chrono::steady_clock::now();
        }
    }
}

[[noreturn]] inline void priPressureProducer() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        while (!toDecrypt.empty()) {
            const auto [toHash, pattern] = decryptNext();
            if (!toHash.empty() && !pattern.empty()) {
                int bestNode = leastStressedNode();
                const auto [freeSpace, backlogAmount] = nodeStateMap.find(bestNode)->second;
                if (freeSpace > 0) {
                    nodeStateMap.find(bestNode)->second.first--;
                } else {
                    nodeStateMap.find(bestNode)->second.second++;
                }

                const int activeCores = coreNodePartitions.find(bestNode)->second;
                printf("Active: %d, Best: %d\n", activeCores, bestNode);
                std::cout << std::flush;

                std::string msg;
                msg.append(std::to_string(activeCores));
                msg.append(PRESSURE_JOB_MSG_DELIMITER);
                msg.append(toHash);
                msg.append(PRESSURE_JOB_MSG_DELIMITER);
                msg.append(pattern);

                job_mutex.lock();
                currentOperations++;
                job_mutex.unlock();

                // Return the following to the main node:
                // 1: The amount of cores that the decrypter should be using at max,
                // 2: The string to decrypt
                // 3: The pattern used to decrypt
                MPI_Send(msg.c_str(), static_cast<int>(msg.size()) + 1, MPI_UNSIGNED_CHAR, bestNode, 1, MPI_COMM_WORLD);
            }
        }
    }
}

#endif //MAINJOBSPAWNER_H

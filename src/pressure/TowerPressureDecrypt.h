#ifndef TOWERPRESSUREDECRPT_H
#define TOWERPRESSUREDECRPT_H

#include <future>
#include <mutex>
#include <regex>
#include <queue>
#include "../helper/Accumulator.h"

inline Accumulator additionRate;
inline std::queue<std::pair<std::string, std::string> > toDecrypt;
inline std::queue<std::pair<std::string, std::string> > decryptedPins;
inline int activeCores = 1;
inline std::map<int, int> coreNodePartitions;
inline bool masterIgnoreCore = false;
inline int currentOperations = 0;
inline int currentNodes = 0;
inline std::mutex job_mutex;
inline std::mutex queue_mutex;

template<typename R>
bool is_ready(std::future<R> const &f) {
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

inline void setActiveCoresTo(const int active, const int numSlaveNodes) {
    activeCores = active;
    if (numSlaveNodes > 0 && active > 0) {
        coreNodePartitions.clear();
        const int baseValue = active / numSlaveNodes;
        const int remainder = active % numSlaveNodes;

        std::vector coreParts(numSlaveNodes, baseValue);

        // Distribute the remainder
        for (int i = 0; i < remainder; ++i) {
            coreParts[i] += 1;
        }
        for (int i = 0; i < numSlaveNodes; i++) {
            coreNodePartitions.insert({ i + 1, coreParts.at(i) });
        }
    }
}

inline void setMasterIgnoreCores(const bool active) {
    masterIgnoreCore = active;
}

inline void addToAccumRate(const int pressureAdded) {
    additionRate.accumulate(pressureAdded);
}

inline void addToDecrypt(const std::string &toHash, const std::string &pattern) {
    queue_mutex.lock();
    toDecrypt.emplace(toHash, pattern);
    queue_mutex.unlock();
}

// PIN is first entry, the hashed string is second entry
inline std::pair<std::string, std::string> consumeDecrypted() {
    job_mutex.lock();
    if (decryptedPins.empty()) {
        return {"", ""};
    }
    std::pair<std::string, std::string> entry = decryptedPins.front();
    decryptedPins.pop();
    job_mutex.unlock();
    return entry;
}

inline bool anyCompletedJobs() {
    job_mutex.lock();
    const bool anyCompleted = !decryptedPins.empty();
    job_mutex.unlock();
    return anyCompleted;
}

inline std::pair<std::string, std::string> decryptNext() {
    queue_mutex.lock();
    if (toDecrypt.empty() || (!masterIgnoreCore && currentOperations >= activeCores)) {
        queue_mutex.unlock();
        return {"", ""};
    }

    const auto [decrStr, pattern] = toDecrypt.front();
    toDecrypt.pop();
    queue_mutex.unlock();

    return {decrStr, pattern};
}

#endif //TOWERPRESSUREDECRPT_H

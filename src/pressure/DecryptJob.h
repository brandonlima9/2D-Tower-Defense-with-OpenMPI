#ifndef DECRYPTJOB_H
#define DECRYPTJOB_H
#include <iomanip>
#include <regex>
#include <queue>
#include <openssl/sha.h>
#include <mutex>

#include "../helper/StringUtil.h"

inline std::string decrypt(const int maxCores, const std::string& decrStr, const std::string& pattern, std::queue<std::pair<std::string, std::string>>& output) {
    const int bound = maxCores % 4 == 0 ? std::max(1, maxCores / 4) : static_cast<int>(std::floor(maxCores / 4)) + 1;
    const int limit = std::max(1, static_cast<int>(pattern.size()) - 2) * bound;
    const std::chrono::steady_clock::time_point completionStart = std::chrono::steady_clock::now();
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    std::stringstream ss;
    std::string rStr;

    while (!std::regex_search(ss.str().c_str(), std::regex(pattern))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - completionStart).count();
        if (timeDiff >= limit) {
            job_mutex.lock();
            output.push({ rStr, decrStr });
            job_mutex.unlock();
            return rStr;
        }
        ss.str(std::string());
        rStr = randomString();
        std::string toHash = rStr + decrStr;

        SHA256_Update(&sha256, toHash.c_str(), toHash.size());
        SHA256_Final(hash, &sha256);

        for(const unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }
    }

    printf("Completed Hash: %s\n", ss.str().c_str());
    std::cout << std::flush;

    job_mutex.lock();
    output.push({ rStr, decrStr });
    job_mutex.unlock();
    return rStr;
}

#endif //DECRYPTJOB_H

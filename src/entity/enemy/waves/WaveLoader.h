#ifndef WAVELOADER_H
#define WAVELOADER_H
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "../special_enemy/normal/pink/PinkEnemy.h"
#include "../special_enemy/normal/red/RedEnemy.h"

class Enemy;

struct EnemySpawnInfo {
    int enemyMultiplier{};
    int spawnTimePercent{};
    std::string spawnGap;
    std::shared_ptr<Enemy> enemyToSpawn;
};

struct EnemyWaveInfo {
    int waveCoinReward{};
    float totalWaveTime{};
    std::vector<EnemySpawnInfo> enemyInfos;
};

class WaveLoader {
    std::shared_ptr<EnemySpriteInjector> spriteInjector;


    /*
     *  'waves' variable (map):
     *  > Wave Number (integer)
     *  > Pair
     *      > JSON Object Containing an Array of Enemies
     *      > Pair
     *          > Total Time Wave is meant to Last (float)
     *          > Total Coins Player Earns When Beating the Wave (integer)
     */
    std::map<int, nlohmann::basic_json<>> enemyWaves;
    std::map<int, std::pair<float, int>> wavesInfo;
    int currentWave = 0;
    int maxWaves = 0;

    static std::shared_ptr<Enemy> constructEnemy(const std::shared_ptr<EnemySpriteInjector>& spriteInjector, const std::string &enemyName) {
        if(enemyName == "RED") {
            return std::make_shared<RedEnemy>(RedEnemy(spriteInjector, nullptr));
        }
        if(enemyName == "BLUE") {
            return std::make_shared<BlueEnemy>(BlueEnemy(spriteInjector, nullptr));
        }
        if(enemyName == "GREEN") {
            return std::make_shared<GreenEnemy>(GreenEnemy(spriteInjector, nullptr));
        }
        if(enemyName == "YELLOW") {
            return std::make_shared<YellowEnemy>(YellowEnemy(spriteInjector, nullptr));
        }
        if(enemyName == "PINK") {
            return std::make_shared<PinkEnemy>(PinkEnemy(spriteInjector, nullptr));
        }
        return std::make_shared<RedEnemy>(RedEnemy(spriteInjector, nullptr));
    }

    public:
        explicit WaveLoader(const std::string& file_path) {
            std::ifstream wavesFile(file_path);
            nlohmann::json waveFileJson = nlohmann::json::parse(wavesFile);
            wavesFile.close();
            auto wavesJson = waveFileJson.at("waves");
            auto wavesJsonIterator = wavesJson.items();
            int counter = 0;
            for (const auto& iteration_proxy_value : wavesJsonIterator) {
                auto item = iteration_proxy_value.value();
                auto wave_number = item.value("wave_number", 0);
                auto coin_reward = item.value("reward", 0);
                auto total_wave_time = item.value("total_spawn_time_seconds", .0f);
                auto enemies_json = item.at("enemies");
                enemyWaves.insert(std::pair(wave_number, enemies_json));
                wavesInfo.insert(std::pair(wave_number, std::pair(total_wave_time, coin_reward)));
                counter++;
            }
            currentWave = 0;
            maxWaves = counter;
            spriteInjector = std::make_shared<EnemySpriteInjector>(EnemySpriteInjector());
        }

        EnemyWaveInfo getNextWave() {
            std::vector<EnemySpawnInfo> enemies;
            if (currentWave >= maxWaves) {
                EnemyWaveInfo garbage;
                garbage.waveCoinReward = 0;
                garbage.totalWaveTime = 0;
                garbage.enemyInfos = enemies;
                return garbage;
            }
            currentWave++;
            const auto enemyJsonIterator = enemyWaves.find(currentWave)->second.items();
            const auto totalWaveTime = wavesInfo.find(currentWave)->second.first;
            const auto coinReward = wavesInfo.find(currentWave)->second.second;
            for (const auto& iteration_proxy_value : enemyJsonIterator) {
                const auto& item = iteration_proxy_value.value();
                const auto spawn_time = item.value("spawn_time_percent", 0);
                auto enemy_type = item.value("type", "none");
                const auto enemy_multiplier = item.value("multiplier", 1);
                const auto spawn_gap_type = item.value("spawn_gap", "FAR");
                EnemySpawnInfo newEnemy;
                newEnemy.spawnTimePercent = spawn_time;
                newEnemy.enemyMultiplier = enemy_multiplier;
                newEnemy.spawnGap = spawn_gap_type;
                newEnemy.enemyToSpawn = constructEnemy(spriteInjector, enemy_type);
                enemies.emplace_back(newEnemy);
            }

            EnemyWaveInfo waveInfo;
            waveInfo.waveCoinReward = coinReward;
            waveInfo.totalWaveTime = totalWaveTime;
            waveInfo.enemyInfos = enemies;
            return waveInfo;
        }

        [[nodiscard]] int getMaxWaves() const {
            return maxWaves;
        }

        [[nodiscard]] int getCurrentWave() const {
            return currentWave;
        }
};



#endif //WAVELOADER_H

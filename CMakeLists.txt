cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH} /usr/lib/aarch64-linux-gnu/openmpi/include)
link_directories(/usr/lib/aarch64-linux-gnu/openmpi/lib)
###

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_Declare(
        ssl_content
        URL      https://github.com/janbar/openssl-cmake/archive/refs/tags/1.1.1k-20210430.tar.gz
        URL_HASH MD5=f4294e5de7ebb20f9a695155660f1afd
)
FetchContent_MakeAvailable(SFML json ssl_content)
add_library(OpenSSL::SSL ALIAS ssl)
add_library(OpenSSL::Crypto ALIAS crypto)

add_executable(main src/main.cpp
        src/entity/hit_texture/HitTexture.cpp
        src/entity/enemy/Enemy.cpp
        src/entity/enemy/Enemy.h
        src/entity/defence/tower/Tower.cpp
        src/entity/defence/tower/Tower.h
        src/entity/defence/tower/special_tower/Jo_tower/JoTower.h
        src/entity/defence/tower/special_tower/Jo_tower/JoTower.cpp
        src/entity/defence/projectile/Projectile.cpp
        src/entity/defence/projectile/Projectile.h
        src/helper/CollisionDetect.h
        src/GraphicsManager.cpp
        src/GraphicsManager.h
        src/GameManager.cpp
        src/GameManager.h
        src/entity/enemy/EnemyManager.cpp
        src/entity/enemy/EnemyManager.h
        src/entity/defence/tower/TowerManager.cpp
        src/entity/defence/tower/TowerManager.h
        src/entity/defence/projectile/ProjectileManager.cpp
        src/entity/defence/projectile/ProjectileManager.h
        src/helper/UUID.h
        src/entity/hit_texture/HitTexture.h
        src/entity/enemy/waves/WaveLoader.cpp
        src/entity/enemy/waves/WaveLoader.h
        src/entity/enemy/special_enemy/normal/red/RedEnemy.cpp
        src/entity/enemy/special_enemy/normal/red/RedEnemy.h
        src/entity/enemy/special_enemy/normal/yellow/YellowEnemy.cpp
        src/entity/enemy/special_enemy/normal/yellow/YellowEnemy.h
        src/entity/enemy/special_enemy/normal/pink/PinkEnemy.cpp
        src/entity/enemy/special_enemy/normal/pink/PinkEnemy.h
        src/entity/enemy/special_enemy/normal/green/GreenEnemy.cpp
        src/entity/enemy/special_enemy/normal/green/GreenEnemy.h
        src/entity/enemy/special_enemy/normal/blue/BlueEnemy.cpp
        src/entity/enemy/special_enemy/normal/blue/BlueEnemy.h
        src/entity/enemy/special_enemy/normal/BaseNormalEnemy.cpp
        src/entity/enemy/special_enemy/normal/BaseNormalEnemy.h
        src/entity/enemy/special_enemy/special/armoured/ArmouredEnemy.cpp
        src/entity/enemy/special_enemy/special/armoured/ArmouredEnemy.h
        src/entity/enemy/special_enemy/special/metal/MetalEnemy.cpp
        src/entity/enemy/special_enemy/special/metal/MetalEnemy.h
        src/entity/hit_texture/circle/CircleHitTexture.cpp
        src/entity/hit_texture/circle/CircleHitTexture.h
        src/entity/hit_texture/rectangle/RectangleHitTexture.cpp
        src/entity/hit_texture/rectangle/RectangleHitTexture.h
        src/selection/TowerSelector.cpp
        src/selection/TowerSelector.h
        src/entity/defence/projectile/tower_projectile/basic/BasicProjectile.cpp
        src/entity/defence/projectile/tower_projectile/basic/BasicProjectile.h
        src/entity/defence/projectile/tower_projectile/BaseNormalProjectile.cpp
        src/entity/defence/projectile/tower_projectile/BaseNormalProjectile.h
        src/entity/defence/tower/special_tower/radial_spreader/RadialShooterTower.cpp
        src/entity/defence/tower/special_tower/radial_spreader/RadialShooterTower.h
        src/entity/defence/tower/special_tower/BaseTower.cpp
        src/entity/defence/tower/special_tower/BaseTower.h
        src/entity/defence/projectile/tower_projectile/BaseExplosiveProjectile.cpp
        src/entity/defence/projectile/tower_projectile/BaseExplosiveProjectile.h
        src/entity/defence/projectile/tower_projectile/bomb/BombProjectile.cpp
        src/entity/defence/projectile/tower_projectile/bomb/BombProjectile.h
        src/entity/defence/tower/special_tower/bomb_tower/BombTower.cpp
        src/entity/defence/tower/special_tower/bomb_tower/BombTower.h
        src/pressure/TowerPressureDecrypt.h
        src/helper/visual/FPS.cpp
        src/helper/visual/FPS.h
        src/helper/visual/ThickLine.cpp
        src/helper/visual/ThickLine.h
        src/helper/Accumulator.cpp
        src/helper/Accumulator.h
        src/helper/Digits.h
        src/pressure/DecryptJob.h
        src/helper/StringUtil.h
        src/display/DisplayConsts.h
        src/display/DisplayTextManager.cpp
        src/display/DisplayTextManager.h
        src/display/StaticGraphicsManager.cpp
        src/display/StaticGraphicsManager.h
        src/entity/defence/tower/special_tower/depressure_tower/DepressureTower.cpp
        src/entity/defence/tower/special_tower/depressure_tower/DepressureTower.h
        src/entity/hit_texture/animated_sprite/AnimatedSprite.cpp
        src/entity/hit_texture/animated_sprite/AnimatedSprite.h
        src/entity/hit_texture/circle/animated_circle/AnimCircleHitTexture.cpp
        src/entity/hit_texture/circle/animated_circle/AnimCircleHitTexture.h
        src/texture/EnemySpriteInjector.cpp
        src/texture/EnemySpriteInjector.h
        src/texture/ProjectileSpriteInjector.cpp
        src/texture/ProjectileSpriteInjector.h
        src/entity/hit_texture/rectangle/animated_rectangle/AnimRectangleHitTexture.cpp
        src/entity/hit_texture/rectangle/animated_rectangle/AnimRectangleHitTexture.h
        src/texture/TowerSpriteInjector.cpp
        src/texture/TowerSpriteInjector.h
        src/pressure/job_spawners/SecondaryJobSpawner.h
        src/pressure/job_spawners/MainJobSpawner.h
        src/pressure/job_spawners/PressureJobConstants.h
)

#target_link_libraries(main PRIVATE SFML::Graphics crypto nlohmann_json::nlohmann_json)
target_link_libraries(main PRIVATE SFML::Graphics crypto nlohmann_json::nlohmann_json MPI::MPI_CXX)

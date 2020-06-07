cmake_minimum_required(VERSION 3.12)
project(TomeshStein)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lSDL2")

add_executable(TomeshStein source/main.cpp source/engine/tpge/TPGE.h source/engine/tpge/TPGE.cpp source/player/player.cpp source/player/player.h source/objects/moving/projectile/projectile.cpp source/objects/moving/projectile/projectile.h source/misc/texture/texture.cpp source/misc/texture/texture.h source/items/gun/gun.cpp source/items/gun/gun.h source/objects/moving/enemy/enemy.cpp source/objects/moving/enemy/enemy.h source/misc/path/path.cpp source/misc/path/path.h source/objects/gameobject.cpp source/objects/gameobject.h source/objects/moving/movinggameobject.cpp source/objects/moving/movinggameobject.h source/engine/game/shootergame.cpp source/engine/game/shootergame.h source/objects/container/objectcontainer.cpp source/objects/container/objectcontainer.h source/objects/static/powerup/powerup.cpp source/objects/static/powerup/powerup.h source/engine/utils/rng/rng.h source/engine/menu/menu.cpp source/engine/menu/menu.h)
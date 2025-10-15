cmake_minimum_required(VERSION 4.1.2)
project(AsteroidsGame VERSION 0.4.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED true)

set(SOURCES src/application.cpp src/application.h
  src/managers/inputManager.h src/managers/sources/inputManager.cpp
  src/managers/renderManager.h src/managers/sources/renderManager.cpp
  src/managers/timeManager.h src/managers/sources/timeManager.cpp
  src/managers/entityManager.h src/managers/sources/entityManager.cpp src/managers/sources/entityManager.tpp
  src/math/tools.h src/math/sources/tools.tpp
  src/math/vector2.h src/math/sources/vector2.cpp
  src/ecs/IComponentPool.h src/ecs/componentPool.h src/ecs/componentPool.tpp
  src/ecs/components/abstractions/component.h
  src/ecs/components/transform.h src/ecs/components/sources/transform.cpp
  src/ecs/components/sprite.h src/ecs/components/sources/sprite.cpp
  src/ecs/components/controller.h src/ecs/components/sources/controller.cpp)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

add_executable(AsteroidsGame src/main.cpp ${SOURCES})

configure_file(src/AsteroidsGameConfig.h.in src/AsteroidsGameConfig.h)
target_include_directories(AsteroidsGame PUBLIC "${PROJECT_BINARY_DIR}/src")

# SDL3 basic library
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# SDL3_image library
set(SDLIMAGE_VENDORED ON)
set(SDLIMAGE_AVIF OFF)	# disable formats we don't use to make the build faster and smaller.
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# Link all external libraries
target_link_libraries(AsteroidsGame PRIVATE 
    SDL3_image::SDL3_image
    SDL3::SDL3)

target_compile_features(AsteroidsGame
    PRIVATE cxx_std_20
)

target_compile_options(AsteroidsGame INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
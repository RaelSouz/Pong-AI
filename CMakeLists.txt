cmake_minimum_required(VERSION 3.16)
project(Pong-AI)

set(CMAKE_CXX_STANDARD 17)

# Diretórios de código
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${INCLUDE_DIR}
)

# Usa pkg-config para encontrar SDL2 e SDL2_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)


# Mensagem pós-configuração
message(STATUS "Projeto ${PROJECT_NAME} configurado com sucesso")

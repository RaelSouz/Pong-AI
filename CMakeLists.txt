cmake_minimum_required(VERSION 4.0.3)
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

# REQUISITO: SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

# Mensagem pós-configuração
message(STATUS "Projeto ${PROJECT_NAME} configurado com sucesso")
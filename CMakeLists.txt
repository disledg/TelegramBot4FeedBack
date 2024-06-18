cmake_minimum_required(VERSION 3.10)
project(TelegramBot)

# Пути к локальной копии tgbot-cpp
set(TGBOT_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tgbot-cpp)

# Настройка сборки tgbot-cpp
add_subdirectory(${TGBOT_CPP_DIR})

# Определение исполняемого файла вашего бота
add_executable(TelegramBot src/main.cpp)

# Линковка с библиотекой tgbot-cpp
target_link_libraries(TelegramBot TgBot)

# Включение заголовочных файлов tgbot-cpp
target_include_directories(TelegramBot PRIVATE ${TGBOT_CPP_DIR}/include)

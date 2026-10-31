cmake_minimum_required(VERSION 3.22)
project(SortingNetwork)

# Adiciona a pasta da biblioteca para que o CMake leia o arquivo que você postou
# add_subdirectory(simde)

# Cria o executável usando o seu arquivo que está na pasta src_neon
add_executable(sorting_test src_neon/main.c)

include_directories(simde)

# O comando que o professor pediu, mas usando o "link" 
# que já resolve os diretórios de inclusão automaticamente
# target_link_libraries(sorting_test PRIVATE simde)
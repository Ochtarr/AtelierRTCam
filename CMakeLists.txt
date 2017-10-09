# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)


# Nom du projet

project (stabilization)


find_package (OpenCV REQUIRED)


# Exécutable "hello", compilé à partir du fichier helloworld.c

add_executable (stabilization main.c)


# Hello sera linké avec les bibliothèques d'OpenCV

target_link_libraries (stabilization ${OpenCV_LIBS})

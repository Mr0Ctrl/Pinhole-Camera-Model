cmake_minimum_required(VERSION 3.10)
project(PinholeCameraModel)

# C++ Standardını ayarla
set(CMAKE_CXX_STANDARD 17)

# Kütüphaneleri bul (Eigen ve OpenCV)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(assimp REQUIRED)

# Include klasörünü tanıt
include_directories(include ${EIGEN3_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

# Çalıştırılabilir dosyayı oluştur
add_executable(Pinhole-Camera-Model main.cpp src/CameraModel.cpp src/MeshLoader.cpp src/Renderer.cpp)

# Kütüphaneleri bağla
target_link_libraries(Pinhole-Camera-Model ${OpenCV_LIBRARIES} Eigen3::Eigen)
target_link_libraries(Pinhole-Camera-Model ${OpenCV_LIBRARIES} Eigen3::Eigen assimp)
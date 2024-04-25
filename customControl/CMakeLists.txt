cmake_minimum_required(VERSION 3.5)
project(customControl)

# 搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

# 将当前目录下的源文件制作成动态库
add_library(customControl SHARED ${SRC})

# 链接用到的库文件
target_link_libraries(customControl PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

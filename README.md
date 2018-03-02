1) Install glog
2) Build:

g++ -std=c++11  src/file_loader.cpp src/ilbm_iff.cpp  src/image.cpp src/rle.cpp src/utils.cpp src/window.cpp src/main.cpp -Iinclude -lGL -lGLU -lX11 -lglog -lXxf86vm -o ilbm

3) export GLOG_logtostderr=1

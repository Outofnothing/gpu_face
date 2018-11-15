```
g++ -shared -rdynamic -g -O3 -Wall -fPIC -std=c++11 \
bv.cpp src/bvmodule.cpp \
-DMODULE_STR=bv -DMODULE_PREFIX=pybv \
-DNDEBUG -DPY_MAJOR_VERSION=3 \
`pkg-config --cflags --libs /usr/local/opencv3.4/lib/pkgconfig/opencv.pc`  \
`python3-config --includes --ldflags` \
-I . -I/home/zack/.local/lib/python3.5/site-packages/numpy/core/include \
-o build/bv.so
```
pkg-config 命令依靠.pc文件来配置opencv等库，所以指定.pc文件就可以指定opencv版本。

注意opencv版本一定要是3.4.X
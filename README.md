这是一个生成供python调用的，用C++写成的人脸检测程序。
opencv提供了CUDA的C++接口，却没有python 接口，如果想要在python里使用GPU加速的人脸检测，则必须自己编译shared lib

这个项目参照了[这篇教程][link]。
### 如何编译：
[link]:https://www.learnopencv.com/how-to-convert-your-opencv-c-code-into-a-python-module/
```
python3 gen2.py pybv build headers.txt
```

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
### 注意事项：
- 编译opencv时如果其安装位置不在/usr/local位置，则需要ldconfig来将其链接
   ```
   ldconfig /path/to/your/opencv/lib
   ```
   
- pkg-config 命令依靠.pc文件来配置opencv等库，所以指定.pc文件就可以指定opencv版本。

- 注意opencv版本一定要是3.4.X
- 每次修改源文件后都要再次编译
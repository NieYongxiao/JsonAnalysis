如下图为一典型的json文件格式

{
    "name": "Jack (\"Bee\") Nimble",
    "format" : {
        "type":        "rect",
        "width" :      1920,
        "height" :     1080,
        "interlace" :  false,
        "frame rate" : 24
    }
}

可以想到树状结构：
root->child(name)
           (format)->child(type)
                          (width)
                          (height)
                          (interlace)
                          (frame rate)

其中，同级节点可以使用一个双向链表，将其串联起来

std::string：标准中未规定需要\0作为字符串结尾。编译器在实现时既可以在结尾加\0，也可以不加。（因编译器不同）
但是，当通过c_str()或data()（二者在 C++11 及以后是等价的）来把std::string转换为const char *时，会发现最后一个字符是\0。但是C++11，string字符串都是以'\0'结尾。

if仅当判断值是0的时候才不允许，否则-1也运行


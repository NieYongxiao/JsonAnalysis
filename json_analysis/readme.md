����ͼΪһ���͵�json�ļ���ʽ

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

�����뵽��״�ṹ��
root->child(name)
           (format)->child(type)
                          (width)
                          (height)
                          (interlace)
                          (frame rate)

���У�ͬ���ڵ����ʹ��һ��˫���������䴮������

std::string����׼��δ�涨��Ҫ\0��Ϊ�ַ�����β����������ʵ��ʱ�ȿ����ڽ�β��\0��Ҳ���Բ��ӡ������������ͬ��
���ǣ���ͨ��c_str()��data()�������� C++11 ���Ժ��ǵȼ۵ģ�����std::stringת��Ϊconst char *ʱ���ᷢ�����һ���ַ���\0������C++11��string�ַ���������'\0'��β��

if�����ж�ֵ��0��ʱ��Ų���������-1Ҳ����


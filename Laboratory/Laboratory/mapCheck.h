#include <iostream>
#include <map>

void mapCheck()
{
    // char���L�[�Aint��l�Ƃ��Ĉ����A�z�z��
    std::map<char, int> m;

    // �}��
    m.insert(std::make_pair('a', 10));
    m.insert(std::make_pair('a', 5));       //�����ȏ����ɂȂ��Ă���
    m['a'] += 1;                            //�L���ȏ���
    m['b'] += 1;                            //�f�t�H���g��0�Œl�����������ꂽ

    // ���� : �L�[(char)���w�肵�A�l(int)�𓾂�
    int value = m.at('a');
    std::cout << m['a'] << std::endl;
    std::cout << m['b'] << std::endl;
    std::cout << value << std::endl;
}
#include <iostream>
#include <map>

void mapCheck()
{
    // charをキー、intを値として扱う連想配列
    std::map<char, int> m;

    // 挿入
    m.insert(std::make_pair('a', 10));
    m.insert(std::make_pair('a', 5));       //無効な処理になっていた
    m['a'] += 1;                            //有効な処理
    m['b'] += 1;                            //デフォルトで0で値が初期化された

    // 検索 : キー(char)を指定し、値(int)を得る
    int value = m.at('a');
    std::cout << m['a'] << std::endl;
    std::cout << m['b'] << std::endl;
    std::cout << value << std::endl;
}
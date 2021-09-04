#include <cstdio>

namespace
{
struct Wrap
{
    int value;
};

void swapAndEditVal(Wrap left, Wrap right)
{
    Wrap tmp = left;
    left = right;
    right = tmp;
    tmp.value *= 100;
}
void swapAndEditVal(Wrap* left, Wrap* right)
{
    Wrap* tmp = left;
    left = right;
    right = tmp;
    tmp->value *= 100;
}
void swapAndEditRef(Wrap& left, Wrap& right)
{
    Wrap tmp = left;
    left = right;
    right = tmp;
    tmp.value *= 100;
}
void swapAndEditRef(Wrap*& left, Wrap*& right)
{
    Wrap* tmp = left;
    left = right;
    right = tmp;
    tmp->value *= 100;
}
}

int main(void)
{
    {
        puts("値の値渡し");
        Wrap left = Wrap();
        left.value = 1;
        Wrap right = Wrap();
        right.value = 2;
        swapAndEditVal(left, right);
        printf("%d, %d\n", left.value, right.value);
    }
    {
        puts("参照の値渡し");
        Wrap* left = new Wrap();
        left->value = 1;
        Wrap* right = new Wrap();
        right->value = 2;
        swapAndEditVal(left, right);
        printf("%d, %d\n", left->value, right->value);

        delete left;
        delete right;
    }
    {
        puts("値の参照渡し");
        Wrap left = Wrap();
        left.value = 1;
        Wrap right = Wrap();
        right.value = 2;
        swapAndEditRef(left, right);
        printf("%d, %d\n", left.value, right.value);
    }
    {
        puts("参照の参照渡し");
        Wrap* left = new Wrap();
        left->value = 1;
        Wrap* right = new Wrap();
        right->value = 2;
        swapAndEditRef(left, right);
        printf("%d, %d\n", left->value, right->value);

        delete left;
        delete right;
    }

    return 0;
}

// 実行結果
// 値の値渡し
// 1, 2
// 参照の値渡し
// 100, 2
// 値の参照渡し
// 2, 1
// 参照の参照渡し
// 2, 100

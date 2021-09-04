// https://qiita.com/irxground/items/5b37a1643fb521cf057b

using System;

class Program {
    struct ValWrap { public int value; }
    class RefWrap { public int value; }

    static void swapAndEdit(ValWrap left, ValWrap right) {
        var tmp = left;
        left = right;
        right = tmp;
        tmp.value *= 100;
    }
    static void swapAndEdit(RefWrap left, RefWrap right) {
        var tmp = left;
        left = right;
        right = tmp;
        tmp.value *= 100;
    }
    static void swapAndEdit(ref ValWrap left, ref ValWrap right) {
        var tmp = left;
        left = right;
        right = tmp;
        tmp.value *= 100;
    }
    static void swapAndEdit(ref RefWrap left, ref RefWrap right) {
        var tmp = left;
        left = right;
        right = tmp;
        tmp.value *= 100;
    }

    static void Main() {
        {
            Console.WriteLine("値の値渡し");
            var left = new ValWrap();
            left.value = 1;
            var right = new ValWrap();
            right.value = 2;
            swapAndEdit(left, right);
            Console.WriteLine(left.value + ", " + right.value);
        }
        {
            Console.WriteLine("参照の値渡し");
            var left = new RefWrap();
            left.value = 1;
            var right = new RefWrap();
            right.value = 2;
            swapAndEdit(left, right);
            Console.WriteLine(left.value + ", " + right.value);
        }
        {
            Console.WriteLine("値の参照渡し");
            var left = new ValWrap();
            left.value = 1;
            var right = new ValWrap();
            right.value = 2;
            swapAndEdit(ref left, ref right);
            Console.WriteLine(left.value + ", " + right.value);
        }
        {
            Console.WriteLine("参照の参照渡し");
            var left = new RefWrap();
            left.value = 1;
            var right = new RefWrap();
            right.value = 2;
            swapAndEdit(ref left, ref right);
            Console.WriteLine(left.value + ", " + right.value);
        }
    }
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

## 2026.5.31

マルチテストケース用のコードを追加。
```cpp
#define MULTI_TESTCASE
```
のコメントアウトを解除することでマルチテストケースに対応する仕様にした。

## 2026.6.14

フォーマット設定を[ライブラリ用](../.clang-format)と[本番用](https://github.com/fura2/cp-utils/blob/main/.clang-format)で分けることにした。
main.cpp だけは、本番用の設定に従うべきであって、ライブラリ用に従っていなくてもよいものとした。

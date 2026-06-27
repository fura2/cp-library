## 2026.6.27

アイデアはこちらの記事による。
https://codeforces.com/blog/entry/62393

std::size_t が 32 bit の環境では、ハッシュ値が 32 bit になってしまい期待した挙動にならない。ZobristHashSet で Splitmix64 を std::unordered_set のハッシュ関数として使っており、その要件に std::size_t を返すことが含まれているので仕方なくそうしている。
32 bit 環境で動かすことはほとんどないと思われるのでこれでよいことにする。

# WebAssemblyメモ

## 参考文献

* [Developer's Guide - WebSeembly](https://webassembly.org/getting-started/developers-guide/)
* [WebAssemblyのコンセプト | MDN](https://developer.mozilla.org/ja/docs/WebAssembly/Concepts)
* [C/C++からWebAssemblyにコンパイルする | MDN](https://developer.mozilla.org/ja/docs/WebAssembly/C_to_wasm)
* [WebAssembly: 「なぜ」と「どうやって」](https://dev.to/nabbisen/webassembly--3385)


## WebAssemblyとは

ブラウザ上でバイナリが実行可能。

以下のようなメリットがある。

* JSよりも高速
* asm.jsよりもファイルサイズが小さい（ロード時間が短い）

現状、C, C++, Rust, Goからコンパイル可能。

以下のようなWebAssemblyテキストフォーマットというS式からも変換可能。

````
;2つの整数を足し算する関数addIntをJSから利用可能にする。
(module
  (func $addInt (param $x i32) (param $y i32) (result i32)
    get_local $x
    get_local $y
    i32.add)
  (export "addInt" (func $addInt)))
````

Firefox, Chrome, Safari, Edgeで実行可能。

以下のような用途で実用が期待される(?)

* グラフィック・音声関係の処理、ゲームなど
* 既存アプリの移植

以下、デモ。

* [Tanks! Demo(公式)](https://webassembly.org/demo/)
* [VimをWebAssemblyに移植した](https://rhysd.hatenablog.com/entry/2018/07/09/090115)


## C言語からコンパイルおよび実行

### 必要

* Emscripten


### 実行までの流れ

![EmscriptenでC/C++のソースコードをコンパイルし、WASMモジュールとそれを呼び出すjsコードを生成する](https://mdn.mozillademos.org/files/14647/emscripten-diagram.png)

※ [WebAssemblyのコンセプト | MDN](https://developer.mozilla.org/ja/docs/WebAssembly/Concepts)の画像を流用。

### Cの関数をJSから呼ぶには

EmscriptenのModule.ccall()関数を用いる。
ccallのフォーマットは以下のとおり。

````javascript
var result = Module.ccall('myFunction', // name of C function 
                          null, // return type
                          null, // argument types
                          null); // arguments
````

……が、なんか上手くいかなかった。


### パフォーマンス

以下のたらい回し関数で比較。

````c:tarai.c
/* C */
int tarai(int x, int y, int z){
  if(x <= y){
    return y;
  }
  return tarai(tarai(x-1, y, z), tarai(y-1, z, x), tarai(z-1, x, y));
}
````

````javascript:tarai.js
/* JS */
function tarai(x, y, z){
  if(x <= y){
    return y;
  }
  return tarai(tarai(x-1, y, z), tarai(y-1, z, x), tarai(z-1, x, y));
}
````

tarai(16, 8, 0)で比較。

スペック。

* OS : Xubuntu 18.04 (in VirtualBox)
* CPU : Core i5-5200U 2.20GHz
* メモリ : 4GB

ブラウザ。

* Mozilla Firefox 64.0
* Google Chrome 71.0.3578.98

結果。

* tarai(16, 8, 0) in JS (Firefox) : 1147465ms
* tarai(16, 8, 0) in WASM (Firefox) : 1175544ms
* tarai(16, 8, 0) in JS (Chrome) : 2378083ms
* tarai(16, 8, 0) in WASM (Chrome) : 1338993ms

参考。

* tarai(16, 8, 0) in C : 668317ms

Firefoxだとあまり変わらない。というか、JSの時点で早い。


### 感想など

Cからコンパイルする場合は、WebAssemblyというよりは、ほとんどEmscriptenの機能任せ。

データ型が4種類しかない（32 or 64 ビットの 整数or浮動小数点数）。

EmscriptenがWASMモジュールを自動で呼び出すコードを生成してくれるが、自分で書いたりカスタマイズするのは大変。

DOM操作はJSでやる必要がある。（将来的には、WebAssembly側でできるようになるらしい）

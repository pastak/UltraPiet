# 音ゲーもBotもなんでも出来る最強のPiet「UltraPiet」

## 遊び方
1. git clone https://github.com/Muratam/UltraPiet.git
   (もしくはZipをダウンロード)
2. ./bin/windows/ultrapiet.exe を実行
3. 同梱のtestimageフォルダの画像でテスト出来ますよ
4. 大きな画像も無限ループも無理なく扱えるし、追跡機能も命令履歴機能もあるよ！

## CUIで使う方法(Windows)
../bin/windows/ultrapiet testimage/HelloWorld.png でコマンドラインで実行できます。
Windowsの場合はMinttyなどのターミナルを使って下さい。
コマンドプロンプト及びパワーシェルでは標準入出力が上手く行われません。
### MinGWがよく分からない場合
Pietをやろうとしているということは、あなたはGitをインストールしているはずです。
「Win + R 」で「C:\Program Files\Git\usr\bin\mintty」を入力し、minttyを起動できます。
cd ~ ののち、cd とpwd を駆使してultrapietのあるディレクトリに行き
./ultrapiet HelloWorld.png のようにして実行出来ます。
ultrapietのあるディレクトリにパスを通せばperlやpythonと同じように利用できます


## UltraPietの新機能
- スタックにスタックを積んで木構造を実現できる
- 強力な文字処理機構
- DLLのインポート
- 他のPietのコードの呼び出し

## UltraPietで新たに出来ること

### Webアプリケーションやゲームを簡単に作れる！
旧Pietでは標準入出力になにかをするので手一杯でした.
しかし,UltraPietでは前述のDLLのインポートをサポートしているので,
気軽に素晴らしいプログラミングができます！
更に,他のPietコードを呼び出せるので,
作成した機能はモジュール化することができ,
ライブラリを揃えれば上述のことはたやすく実現できます！

### 高い表現力により自由なコーディングが出来る！
旧Pietでは数字しかスタックに積めませんでした.
そのため,とても表現力が低かったのです.
しかし,UltraPietでは,新たにスタックにスタックを積むことが出来て,
とても表現力豊かなプログラミングが出来ます.
スタックにスタックに,...,スタックに,スタックを積むというふうに
無限にスタックにはスタックを積めるので,
木構造のようなデータ構造も再現できます.
また,新たにスタックに積んだスタックを
文字列とみなすことで高度な文字処理が出来る機構も搭載していて,
ファイル処理や高度な文字処理にも耐えうるコーディングが可能となります.

### 楽しく直感的なプログラミングが出来る！
旧Pietでも楽しくプログラミングが出来たのですが,
コメントの挿入も出来ず,決まった18色しか出来ないなどの成約がありました.
しかし,UltraPietでは専用のIDEでそれらをカバーする予定です.
また,UltraPietのプログラミングで本質的に変わったことは,
新たにスタックが積めるようになったことだけなので,
学習コストも少なくて済みます！
Pietがコードとして利用するPNG画像にはメタデータを挿入できることを利用して,
コメントの挿入,好きな18色でのコーディングなどをサポートし,
より快適なプログラミングを目指します！

## PietとしてのUltraPiet
UltraPietはあくまでもPietです.
色は18色しか扱わないし,
画像のみがあれば誰でも同じコードを再現できます.
これらを守ることで,Cに対するC++のように,
旧Pietのコードもそのまま動かすことが可能となります.


## 記法の定義
今までの項でUltraPietの新たな機能を見ました.
以降の項でUltraPietの具体的な仕様を見ていくことにしましょう.
まずは記法の定義から見ていきましょう.
DirectionPointerをDPと略します.
CodelChooserをCCと略します.

空のスタックに順番に, **1 をPush, 2 をPush, 3 をPush, 4 をPush** した時,
スタックの変化を **[] => [1] => [1,2] => [1,2,3] => [1,2,3,4]** と表記します.
つまり,一番右にあるものが一番最近Pushしたものとなります.

また, **[1,2,3,4]** にAdd命令を行うことを,
**Add [1,2,3,4] => [1,2,7]** と表記できます.
この場合,1,2 は直接演算に関係ないので,
**Add [...,3,4] => [...,7]** のように記すことができます.
さらに演算に関係ない項目 **...** を省略し,以降の文章では
**Add [3,4] => [7]** のように記します.
例えば,Sub[7,2] =>[5] は,7 - 2 = 5 をしたことを意味しますね.

また,UltraPietでは前述のとおり数字とスタックのみで構成されています.
しかし,文字処理のために糖衣構文を定義しておきます.
まず,65 ="A"; 66 ="B";67 ="C";とUnicodeで対応しています.
文字処理のために [65,66,67] と積んだ場合,
その数字が何を指しているかで表現したほうが見やすいので,
[65,66,67] = [A,B,C] と表記しましょう.
さらにもっと省略して,[A,B,C] = "ABC" という糖衣構文も定義します.
UltraPiet では数字とスタックしか使わないということは
大事なことなので忘れないでください.　　

## モジュール呼び出しとスタック型の導入
UltraPietでは新たに２つの命令を導入します.
既存のPietと競合しないように導入しなければなりません.
そこで,普通しない命令である 0除算命令,0剰余命令に
新たな命令としての意味を持たせるという形で導入します.  

#### モジュール呼び出し
**Div [n,0] => []**  
**n.png という名前のPietを読み込んで実行します.**  
現在の標準入力,標準出力,スタックのまま,
そしてDP は 0 ,CC は 1 で初期化されて
そのPietを一番左上の位置から実行します.
終了後,退避されていたDP,CCは元に戻ります.
この命令によりファイルをモジュール化でき,
ファイルの分割が可能になります.
また,言語としての強さの要素である標準ライブラリを搭載し
簡単にこの命令でアクセス出来るように,
nが負の場合は標準ライブラリから探すというふうにする予定です.

#### スタックの新規作成
**Mod [...,x1,x2,...,xn,n,0] => [...,[x1,x2,...,xn]]**  
**深さnまでの要素を積んだ新たなスタックをPushします**  
この関数により,UltraPietのスタックに,
新たなスタックを積むことが出来るようになります  
例： Mod [...,C,H,Y,3,0] => [...,[C,H,Y]]

## スタック演算
今回の拡張によって数字だけでなくスタックも積めるようになりました.
スタック同士の演算を新たに定義しましょう.
スタックとスタック,数字と数字との演算なら問題はないのですが,
スタックと数字が混じっていた場合についてはちょっと考えないといけません.
結論としては,その数字を一つのみを積んだ新たなスタックを作成して,
それを利用してスタックとスタックの演算として扱います.


例えば,加算の関数 Add は,
**Add [...,72,28] => [...,100]** と,数字同士なら演算されますが,
スタック同士に Add を作用させると結合の意味になります.  
例：) **Add [...,[A,B,C],[X,Y]] => [...,[A,B,C,X,Y]]**  
そして,数字とスタックの演算の場合は,
その数字を一つのみを積んだ新たなスタックを作成して,
それを利用してスタックとスタックの演算として扱うので,
**Add [...,C,[H,Y]] => Add [...,[C],[H,Y]] => [...,[C,H,Y]]** と計算されます.

それでは早速命令を見ていきましょう！

### ゼロ引数関数
Push , In(char) , In (number) はスタックとしては定義されません.

### 一引数関数
スタックの先頭から一つとり,それに演算を適用します.

#### Switch: 平坦化する (Flatten)
型 :: [...] => [...]  
スタックにスタックが積まれて,更にそれにスタックが...という状態に
陥いったときにそれを全て平坦化してくれます.
例を見てみますよ.

- Switch [...,[A,[B,C,[D],E],F,G]] => [...,[A,B,C,D,E,F,G]]
- Switch [...,[A,B,C]] => [...,[A,B,C]]
- Switch [...,[A]] => [...,[A]]
- Switch [...,[]] => [...,[]]

#### Point : スタックの先頭を取得する
型 :: [...,T] => [...], T  
スタックの先頭要素を一つとりだしてPushします.例をあげましょう.

- Point [...,[A,B,C]] => [...,[A,B],C]
- Point [...,[A,B,C,[X,Y]]] => [...,[A,B,C],[X,Y]]
- Point [[[Xn,Yn],...,[X1,Y1]]] => [[[Xn,Yn],...,[X2,Y2]],[X1,Y1]]
- Point [...,[A]] => [...,[],A]
- Point [[]] => []

#### Dup: コピーする (Duplicate)
型 :: [...] => [...],[...]
数字版のDupと同じです.例を見せます.

- Dup [...,[[1,3,4],4,32]] => [...,[[1,3,4],4,32],[[1,3,4],4,32]]

#### Not: スタックの中身が空の時 1 を,それ以外では 0 を返す
型 :: [...] => Num  
数字版のNotと意味的には一緒です.
NotとSwitchを組み合わせることで条件分岐に使用出来ますね.

- Not ["Hello"] => [0]
- Not [[A]] => [0]
- Not [[]] => [1]

#### Pop: 捨てる  
型 :: [...] => []  
数字版のPopと一緒です.例を一応載せときます.

- Pop [[A,B,C]] => []

#### Out(Char) : 文字列として出力する
  型 :: [...] => []  
  Switchで平坦化して文字列と見なせるようにしてから出力します.

  - Out(Char) [[C,H,Y]] => [] : "CHY"と出力する
  - Out(Char) [[千,早]] => [] : "千早"と出力する
  - Out(Char) [[C,[H,I,[H,A],Y],A]] => [] : "CHIHAYA"と出力する
  - Out(Char) [[C]] => [] : "C" と出力する.
  - Out(Char) [[]] => [] : 何も出力しない.

#### Out(Num) : スタックの中身を全て取り出し,先頭に要素数を積む
型 :: [Tn,...,T1] => Tn,...,T1,Num  
Out(Num) という名前ですが出力はしませんので注意して下さい.
Mod0命令の逆演算となります.

  - Out(Num) [...,[A,B,C]] => [...,A,B,C,3]
  - Out(Num) [...,[A,[B,C],D]] => [...,A,[B,C],D,3]
  - Out(Num) [...,[A]] => [...,A,1]
  - Out(Num) [...,[]] => [...,0]


#### Div 0: モジュール呼び出し
Div 0 の 被除数が スタックの場合,それをSwitchで平坦化して
文字列にし,その文字列のパスにあるPietモジュールを実行します.


#### Mod 0 : スタックの新規作成
Mod [...,x1,x2,...,xn,n,0] => [...,[x1,x2,...,xn]]  
深さnまでの要素を積んだ新たなスタックをPushします.
x1,...,xn がスタック型であっても大丈夫です.
n がスタックであった場合は n = ∞ として(つまり深さ無限で)演算します.

- Mod [...,C,H,Y,3,0] => [...,[C,H,Y]]
- Mod [...,[C,H,I],[H,A,Y,A],2,0] => [...,[[C,H,I],[H,A,Y,A]] ]
- Mod [...,C,H,Y,[],0] => [[...,C,H,Y]]

### 二引数関数
スタックの先頭から２つとり,それらに演算を適用します.

#### Add: 結合する (Append)  
型 :: [...],[...] => [...]  
単純に結合します.例を示します.
[A,B,C] ="ABC" という糖衣構文を思い出してくださいね.

- Add [...,[C,H,I],[h,a,y,a]] => [...,[C,H,I,h,a,y,a]]
- Add [...,"CHIhaya","isGOD") => [...,"CHIhayaisGOD"]
- Add [...,1,[10,100,1000]] => [...,[1,10,100,1000]]
- Add [...,[1,10,100],1000] => [...,[1,10,100,1000]]
- Add [...,[A,B,C],[]] => [...,[A,B,C]]
- Add [...,[],[]] => [...,[]]

#### Sub: 正規表現で分割する (Split)
型 :: [...],[...] => [...]  
Switch で平坦化し,文字列として見なせるようにしてからSplitします.
[A,B,C] ="ABC" という糖衣構文を思い出してくださいね.

- Sub [...,"Pietはx楽しい","x"]=> [...,["Pietは","楽しい"]]
- Sub ["12/12 21:41"," |/|:"] => [["12","12","21","41"]]
- Sub [...,"helloHWWH!",H] => [...,["hello",[W,W],[!]]]
- Sub [...,[h,[e,H],[l,l,[[o]]]],[H] => [...,["he","llo"]]
- Sub [...,H,"\s"] => [...,[H]]
- Sub [...,H,"H"] => [...,[]]

#### Mul: 直積集合を取得する
型 :: [...],[...] => [...]  
便利な未来を作る直積集合ですよ.

- Mul [ [A,B,C],[X,Y] ] => [ [[A,X],[A,Y],[B,X],[B,Y],[C,X],[C,Y]] ]
- Mul [...,H,[J,I]] => [..., [ [H,J],[H,I] ] ]
- Mul [...,[A],H] => [...,[A,H]]
- Mul [...,[],A]=> [...,[]]
- Mul [[A,B,C],[[X,Y]]] => [[A,[X,Y]],[B,[X,Y]],[C,[X,Y]]]
- これにより,例えば 0 < n,m ≦ 20 の n * m の取りうる値を列挙できます
  1. 1~20までと20と0を順に積む => [1,...,20,20,0]
  1. Mod0 を実行してスタック化 => [[1,...,20]]
  1. Dupでコピーを作成 => [[1,...,20],[1,...20]]
  1. Mul で直積を取る => [[[1,1],...,[20,20]]]
  1. Point で先頭の要素を取得 => [[[1,1],...,[20,19]],[20,20]]
  1. Out(N)で分解する => [[[1,1],...,[20,19]],20,20,2]
  1. 2 を Pop => [[[1,1],...,[20,19]],20,20]
  1. Mulで掛け算する => [[[1,1],...,[20,19]],400]
  1. Roll 2 1 で入れ替える => [400,[[1,1],...,[20,19]]]
  1. 5~9 を繰り返す.
  1. [400,...,1] が出来上がる.

#### Div: 正規表現でマッチさせる (Match)  
型 :: [...],[...] => [...]  
Switch で平坦化し,文字列として見なせるようにしてからMatchします.
[A,B,C] ="ABC" という糖衣構文を思い出してくださいね.

- Div ["aa9bb1cc",".[1-9]."] => [["a9b","b1c","aa9bb1cc"]]
- Div ["aa",".[1-9]."] => [[]]
- Div ["aHxH",H]=> [["H","H","aHxH"]]
- Div [H,"h|H"] => [["H","H"]]
- Div [[],"A|B"] => [[]]
- Div ["ABC",[]] => [[]]


#### Mod: スタックを順番に結合する (Combine)  
型 :: [...],[...] => [...]  
それぞれの要素を結合します。Mulと上手く組み合わせてご使用下さい。

- Mod ["01234","abc"] => [["2a","3b","4c"]]
- Mod [[a,b,c,d,e],[x,y]] => [[[d,x],[e,y]]]
- Mod [[[a,b,c],[d,e]],[x,y]] => [[[[a,b,c],x],[[d,e],y]]]
- Mod [H,"abc"]=> [[H,c]]
- Mod ["abc",H]=> [[c,H]]
- Mod ["abc",[]]=> [[]]
- Mod [[],[]]=> [[]]


#### Greater: ファイルを読み書きする
型 :: [...],[...] => Num  
ファイルを開いて標準入出力へリダイレクトします.
成功したら 1 ,失敗したら 0 が積まれます.
第一引数は開くファイル名です.
第二引数は開くモードを指定します.  
**モード**

- [R] or [r] or [0] : 読み込み(Read)モードで開く.
- [W] or [w] or [1] : 書き込み(Write)モードで開く.
- [A] or [a] or [2] : 追加書き込み(Append)モードで開く
- [CR] or [cr] or [3] : Readで開いたファイルを閉じる.
- [Cw] or [cw] or [4] : Writeで開いたファイルを閉じる.
- その他 : 現在開いているファイルをRead,Writeともに閉じる.

新たにファイルを開いた場合,現在開いているファイルは閉じられます.
例を示します.

  1. Greater ["f1.txt",r] => [1] : f1.txtを開き標準入力をリダイレクトする
  1. Greater ["f2.txt",1] => [1] : f2.txtを開き標準出力をリダイレクトする
  1. Greater ["f.t",r] => [0] : f.tが開けなかった.
  1. Greater ["f3.txt",a] => [1]
  : 2.で開いていたf2.txtを閉じて新たにf3.txtを
  追加書き込みモードで開き,標準出力をリダイレクトする.
  1. Greater [[],[cr]] => [0] : 1.で開いていたf1.txtを閉じる.


### 四引数関数
### Roll: Dllを読み込む  
型 :: [...],[...],[...],[...] => Num or [...] or Empty  
Pietが何でも出来るようになる魔法の関数です.
この関数により,ゲームやアプリを作ることが出来るようになります.
実用的なマルチメディアな操作をPietで体感しよう！  
**引数**  
第一引数 : DLLへのファイル名を含むパス  
第二引数 : DLL内の関数名  
第三引数 : DLLに渡される型の宣言  
第四引数 : DLLを呼び出すための引数リスト  
**例**  

- Roll ["user32.dll","MessageBoxA", "vssui",
[[],"Hello World.","Caption",0]]  
C製Dllであるuser32.dllの,MessageBoxA関数を呼び出します.
"vssui"というのは的確にDLLを扱う為の型情報の宣言です.
引数の型は前から順に void*(HANDLE),char*(LPSTR),
char*(LPSTR),unsigned int (UINT) で,
返り値はint型にするという宣言で,
前から順にvoid*,char*,char*,unsigned int,intなので
頭文字をとって "vssui"です.
- Roll ["TestDLL.dll","TestDll.CsDll.LangA",0,[123,"RA"]]  
自作.Net製DllでTestDll.CsDll.LangA(123,"RA")を実行します.
第二引数は NAMESPACE.CLASSNAME.STATICFUNCTIONNAME とします.
.Net製Dllは型情報が内蔵されているために,
型を推論することができ,第三引数は省略できます.
　というか,型を書いても無視されます.
C製Dllで扱えるプリミティブな型以外はサポートしません.

## 実装

ここまで,仕様を詳細に示してきました.
あとは実装するだけですね！
スクリプト言語としてのPietとなって,
Perl,PHP,Python,UltraPiet の 4Pとなることを願っています.
このUltraPietプロジェクトは,ここまでの基本仕様が出来た時点で公開します.
オープンソースプロジェクトとして,
公開後は標準ライブラリの実装のContributeをお待ちしております！
最新情報は Github : Muratam, Twitter : paradigm_9 を参照下さい！

UltraPietは、Qtフリーライセンス、すなわちLGPLに従う範囲でオープンソースで提供のこと。

## 色の問題
Piet標準色はチカチカするものばかり…
しかも特に青系の色など,違いがわかりにくいものもあります.
UltraPietでは,自分で18色を定義できます.
とはいえ,好き勝手定義しては読みにくいですので,指針を.
色相(H)の順序付けが出来て,明度(彩度)の順序付けができればいいですよね.
6色なので,色相は基本的には60度程度違えばよいですね.
色を見分けられるように,30度以上の差を全てが保つ色にしましょう.
明度と彩度でSVの三角形は,色が分かるように,
0.5 < V < 1 ,0.2 < S <= 1であるという条件のもと,
V,Sを変えればすきな組み合わせに出来ますね.

## UltraPietとしての情報の保存方法
UltraPiet(Pietも！)が実行するコードはPNGファイルです.
実はPNGにはメタデータを入れることが出来ます.
UltraPietのIDEではコメントを挿入することが出来ます.
また,前述のとおり18色の指定も出来ます.
そのほかにも,IDEとして保存しておきたい情報は数多くあります.
それも全てこのメタデータに保存すればよいですね.


##仕様追記
コーデルサイズは1固定?(関数呼び出しの際の混乱を避けるため)
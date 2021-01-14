# 画像処理

## 色調変換

- 実行時第1引数として指定したP6形式のカラー画像ファイル（拡張子は「.ppm」）を，以下の5種類のP5形式グレイスケール画像（拡張子は「.pgm」）に変換して出力するプログラムを作成せよ．<br>

1. カラー画像の赤だけを取り出し，出力
2. カラー画像の緑だけを取り出し，出力
3. カラー画像の青だけを取り出し，出力
4. 赤，緑，青の平均を求め，出力
5. 輝度Yを求め，出力

[色調変換](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw01)

<br>

## 縮小・拡大・回転

- P5形式256×256画素のグレイスケール画像を平均操作法を用いて128×128画素の画像に縮小するプログラムを作成せよ．
- P5形式256×256画素のグレイスケール画像を直線補間法を用いて512×512画素の画像へ拡大するプログラムを作成せよ．
- P5形式256×256画素のグレイスケール画像を30度回転させるプログラムを作成せよ．ただし，出力される画像の大きさは，回転させた画像がちょうど入る大きさとし，回転させた画像全体を表示するよう平行移動すること．

[縮小・拡大・回転](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw02)

<br>

## ヒストグラム・ヒストグラム平滑化

- P5形式グレイスケール画像の濃度度数分布表を出力するプログラムを作成せよ．また，グラフ作成ツールを利用してヒストグラムを作成せよ．
- P5形式256階調のグレイスケール画像について，目標階調数を64とした場合のヒストグラム平滑化を行うプログラムを作成せよ．

[ヒストグラム・ヒストグラム平滑化](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw03)

<br>

## ノイズ除去・鮮鋭化

- P5形式グレイスケールのノイズ画像について，メディアンフィルタを用いてノイズ除去を行うプログラムを作成せよ．
- P5形式グレイスケール画像について，8近傍ラプラシアンフィルタを用いて鮮鋭化を行うプログラムを作成せよ．

[ノイズ除去・鮮鋭化](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw04)

<br>

## 二値化・階調変換・ディザ

- 128をしきい値とした場合，画像全体の濃度の平均値をしきい値とした場合，判別分析法のいずれか二つを用い，P5形式グレイスケール画像を2値化するプログラムを作成せよ．ただし，画像形式はpgm形式で出力するものとし，白は濃度値255，黒は濃度値0で表現するものとする．
- Bayerマトリクスを用いて，グレイスケール画像からディザ画像を生成するプログラムを作成せよ．

[二値化・階調変換・ディザ](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw05)

<br>

## フィルタ処理

- P5 形式グレイスケールに変換した画像「Lenna」と画像「Airplane」に対して，SobelフィルタとLaplacianフィルタをそれぞれ適用したときの出力画像はどのような結果となるか．なお，Sobelフィルタについては，√((水平方向 * 2) + (垂直方向 * 2))として画素値を求めること．また，Laplacianフィルタについては，負の値が出ないよう，画素値の絶対値をとったものを出力画像とすること．

[フィルタ処理](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw06)

<br>

## 標本化と量子化

- P5 形式グレイスケールに変換した画像「Lenna」と画像「Airplane」に対して，線形量子化を行うことで 4[bit/pixel] の画像を作成せよ．このとき，輝度の最大値と最小値が，元の画像と変わらないように注意する．
- 入力画像と出力画像の間のPSNR値も求めよ．

[標本化と量子化](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw07)

<br>

## HSVによる画質の改善

- 低解像度のカラー画像と高解像度のグレースケール画像を利用して，高解像度のカラー画像を生成するプログラムを作成せよ．

[HSVによる画質の改善](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw08)

<br>

## アフィン変換による画像の幾何学的歪みの除去

- 衛星写真に含まれている歪みを（順変換で）修正し，多時期の衛星写真（幾何学的歪みを補正済のもの）とオーバーレイできるようにする．多時期の衛星画像を比較できることによって，環境モニタリングや土地利用の変化が容易になる．
  - 補完方法：最近隣内挿法（ニアレストネイバー法）
  - 入力画像と目的画像から基準点（同一点）を8点以上求めること．
  - パラメータ a ～ f をExcelで求めること．

[アフィン変換による画像の幾何学的歪みの除去](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw09)

<br>

## 画像ラベリング

- グレースケール画像を二値化し，再帰関数でラベリング処理した結果を出力せよ．（8連結・最大ラベル数32）
- 各ラベルが指定された画像領域において個別に色付けせよ．

[画像ラベリング](https://github.com/Deteikepeperon/ImageProcessing/tree/master/hw10)



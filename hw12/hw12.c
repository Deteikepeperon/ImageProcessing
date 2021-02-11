#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIMENSION 2  // データ次元数
#define MAXDATA 500  // 最大データ数

void get_data(char *filename, double coordinate[][DIMENSION], int *column, int *row);
void save_data(char *filename, double coordinate[][DIMENSION], double output[], int *column, int *row);
void plot_data(char *filename);
void plot_data_before(char *filename);
void simple_perceptron(double coordinate[][DIMENSION], double output[], double weight[DIMENSION + 1], int n);
void multilayer_perceptron(double coordinate[][DIMENSION], double output[], double weight_or[DIMENSION + 1], double weight_and[DIMENSION + 1], int n);
double f_step(double u);


int main(int argc, char *argv[])
{
  int column;                                         // 各dataの列数
  int row1, row2, row3;                               // 各dataの行数
  double coordinate1[MAXDATA][DIMENSION];             // data01の座標
  double coordinate2[MAXDATA][DIMENSION];             // data02の座標
  double coordinate3[MAXDATA][DIMENSION];             // data03の座標
  double category1[MAXDATA];                          // data01のカテゴリ
  double category2[MAXDATA];                          // data02のカテゴリ
  double category3[MAXDATA];                          // data03のカテゴリ
  double weight_or[DIMENSION + 1]  = { 1,  1, 0.5 };  // OR分度器の重み
  double weight_and[DIMENSION + 1] = { 1,  1, 1.2 };  // AND分度器の重み
  double weight_xor[DIMENSION + 1] = { 1, -1, 0.5 };  // XOR分度器の重み


  // データを読み込み
  get_data("txt/data01.txt", coordinate1, &column, &row1);
  get_data("txt/data02.txt", coordinate2, &column, &row2);
  get_data("txt/data03.txt", coordinate3, &column, &row3);

  // 課題1：OR分度器の作成
  simple_perceptron(coordinate1, category1, weight_or, row1);
  save_data("converted_txt/data01_or.txt", coordinate1, category1, &column, &row1);

  simple_perceptron(coordinate2, category2, weight_or, row2);
  save_data("converted_txt/data02_or.txt", coordinate2, category2, &column, &row2);

  simple_perceptron(coordinate3, category3, weight_or, row3);
  save_data("converted_txt/data03_or.txt", coordinate3, category3, &column, &row3);

  // 課題1：AND分度器の作成
  simple_perceptron(coordinate1, category1, weight_and, row1);
  save_data("converted_txt/data01_and.txt", coordinate1, category1, &column, &row1);

  simple_perceptron(coordinate2, category2, weight_and, row2);
  save_data("converted_txt/data02_and.txt", coordinate2, category2, &column, &row2);

  simple_perceptron(coordinate3, category3, weight_and, row3);
  save_data("converted_txt/data03_and.txt", coordinate3, category3, &column, &row3);

  // 課題2：XOR分度器の作成
  multilayer_perceptron(coordinate1, category1, weight_or, weight_and, row1);
  save_data("converted_txt/data01_xor.txt", coordinate1, category1, &column, &row1);

  multilayer_perceptron(coordinate2, category2, weight_or, weight_and, row2);
  save_data("converted_txt/data02_xor.txt", coordinate2, category2, &column, &row2);

  multilayer_perceptron(coordinate3, category3, weight_or, weight_and, row3);
  save_data("converted_txt/data03_xor.txt", coordinate3, category3, &column, &row3);

  // 分類前のデータをプロット
  plot_data_before("txt/data01.txt");
  plot_data_before("txt/data02.txt");
  plot_data_before("txt/data03.txt");

  // OR分度器で作成した分類後のデータをプロット
  plot_data("converted_txt/data01_or.txt");
  plot_data("converted_txt/data02_or.txt");
  plot_data("converted_txt/data03_or.txt");

  // AND分度器で作成した分類後のデータをプロット
  plot_data("converted_txt/data01_and.txt");
  plot_data("converted_txt/data02_and.txt");
  plot_data("converted_txt/data03_and.txt");

  // XOR分度器で作成した分類後のデータをプロット
  plot_data("converted_txt/data01_xor.txt");
  plot_data("converted_txt/data02_xor.txt");
  plot_data("converted_txt/data03_xor.txt");


  return 0;
}


// 単純パーセプトロン（引数：データの座標，データのカテゴリ，重み，行数）
void simple_perceptron(double coordinate[][DIMENSION], double category[], double weight[DIMENSION + 1], int n)
{
  double u;

  for (int i = 0; i < n; i++) {
    u = 0;

    // 入力に重みを掛け、閾値を引く
    for (int j = 0; j < DIMENSION; j++) {
      u += coordinate[i][j] * weight[j];
    }
    u -= weight[DIMENSION];

    // ステップ関数に渡した u を出力
    category[i] = f_step(u);
  }
}


// 多層パーセプトロン（引数：データの座標，データのカテゴリ，OR分度器の重み，AND分度器の重み，行数）
void multilayer_perceptron(double coordinate[][DIMENSION], double category[], double weight_or[DIMENSION + 1], double weight_and[DIMENSION + 1], int n)
{
  double u;
  double output_or[MAXDATA];
  double output_and[MAXDATA];

  simple_perceptron(coordinate, output_or, weight_or, n);
  simple_perceptron(coordinate, output_and, weight_and, n);

  for (int i = 0; i < n; i++) {
    u = 0;

    // OR分度器とAND分度器を使ってXOR分度器を作成する
    u = output_or[i] * 1 + output_and[i] * (-1) - 0.5;

    // ステップ関数に渡した u を出力
    category[i] = f_step(u);
  }
}


// ステップ関数（活性化関数）
double f_step(double u)
{
  if (u >= 0) return 1.0;
  else return 0.0;
}


// データを読み込む（引数：受け取ったファイルデータ，座標情報，列数，行数）
void get_data(char *filename, double coordinate[][DIMENSION], int *column, int *row)
{
  FILE *fp = fopen(filename, "r");
  char readline[256];

  int n = 0;

  // 一行ずつ処理
  while (fgets(readline, sizeof(readline), fp) != NULL) {
    // 文字列を分解
    char *token = strtok(readline, " ");
    int m = 0;

    // 一文字ずつ処理
    while (token != NULL) {
      // 最初の2つを座標情報としてdouble型に変換
      coordinate[n][m] = atof(token);
      // 次の文字へ移動
      token = strtok(NULL, " ");
      m++;
    }

    n++;
  }

  fclose(fp);

  *column = 2;
  *row = n;
}


// データを保存（引数：受け取ったファイルデータ，座標情報，出力データ，列数，行数）
void save_data(char *filename, double coordinate[][DIMENSION], double output[], int *column, int *row)
{
  FILE *fp = fopen(filename, "w");

  for (int i = 0; i < *row; i++) {
    for (int j = 0; j < *column; j++) {
      fprintf(fp, "%.4f ", coordinate[i][j]);
    }
    fprintf(fp, "%.4f\n", output[i]);
  }

  fclose(fp);
}


// gnuplotで分類後のデータをプロット
void plot_data(char *filename)
{
  FILE *gp = popen("gnuplot -persist", "w");

  fprintf(gp, "reset\n");
  fprintf(gp, "unset label\n");
  fprintf(gp, "unset key\n");
  fprintf(gp, "unset grid\n");
  fprintf(gp, "set title \"%s\"\n", filename);
  fprintf(gp, "set xlabel \"%s\"\n", "INPUT X1");
  fprintf(gp, "set ylabel \"%s\"\n", "INPUT X2");
  fprintf(gp, "set xrange [0:1]\n");
  fprintf(gp, "set yrange [0:1]\n");
  fprintf(gp, "set terminal qt size 700, 400 font \"Verdana, 10\"\n");
  fprintf(gp, "set size ratio - 1\n");
  fprintf(gp, "set palette rgbformulae 22, 13, -31\n");
  fprintf(gp, "set pm3d map # no interpolation\n");
  fprintf(gp, "plot \"%s\" using 1:2:3 with points palette pointsize 3 pointtype 7\n", filename);

  fflush(gp);
  fprintf(gp, "exit\n");

  pclose(gp);
}


// gnuplotで分類前のデータをプロット
void plot_data_before(char *filename)
{
  FILE *gp = popen("gnuplot -persist", "w");

  fprintf(gp, "reset\n");
  fprintf(gp, "unset label\n");
  fprintf(gp, "unset key\n");
  fprintf(gp, "unset grid\n");
  fprintf(gp, "set title \"%s\"\n", filename);
  fprintf(gp, "set xlabel \"%s\"\n", "INPUT X1");
  fprintf(gp, "set ylabel \"%s\"\n", "INPUT X2");
  fprintf(gp, "set xrange [0:1]\n");
  fprintf(gp, "set yrange [0:1]\n");
  fprintf(gp, "set terminal qt size 700, 400 font \"Verdana, 10\"\n");
  fprintf(gp, "set size ratio - 1\n");
  fprintf(gp, "set palette rgbformulae 22, 13, -31\n");
  fprintf(gp, "set pm3d map # no interpolation\n");
  fprintf(gp, "plot \"%s\" using 1:2 with p ps 3 pt 7\n", filename);

  fflush(gp);
  fprintf(gp, "exit\n");

  pclose(gp);
}
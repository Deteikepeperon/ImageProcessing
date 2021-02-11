#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define DIMENSION 2  // データ次元数
#define MAXDATA 500  // 最大データ数

void nearest_neighbor(double coordinate1[][DIMENSION], double coordinate2[][DIMENSION], double category1[], double category2[], int row1, int row2);
void k_nearest_neighbor(double coordinate1[][DIMENSION], double coordinate2[][DIMENSION], double category1[], double category2[], int row1, int row2, int K);
void k_means(double coordinate[][DIMENSION], double category[], int row, int cluster);
void get_data(char *filename, double coordinate[][DIMENSION], double category[], int *column, int *row);
void save_data(char *filename, double coordinate[][DIMENSION], double category[], int *column, int *row);
void plot_data(char *filename);


int main(int argc, char *argv[])
{
  int column;                              // 各dataの列数
  int row1, row2, row3, row4;              // 各dataの行数
  double coordinate1[MAXDATA][DIMENSION];  // data_01の座標
  double coordinate2[MAXDATA][DIMENSION];  // data_02の座標
  double coordinate3[MAXDATA][DIMENSION];  // data_03の座標
  double coordinate4[MAXDATA][DIMENSION];  // data_04の座標
  double category1[MAXDATA];               // data_01のカテゴリ
  double category2[MAXDATA];               // data_02のカテゴリ
  double category3[MAXDATA];               // data_03のカテゴリ
  double category4[MAXDATA];               // data_04のカテゴリ


  // 未分類データを読み込み
  get_data("txt/data01.txt", coordinate1, category1, &column, &row1);
  get_data("txt/data02.txt", coordinate2, category2, &column, &row2);
  get_data("txt/data03.txt", coordinate3, category3, &column, &row3);
  get_data("txt/data04.txt", coordinate4, category4, &column, &row4);

  // 課題1：最近傍法を実装 & クラスタリング後のデータを保存
  nearest_neighbor(coordinate1, coordinate2, category1, category2, row1, row2);
  save_data("converted_txt/data02_nearest.txt", coordinate2, category2, &column, &row2);

  // 課題2：k-最近傍法を実装（k値：奇数と偶数） & クラスタリング後のデータを保存
  k_nearest_neighbor(coordinate1, coordinate2, category1, category2, row1, row2, 5);
  save_data("converted_txt/data02_k_nearest_odd.txt", coordinate2, category2, &column, &row2);

  k_nearest_neighbor(coordinate1, coordinate2, category1, category2, row1, row2, 6);
  save_data("converted_txt/data02_k_nearest_even.txt", coordinate2, category2, &column, &row2);

  // 課題3：k-平均法を実装 & クラスタリング後のデータを保存
  k_means(coordinate3, category3, row3, 2);
  save_data("converted_txt/data03_k_means.txt", coordinate3, category3, &column, &row3);

  k_means(coordinate4, category4, row4, 2);
  save_data("converted_txt/data04_k_means.txt", coordinate4, category4, &column, &row4);

  // 分類前のデータをプロット
  plot_data("txt/data02.txt");
  plot_data("txt/data03.txt");
  plot_data("txt/data04.txt");

  // 最近傍法によるクラスタリング結果をプロット
  plot_data("converted_txt/data02_nearest.txt");

  // k-最近傍法によるクラスタリング結果をプロット
  plot_data("converted_txt/data02_k_nearest_odd.txt");
  plot_data("converted_txt/data02_k_nearest_even.txt");

  // k-平均法によるクラスタリング結果をプロット
  plot_data("converted_txt/data03_k_means.txt");
  plot_data("converted_txt/data04_k_means.txt");


  return 0;
}


// 最近傍法（引数：分類済みデータの座標，未分類データの座標，分類済みデータのカテゴリ，未分類データのカテゴリ，分類済みデータの行数，未分類データの行数）
void nearest_neighbor(double coordinate1[][DIMENSION], double coordinate2[][DIMENSION], double category1[], double category2[], int row1, int row2)
{
  long long init = LLONG_MAX;  // 初期値をlong long型の最大値に設定


  for (int i = 0; i < row2; i++) {
    double x1 = coordinate2[i][0];
    double y1 = coordinate2[i][1];


    for (int j = 0; j < row1; j++) {
      double x2 = coordinate1[j][0];
      double y2 = coordinate1[j][1];

      // 所属するクラスの中で最も近いデータのユークリッド距離を測定
      double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

      // 測定した距離が初期値より小さい場合，初期値を更新し，最短距離をカテゴリに代入
      if (init > distance) {
        init = distance;
        category2[i] = category1[j];
      }
    }
  }
}


// k-最近傍法（引数：分類済みデータの座標，未分類データの座標，分類済みデータのカテゴリ，未分類データのカテゴリ，分類済みデータの行数，未分類データの行数，インスタンス数）
void k_nearest_neighbor(double coordinate1[][DIMENSION], double coordinate2[][DIMENSION], double category1[], double category2[], int row1, int row2, int K)
{
  int cluster_A = 0;           // クラスタAに分類された数
  int cluster_B = 0;           // クラスタBに分類された数
  int instance[row1];          // 近傍に登録されたK個のインスタンス
  double distance[row1];       // データ同士の距離
  long long init = LLONG_MAX;  // 初期値をlong long型の最大値に設定


  for (int i = 0; i < row2; i++) {
    double x1 = coordinate2[i][0];
    double y1 = coordinate2[i][1];

    for (int j = 0; j < row1; j++) {
      double x2 = coordinate1[j][0];
      double y2 = coordinate1[j][1];

      // 所属するクラスの中で最も近いデータのユークリッド距離を測定
      distance[j] = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

      // 測定した距離が初期値より小さい場合，初期値を更新し，最短距離をカテゴリに代入
      if (init > distance[j]) {
        init = distance[j];
        category2[i] = category1[j];
      }

      // インスタンスを記録
      instance[j] = j;
    }

    // インスタンスをバブルソート（昇順）
    for (int k = 0; k < row1; k++) {
      for (int l = k; l < row1 + 1; l++) {

        if (distance[k] > distance[l]) {
          int tmp = instance[k];
          instance[k] = instance[l];
          instance[l] = tmp;
        }
      }
    }

    // 距離が短い順に並べたK個のインスタンスをカウントし，クラスタに分類
    for (int j = 0; j < K; j++) category2[instance[K]] == 1 ? cluster_A++ : cluster_B++;
  }
}


// k-平均法（引数：分類済みデータの座標，分類済みデータのカテゴリ，分類済みデータの行数，クラスタ数）
void k_means(double coordinate[][DIMENSION], double category[], int row, int cluster)
{
  double init_gxA;
  double init_gyA;
  double init_gxB;
  double init_gyB;
  double distance_A;
  double distance_B;


  // クラスタA， クラスタBの重心(gx, gy)の初期値を適当に与える
  double gxA = 1;
  double gyA = 2;
  double gxB = 3;
  double gyB = 4;

  // 重心が変化しなくなるまで繰り返し
  while (init_gxA != gxA || init_gyA != gyA || init_gxB != gxB || init_gyB != gyB) {

    // クラスタAとクラスタBに分類されるインスタンスの数
    int cluster_A = 0;
    int cluster_B = 0;

    // クラスタAとクラスタBの重心
    double gravity_gxA = 0;
    double gravity_gyA = 0;
    double gravity_gxB = 0;
    double gravity_gyB = 0;

    // 重心の初期値を記録
    init_gxA = gxA;
    init_gyA = gyA;
    init_gxB = gxB;
    init_gyB = gyB;

    for (int i = 0; i < row; i++) {

      // 各点の座標を記録
      double x = coordinate[i][0];
      double y = coordinate[i][1];

      // 重心と各点のユークリッド距離を計算
      distance_A = sqrt(pow(gxA - x, 2) + pow(gyA - y, 2));
      distance_B = sqrt(pow(gxB - x, 2) + pow(gyB - y, 2));

      // カテゴリをつける
      if (distance_A < distance_B) category[i] = 1;
      else category[i] = 2;
    }

    for (int i = 0; i < row; i++) {

      // 重心をもとに各クラスタに分類
      if (category[i] == 1) {
        gravity_gxA += coordinate[i][0];
        gravity_gyA += coordinate[i][1];
        cluster_A++;
      } else {
        gravity_gxB += coordinate[i][0];
        gravity_gyB += coordinate[i][1];
        cluster_B++;
      }
    }

    // 重心を求める
    gxA = gravity_gxA / cluster_A;
    gyA = gravity_gyA / cluster_A;
    gxB = gravity_gxB / cluster_B;
    gyB = gravity_gyB / cluster_B;
  }
}


// データを読み込む（引数：受け取ったファイルデータ，座標情報，カテゴリ，列数，行数）
void get_data(char *filename, double coordinate[][DIMENSION], double category[], int *column, int *row)
{
  FILE *fp = fopen(filename, "r");;
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
      if (m < 2) coordinate[n][m] = atof(token);
      // 残りの1つをカテゴリとしてint型に変換
      else category[n] = atof(token);

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


// データを保存（引数：受け取ったファイルデータ，座標情報，カテゴリ，列数，行数）
void save_data(char *filename, double coordinate[][DIMENSION], double category[], int *column, int *row)
{
  FILE *fp = fopen(filename, "w");

  for (int i = 0; i < *row; i++) {
    for (int j = 0; j < *column; j++) {
      fprintf(fp, "%.4f ", coordinate[i][j]);
    }
    fprintf(fp, "%.4f\n", category[i]);
  }

  fclose(fp);
}


// gnuplotでクラスタリング結果をプロット
void plot_data(char *filename)
{
  FILE *gp = popen("gnuplot -persist", "w");

  fprintf(gp, "reset\n");
  fprintf(gp, "unset label\n");
  fprintf(gp, "unset key\n");
  fprintf(gp, "unset grid\n");
  fprintf(gp, "set title \"%s\"\n", filename);
  fprintf(gp, "set xlabel \"%s\"\n", "x");
  fprintf(gp, "set ylabel \"%s\"\n", "y");
  fprintf(gp, "set terminal qt size 700, 400 font \"Verdana, 10\"\n");
  fprintf(gp, "set size ratio - 1\n");
  fprintf(gp, "set palette rgbformulae 22, 13, -31\n");
  fprintf(gp, "set pm3d map # no interpolation\n");
  fprintf(gp, "plot \"%s\" using 1:2:3 with points palette pointsize 1 pointtype 7\n", filename);

  fflush(gp);
  fprintf(gp, "exit\n");

  pclose(gp);
}

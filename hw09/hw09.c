#include <stdio.h>
#include <stdlib.h>

void affine(FILE *fp, int width, int width2, int height, int height2, int maxdepth, int maxdepth2, unsigned char gray[], unsigned char gray2[]);


int main(int argc, char *argv[])
{
  FILE *fp, *fp2;
  int width, width2, height, height2, maxdepth, maxdepth2;
  char buf[128];
  unsigned char *gray, *gray2;

  // 第一引数に入力画像 (destination.pgm)，第二引数に出力画像 (source.pgm)を指定する．
  if (argc != 3)                             goto ARGUMENT_ERR;
  if ((fp = fopen(argv[1], "rb")) == NULL)   goto FILE_ERR;
  if ((fp2 = fopen(argv[2], "rb")) == NULL)  goto FILE_ERR;

  if ((gray = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)     goto MEMORY_ERR;
  if ((gray2 = (unsigned char *)malloc(sizeof(unsigned char) * width2 * height2)) == NULL)  goto MEMORY_ERR;

  // 入力画像
  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &maxdepth);

  fread(gray, sizeof(unsigned char), width * height, fp);

  // 目的画像
  fgets(buf, sizeof(buf), fp2);
  fgets(buf, sizeof(buf), fp2);
  fscanf(fp2, "%d %d\n%d\n", &width2, &height2, &maxdepth2);

  fread(gray2, sizeof(unsigned char), width2 * height2, fp2);

  free(gray2);
  free(gray);

  fclose(fp2);
  fclose(fp);

  return 0;


  ARGUMENT_ERR:
    printf("コマンドライン引数の数は1つにしてください．\n");
    exit(1);

  FILE_ERR:
    printf("ファイルを開けません．\n");
    exit(1);

  MEMORY_ERR:
    printf("メモリ領域の確保に失敗しました．\n");
    exit(1);
}


// アフィン変換による画像の幾何学的歪みの除去
void affine(FILE *fp, int width, int width2, int height, int height2, int maxdepth, int maxdepth2, unsigned char gray[], unsigned char gray2[])
{

}
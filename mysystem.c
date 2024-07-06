/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
//extern char **environ; //自身の環境変数を指すポインタenviron
static char *execpath = "/bin/sh"; //グローバル変数
int mysystem(char *command) {//commandには命令が入っている
  pid_t pid;
  // ここにプログラムを書く
  if (command == NULL) { //NULLポインタが渡された
    return 1;
  }else if ((pid = fork()) < 0){ //fork()のエラー時は-1が返される
    perror(command);
    return -1;
  }
  if(pid != 0){
    int status;
    while (wait(&status) != pid);
    return status;
  }else{//子プロセスの処理　親プロセスはスキップ
    execl(execpath, "sh", "-c", command, (char *)NULL);
    perror(execpath);//正常に実行された場合はこれ以降実行されない
    exit(127);
  }
  return 0;//正常時は0を返す
}

/* 実行例

ここに実行例を書く
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa % make
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c

コマンドラインで実行した結果
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa % ls -l
total 736
-rw-r--r--  1 airaichikawa  staff     143  7  4 09:44 Makefile
-rw-r--r--  1 airaichikawa  staff    2795  7  4 09:44 README.md
-rw-r--r--  1 airaichikawa  staff  238232  7  4 09:44 README.pdf
-rwxr-xr-x  1 airaichikawa  staff   50168  7  5 18:08 memo
-rw-r--r--  1 airaichikawa  staff     519  7  5 18:08 memo.c
-rw-r--r--  1 airaichikawa  staff    1132  7  5 18:28 memo1.c
-rwxr-xr-x  1 airaichikawa  staff   50392  7  5 18:38 mysysmain
-rw-r--r--  1 airaichikawa  staff     925  7  4 09:44 mysysmain.c
-rw-r--r--  1 airaichikawa  staff    1152  7  5 18:38 mysystem.c
-rw-r--r--  1 airaichikawa  staff      90  7  4 09:44 mysystem.h

mysysmainを使って実行した結果。出力結果が同じになった。
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa % ./mysysmain "ls -l"
mysystem:
total 736
-rw-r--r--  1 airaichikawa  staff     143  7  4 09:44 Makefile
-rw-r--r--  1 airaichikawa  staff    2795  7  4 09:44 README.md
-rw-r--r--  1 airaichikawa  staff  238232  7  4 09:44 README.pdf
-rwxr-xr-x  1 airaichikawa  staff   50168  7  5 18:08 memo
-rw-r--r--  1 airaichikawa  staff     519  7  5 18:08 memo.c
-rw-r--r--  1 airaichikawa  staff    1132  7  5 18:28 memo1.c
-rwxr-xr-x  1 airaichikawa  staff   50376  7  6 10:00 mysysmain
-rw-r--r--  1 airaichikawa  staff     925  7  4 09:44 mysysmain.c
-rw-r--r--  1 airaichikawa  staff    3146  7  6 10:00 mysystem.c
-rw-r--r--  1 airaichikawa  staff      90  7  4 09:44 mysystem.h
retval = 00000000
system:
total 736
-rw-r--r--  1 airaichikawa  staff     143  7  4 09:44 Makefile
-rw-r--r--  1 airaichikawa  staff    2795  7  4 09:44 README.md
-rw-r--r--  1 airaichikawa  staff  238232  7  4 09:44 README.pdf
-rwxr-xr-x  1 airaichikawa  staff   50168  7  5 18:08 memo
-rw-r--r--  1 airaichikawa  staff     519  7  5 18:08 memo.c
-rw-r--r--  1 airaichikawa  staff    1132  7  5 18:28 memo1.c
-rwxr-xr-x  1 airaichikawa  staff   50376  7  6 10:00 mysysmain
-rw-r--r--  1 airaichikawa  staff     925  7  4 09:44 mysysmain.c
-rw-r--r--  1 airaichikawa  staff    3146  7  6 10:00 mysystem.c
-rw-r--r--  1 airaichikawa  staff      90  7  4 09:44 mysystem.h
retval = 00000000

引数が1つしかない時。エラーメッセージを表示する。
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa % ./mysysmain   
使い方 : ./mysysmain コマンド文字列

mysysmainを使って、date命令の実行
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa % ./mysysmain "date"
mysystem:
2024年 7月 5日 金曜日 18時48分18秒 JST

コマンドの起動に失敗した場合
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa %  ./mysysmain "aaa"
mysystem:
sh: aaa: command not found
retval = 00007f00
system:
sh: aaa: command not found
retval = 00007f00

起動されたコマンドがエラーを起こす場合
airaichikawa@AiranoMacBook-Air kadai10-i21itikawa %  ./mysysmain "rm"        
mysystem:
usage: rm [-f | -i] [-dIPRrvWx] file ...
       unlink [--] file
retval = 00004000
system:
usage: rm [-f | -i] [-dIPRrvWx] file ...
       unlink [--] file
retval = 00004000
*/

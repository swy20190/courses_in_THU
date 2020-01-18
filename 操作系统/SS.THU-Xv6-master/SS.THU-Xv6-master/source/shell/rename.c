/*
*�ļ����ƣ�rename.c
*�����ߣ���ٻ
*�������ڣ�2018/06/07
*�ļ��������ļ�������
*��ʷ��¼�����������ְ෽����
*/
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(2, "Usage: rename filename newfilename\n");
    exit();
  }
  if(link(argv[1], argv[2]) < 0){
    printf(2, "ren: %s failed to rename\n", argv[1]);
    exit();
  }
  if(unlink(argv[1]) < 0){
    printf(2, "ren: %s failed to unlink the old name\n", argv[1]);
    exit();
  }
  chdir("/");
  exit();
}
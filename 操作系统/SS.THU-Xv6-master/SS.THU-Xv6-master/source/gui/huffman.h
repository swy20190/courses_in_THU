/**********************************************************************
 * MPEG/audio ����/�������                                           *
 * VERSION 4.1                                                        *
 *********************************************************************/	
 
#define HUFFBITS unsigned long int
#define HTN	34
#define MXOFF	250
 
struct huffcodetab {
  char tablename[3];	/*�ַ�, ���� table_description	*/
  unsigned int xlen; 	
  unsigned int ylen;	
  unsigned int linbits; /* linbits	��Ŀ 		*/
  unsigned int linmax;	/*�洢��linbits�е������Ŀ 	*/
  int ref;		/*a positive value indicates a reference*/
  HUFFBITS *table;	/*ָ��array[xlen][ylen]��ָ��		*/
  unsigned char *hlen;	/*ָ�� array[xlen][ylen]��ָ��		*/
  unsigned char(*val)[2];/*������				*/ 
  unsigned int treelen;	/*����������		*/
};

extern unsigned char h1[7][2];
extern unsigned char h2[17][2];
extern unsigned char h3[17][2];
extern unsigned char h5[31][2];
extern unsigned char h6[31][2];
extern unsigned char h7[71][2];
extern unsigned char h8[71][2];
extern unsigned char h9[71][2];
extern unsigned char h10[127][2];
extern unsigned char h11[127][2];
extern unsigned char h12[127][2];
extern unsigned char h13[511][2];
extern unsigned char h15[511][2];
extern unsigned char h16[511][2];
extern unsigned char h24[512][2];
extern unsigned char hA[31][2];
extern unsigned char hB[31][2];
extern struct huffcodetab ht[HTN];/* ȫ���ڴ��		*/
				/* ����huffcodtable headers������	*/
				/* 0..31 Huffman code table 0..31	*/
				/* 32,33 count1-tables			*/
extern void read_decoder_table();
extern int huffman_decoder(struct huffcodetab *, int *, int*, int*, int*);

#pragma once
// �f�[�^�x�[�X�G���W���֗̕����𗝉����邽�߂�C����ł̔�r
//
//    ���ȏ��ɏ�����Ă���SELECT���Ɠ���������
//    ���ۂ�C����ŏ����ƁA�ǂ�قǖʓ|�Ȃ̂�...

#include <stdio.h>
#include <string.h>

// CREATE TABLE S ... ;
struct TABLE_S {
	char company_id[5];    // �ƎҔԍ�
	char company_name[20]; // �ƎҖ�
	int  fineness;           // �D�Ǔx
	char location[15];     // ����
};

// CREATE TABLE G ... ;
struct TABLE_G {
	char item_id[5];       // ���i�ԍ�
	char item_name[20];    // ���i��
	char color[8];         // �F
	int  price;              // ���i
	char location[15];     // ����
};

// CREATE TABLE SG ... ;
struct TABLE_SG {
	char company_id[5];    // �ƎҔԍ�
	char item_id[5];       // ���i�ԍ�
	int  inventory;          // �݌ɗ�
};

// INSERT INTO S ... ;
struct TABLE_S table_s[] = {
	{ "S1" , "ABC" , 20 , "Fukui" } ,
	{ "S2" , "LMN" , 10 , "Tokyo" } ,
	{ "S3" , "PQR" , 30 , "Tokyo" } ,
	{ "S4" , "STU" , 20 , "Fukui" } ,
	{ "S5" , "XYZ" , 30 , "Osaka" } ,
	{ "" ,   "" ,    0 ,  ""      }
};

// INSERT INTO G ... ;
struct TABLE_G table_g[] = {
	{ "G1" , "red pencil" , "red"    , 120 , "Fukui" } ,
	{ "G2" , "note" ,       "blue"   , 170 , "Tokyo" } ,
	{ "G3" , "eraser" ,     "yellow" ,  50 , "Kyoto" } ,
	{ "G4" , "eraser" ,     "white" ,   50 , "Fukui" } ,
	{ "G5" , "pencil case", "blue" ,   300 , "Tokyo" } ,
	{ "G6" , "binder" ,     "green" ,  250 , "Fukui" } ,
	{ "" ,   "" ,           "" ,         0 ,  ""     }
};

// INSERT INTO SG ... ;
struct TABLE_SG table_sg[] = {
	{ "S1" , "G1" , 300 } ,
	{ "S1" , "G2" , 200 } ,
	{ "S1" , "G3" , 400 } ,
	{ "S1" , "G4" , 200 } ,
	{ "S1" , "G5" , 100 } ,
	{ "S1" , "G6" , 100 } ,

	{ "S2" , "G1" , 300 } ,
	{ "S2" , "G2" , 400 } ,
	{ "S3" , "G2" , 200 } ,
	{ "S4" , "G2" , 200 } ,
	{ "S4" , "G4" , 300 } ,
	{ "S4" , "G5" , 400 } ,
	{ "" , "" , 0 }
};

int SQLmodoki() {

	// SELECT MAX(S.�D�Ǔx) FROM S WHERE LOCATION = 'Tokyo' ;

	int max = 0;
	// ���ׂĂ�S�ɂ��ČJ��Ԃ�
	for (struct TABLE_S* s = table_s; s->company_id[0] != '\0'; s++) {
		if (strcmp(s->location, "Tokyo") == 0)
			if (s->fineness > max) {
				max = s->fineness;
			}
	}
	printf("| %d  |\n", max);

	// SELECT S.�ƎҔԍ�,G.���i�ԍ� FROM S,G WHERE S.���� = G.���� ;

	// ���ׂĂ�S�ɂ��ČJ��Ԃ�
	for (struct TABLE_S* s = table_s; s->company_id[0] != '\0'; s++) {
		// ���ׂĂ�G�ɂ��ČJ��Ԃ�
		for (struct TABLE_G* g = table_g; g->item_id[0] != '\0'; g++) {
			if (strcmp(s->location, g->location) == 0) {
				printf("| %-3s | %-3s |\n",
					s->company_id,
					g->item_id
					);
			}
		}
	}

	return 0;
}
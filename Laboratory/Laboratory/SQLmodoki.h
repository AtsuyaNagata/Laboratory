#pragma once
// データベースエンジンの便利さを理解するためのC言語での比較
//
//    教科書に書かれているSELECT文と同じ処理を
//    実際にC言語で書くと、どれほど面倒なのか...

#include <stdio.h>
#include <string.h>

// CREATE TABLE S ... ;
struct TABLE_S {
	char company_id[5];    // 業者番号
	char company_name[20]; // 業者名
	int  fineness;           // 優良度
	char location[15];     // 所在
};

// CREATE TABLE G ... ;
struct TABLE_G {
	char item_id[5];       // 商品番号
	char item_name[20];    // 商品名
	char color[8];         // 色
	int  price;              // 価格
	char location[15];     // 所在
};

// CREATE TABLE SG ... ;
struct TABLE_SG {
	char company_id[5];    // 業者番号
	char item_id[5];       // 商品番号
	int  inventory;          // 在庫量
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

	// SELECT MAX(S.優良度) FROM S WHERE LOCATION = 'Tokyo' ;

	int max = 0;
	// すべてのSについて繰り返し
	for (struct TABLE_S* s = table_s; s->company_id[0] != '\0'; s++) {
		if (strcmp(s->location, "Tokyo") == 0)
			if (s->fineness > max) {
				max = s->fineness;
			}
	}
	printf("| %d  |\n", max);

	// SELECT S.業者番号,G.商品番号 FROM S,G WHERE S.所在 = G.所在 ;

	// すべてのSについて繰り返し
	for (struct TABLE_S* s = table_s; s->company_id[0] != '\0'; s++) {
		// すべてのGについて繰り返し
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
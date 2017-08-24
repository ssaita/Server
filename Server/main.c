//
//  main.c
//  Server
//
//  Created by Satoko Saita on 2017/08/25.
//  Copyright © 2017年 Satoko Saita. All rights reserved.
//

#include <stdio.h>
#include <string.h> // bzero
//#include <errno.h>
#include <stdlib.h> // exit
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h> // socket, bind, listen, accept, recv
#include <sys/types.h>
//#include <netinet/in.h> // htons

#define PORT 300000 //ポート番号
#define MAX 1024	//受信するバッファサイズ

int main(){
    int connect; //ソケットへの接続結果を示す. 接続できたら非負の整数値を返し, エラーが発生した場合は -1 を返す.
    int listener; //
    int addr;
	int rsize;
	char buf[MAX]; /* 受信バッファ */
	
	struct sockaddr_in server;	// サーバ用アドレス格納構造体
	struct sockaddr_in client;	// クライアント用アドレス格納構造体
	
//	struct sockaddr_in addr;
//	
//    char advice_recv[512],recv_txt[512];
//    char *advice[]={
//        "毎度ありがとうございます。\n",             // message1
//        "またの機会をお待ちいたしております。\n",     // message2
//		"もう夕方です。\n"};                      // message3
//    

	// socket 接続を受け入れるソケット（差し込み口）を作成
	//	第1引数は通信ドメイン（プロトコルファミリ）を指定する．
	//	第2引数は通信形式を指定する．
	//	第3引数は使用プロトコルを指定する．0で自動設定, もしくはIPPROTO_TCP.
    listener=socket(PF_INET,SOCK_STREAM,0);
	
	
    // ポート番号・IPアドレスなどを指定するための構造体を設定．
    memset(&server, 0, sizeof(struct sockaddr_in));	// 構造体を0で埋める
    server.sin_family = AF_INET;					// IPアドレスを代入する．
    server.sin_addr.s_addr = htonl(INADDR_ANY);		// どのクライアントからも接続要求を受け付ける
	server.sin_port = htons(PORT);					// ポート番号で初期化
	socklen_t clientlen = sizeof(client);
	
    // bind サーバ側で利用するアドレス(IPアドレスとポート番号)を設定する．これでクライアントとの接続準備完了
	printf("bind\n");
	if(bind(listener, (struct sockaddr*)&server, sizeof(struct sockaddr)) < 0){
		perror("bind");
		exit(1);
	}
	
	// listen ソケットの接続準備
	//	要求受付を開始する．第2引数は最大何個のクライアントを
	//	接続要求で待たせることができるか（待ち行列の長さ）を指定している．
	//	通常はOSの段階で5以下に制限されてしまう場合が多いので5でいい．
	printf("listen\n");
	if(listen(listener, 5) < 0){
		perror("listen");
		exit(1);
	}
	
	// accept ソケットの接続待機
	printf("accept\n");
	addr = accept(listener, (struct sockaddr*)&client, &clientlen);
	if(addr < 0){
		perror("accept");
		exit(1);
	}
	
	close(listener);
	
	while(1){
		rsize = recv(addr, buf, MAX, 0);
		if (rsize == 0) { /* クラアイントが接続を切ったとき */
			break;
		} else if (rsize == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		} else {
			write(addr, buf, rsize);
		}
	}
	if ( close(addr) < 0) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	printf("Connection closed.\n");
	return 0;
	//	puts("Waiting Connection.");
//	
//	
//    while(1){
//        struct sockaddr_storage client_addr;
//        unsigned int address_size = sizeof(client_addr);
//     
//        // Connection
//        connect = accept(listener, (struct sockaddr *)&client_addr, &len);
//        if(connect < 0 ){
//            printf("fail in accept connect: ",connect);
//        }
//        printf("接続要求を受け付けました\n");
//        printf("接続してきたクライアントにメッセージを送ってください:\n");
////        while(1){
//            //char *msg = advice[rand() %3];
//            char msg[512];
//            // Send
//            printf(":");
//            scanf("%s",&msg);
//            printf("\n");
//            send(connect_d,msg,strlen(msg),0);
//            printf("サーバーさん：");
//            cmpd=strncmp(msg,"disconnected!", 13);
//            if(cmpd==0){
//                close(connect_d);
//                break;
//            }
//            else{
//                ret = recv(listener_d,recv_txt,sizeof(recv_txt)-1,0);
//                recv_txt[ret]='\0';
//                printf("%s\n",recv_txt);
//            }
//        }
//        if(cmpd==0){
//            close(connect);
//            break;
//        }
//    }
//    return 0;
}
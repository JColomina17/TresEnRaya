#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

char board[5][5] = {
                    {'A','B','C','D','E'},
                    {'F','G','H','I','J'},
                    {'K','L','M','N','P'},
                    {'Q','R','S','T','U'},
                    {'V','W','Y','Z','1'}};
char sBoard[25];
char aux[25];

int winner1=0;
int winner2=0;
int draw=0;
int finish=0;
char option;

void printBoard(){
    printf("\n\n");
    printf("---+---+---+---+---\n");
    for (int x=0; x<5; x++) {
       printf(" %c | %c | %c | %c | %c |\n", board[x][0], board[x][1], board[x][2],board[x][3],board[x][4]);
        printf("---+---+---+---+---\n");
    }
    printf("\n\n");

}
void arrayBoard(){
    int k=0;
    for(int i=0;i<5;i++){
            for(int j=0;j<5;j++){
                sBoard[k]=board[i][j];
                k++;
            }
        }
    //************//
}
void matrixBoard(){
    int k=0;
    for(int i=0;i<5;i++){
            for(int j=0;j<5;j++){
                board[i][j]=sBoard[k];
                k++;
            }
        }
}
void checkWinner(){
    int streak = 1;
            for(int y=0; y<5; y++) {
                streak = 1;
                for(int x=0; x<4; x++) {
                    if(board[y][x] == board[y][x+1]) {
                        streak++;

                        if(streak == 3) {
                            if(board[y][x] == 'X'){
                                winner1 = 1;}
                            else{
                                winner2 = 1;}
                        }
                    } else
                        streak = 1;
                }
            }

            // check Vertical Winner
 {
                for(int x=0; x<5; x++) {
                    streak = 1;
                    for(int y=0; y<4; y++) {
                        if(board[y][x] == board[y+1][x]) {
                            streak++;
                            if(streak == 3) {
                                if(board[y][x] == 'X'){
                                    winner1 = 1;}
                                else{
                                    winner2 = 1;}
                            }
                        } else
                            streak = 1;
                    }
                }
            
             

            //check diagonal winner---------------------------------
           
                int streak2;
                int streak3;
                int streak4;
                for(int j=0; j<4; j++) {
                    streak = 1;
                    streak2 = 1;
                    streak3 = 1;
                    streak4 = 1;
                    for(int i=0; i<5-j; i++) {
                        if(board[i+j][i] == board[i+j+1][i+1]) {
                            streak++;
                            if(streak == 3)
                                if(board[i+j][i] == 'X')
                                    winner1 = 1;
                                else
                                    winner2 = 1;
                        } else
                            streak = 1;



                        if(board[i-1][i+j] == board[i][i+1+j]) {
                            streak2++;
                            if(streak2 == 3)
                                if(board[i-1][i+j] == 'X')
                                    winner1 = 1;
                                else
                                    winner2=1;
                        } else
                            streak2 = 1;



                        if(board[5-i-j][i-1] == board[5-i-1-j][i]) {
                            streak3++;
                            if(streak3 == 3)
                                if(board[5-i-j][i-1] == 'X')
                                    winner1 = 1;
                                else
                                    winner2=1;
                            } else
                                streak3 = 1;



                        if(board[5-i][i+j] == board[5-1-i][i+1+j]) {
                            streak4++;
                            if(streak4 == 3)
                                if(board[5-i][i+j] == 'X')
                                    winner1 = 1;
                                else
                                    winner2 = 1;
                        } else
                            streak4 = 1;
                    }

        
                }
 }
            
    
}
void End(int i){

     if(i==25||(draw==1)){
         printf("Draw");
     }else{
         if(winner1==1){
                 printf("Player 1 is the winner");
             finish=1;
         }else
             if(winner2==1){
               printf("Player 2 is the winner");
             finish=1;
             
         }

     }
 }
void movement(char o){
    if(o=='X'||o=='O'){
        printf("\nNo available"); 
    }
    else{
    for ( int x=0; x<5; x++) {
        for (int y=0 ; y<5; y++) {
            if (board[x][y]==o) {
                    board[x][y]='O';
            }
        }
    }
    }
}
void func(int sockfd)
{
    int player = 1;
    int go = 0;
    int row = 0;
    int column = 0;
    int line = 0;
    int winner = 0;
    
    char option;
    int n;
    for( int i = 0; i<25 && winner==0; i++){
        printf(" --------------------------------------------------------------");

        printf("\nTable status: ");

        printBoard(); //muestra la matrix
        
  
        bzero(sBoard, MAX);
        
        printf("\nPlease enter  the square: ");
        scanf("%c",&option);// donde quiere poner la x/o
        
        movement(option);//la pone
    
        printf("\nTable status:: ");
        arrayBoard();
        printBoard();
        checkWinner();
        End(i);

        if(finish==1){
            write(sockfd, "exit", sizeof("exit"));//manda la respuesta
            read(sockfd, aux, sizeof(aux)); // recibe la respuesta
            write(sockfd, sBoard, sizeof(sBoard));//manda la respuesta
            printBoard();//muestra la matrix
            break;
        }
        else{
            write(sockfd, sBoard, sizeof(sBoard));//manda la respuesta
        }


//************************************************************************************************************************************************
        bzero(sBoard, MAX);

        read(sockfd, sBoard, sizeof(sBoard)); // recibe la respuesta

        if (strcmp(sBoard, "exit") == 0) {
            printf("\nGAME OVER);
            printf("Player 2 is the winner");

            bzero(sBoard, MAX);
            write(sockfd, "", sizeof(""));//manda la respuesta
            bzero(sBoard, MAX);
            read(sockfd, sBoard, sizeof(sBoard)); // recibe la respuesta
            matrixBoard(); //pasa la respuesta a matrix
            printBoard();//muestra la matrix
            break;
        }
        
        matrixBoard(); //pasa la respuesta a matrix

        
    
    }
}


  
int main(int argc, char *argv[]){
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
  
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
  
    // function for chat
    func(sockfd);
  
    // close the socket
    close(sockfd);
}

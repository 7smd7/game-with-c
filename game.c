#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 100

int size;
int fillCounter=0;
int skip=0;
struct Player{
    char username[MAX];
    char firstName[MAX];
    char lastName[MAX];
    int winRec;
    int lostRec;
    int EqRec;
};
struct Player player[2];

struct Cell{
    int value;
    char player[5];
};
struct Cell cell[MAX][MAX]={0};

int Dice();
void writeName();
void setName();
void setSize();
void showDice(int dice);
void table();
void savePlayer(struct Player plyr);
void refresh();
void saveRecord(struct Player plyr) ;
int checkStatus();
void input();
void getReport();


int main()
{
   //intro
   writeName();
   getch();
   system("cls");
   setSize();
   setName();
   int firstPlayer= rand()%2;
   int row,col,value;
   while(checkStatus()==0 && fillCounter<=size*size){
      refresh();
      if((fillCounter-skip+firstPlayer)%2==0){
         printf("it's %s's turn:\n",player[0].firstName);
      }
      else{
         printf("it's %s's turn:\n",player[1].firstName);
      }

      printf("please throw dice for row: ");
      input();
      row=Dice();
      showDice(row);
      if (row>size){
         printf("sry row is bigger than size of table\n");
         skip++;
         input();
         continue;
      }
      printf("please throw dice for column: ");
      input();
      col=Dice();
      showDice(col);
      if (col>size){
         printf("sry col is bigger than size of table\n");
         skip++;
         input();
         continue;
      }
      if(cell[row-1][col-1].value!=0){
        printf("sry! cell is full. your turn is burned. Enter to continue.");
        skip++;
        input();
        continue;
      }
      printf("please throw dice for value: ");
      input();
      value=Dice();
      showDice(value);
      input();

      cell[row-1][col-1].value=value;
      if((fillCounter-skip+firstPlayer)%2==0){
         strcpy(cell[row-1][col-1].player,"A");
      }
      else{
         strcpy(cell[row-1][col-1].player,"B");
      }
      fillCounter+=1;
   }
   refresh();

   if(fillCounter==size*size){
      printf("game result is equal.");
      player[0].EqRec++;
      player[1].EqRec++;
   }
   else if((fillCounter-skip+firstPlayer)%2==1){
      printf("the winner is %s.\n",player[0].firstName);
      player[0].winRec++;
      player[1].lostRec++;
   }
   else{
      printf("the winner is %s.\n",player[1].firstName);
      player[0].lostRec++;
      player[1].winRec++;
   }
   saveRecord(player[0]);
   saveRecord(player[1]);
   input();
   return 0;
}

void writeName()
{
   printf(" __  __           _                             _            __                  _ \n");
   printf("|  \\/  |   __ _  | |__     __ _   _ __         | |   __ _   / _|   __ _   _ __  (_)\n");
   printf("| |\\/| |  / _` | | '_ \\   / _` | | '_ \\     _  | |  / _` | | |_   / _` | | '__| | |\n");
   printf("| |  | | | (_| | | | | | | (_| | | | | |   | |_| | | (_| | |  _| | (_| | | |    | |\n");
   printf("|_|  |_|  \\__,_| |_| |_|  \\__,_| |_| |_|    \\___/   \\__,_| |_|    \\__,_| |_|    |_|\n");
   printf("\n\n");
   printf(" __  __                 _              __             _  __  _               _   _   _   _ \n");
   printf("|  \\/  |   ___    ___  | |_    __ _   / _|   __ _    | |/ / | |__     __ _  | | (_) | | (_)\n");
   printf("| |\\/| |  / _ \\  / __| | __|  / _` | | |_   / _` |   | ' /  | '_ \\   / _` | | | | | | | | |\n");
   printf("| |  | | | (_) | \\__ \\ | |_  | (_| | |  _| | (_| |   | . \\  | | | | | (_| | | | | | | | | |\n");
   printf("|_|  |_|  \\___/  |___/  \\__|  \\__,_| |_|    \\__,_|   |_|\\_\\ |_| |_|  \\__,_| |_| |_| |_| |_|\n");
   printf("                                                                                           \n");
}

int Dice()
{
    srand((unsigned)time(NULL));
    return rand()%6+1;
}

void setName(){
    printf("enter the player's data:\n");
    for(int i=0; i<2; i++)
    {
      char username[MAX];
      printf("%i .\n",i+1);
      printf("  Username: ");
      gets(username);
      int valid=0;
      char buff[MAX];
      int line=0;
      FILE *filePointer;
      filePointer = fopen("player.txt", "r") ;
      if (!filePointer)
      {
         filePointer = fopen("player.txt", "w") ;
         filePointer = fopen("player.txt", "r") ;
      }
      char *pos;
      while(fgets ( buff, MAX, filePointer ) != NULL){
         pos=strchr(buff, '\n');
         *pos = '\0';
         if(line%6==0 && strcmp(username,buff)==0){
            strcpy(player[i].username,buff);
            fgets ( buff, MAX, filePointer );
            pos=strchr(buff, '\n');
            *pos = '\0';
            strcpy(player[i].firstName,buff);
            fgets ( buff, MAX, filePointer );
            pos=strchr(buff, '\n');
            *pos = '\0';
            strcpy(player[i].lastName,buff);
            fgets ( buff, MAX, filePointer );
            pos=strchr(buff, '\n');
            *pos = '\0';
            player[i].winRec= atoi(buff);
            fgets ( buff, MAX, filePointer );
            pos=strchr(buff, '\n');
            *pos = '\0';
            player[i].lostRec= atoi(buff);
            fgets ( buff, MAX, filePointer );
            pos=strchr(buff, '\n');
            *pos = '\0';
            player[i].EqRec= atoi(buff);
            valid=1;
            break;
         }
         line++;
      }
      fclose(filePointer);

      if(valid==0){
         strcpy(player[i].username,username);
         printf("  First name: ");
         gets(player[i].firstName);
         printf("  Last name: ");
         gets(player[i].lastName);
         player[i].winRec=0;
         player[i].lostRec=0;
         player[i].EqRec=0;
         savePlayer(player[i]);
      }
    }
}

void setSize()
{
    srand((unsigned)time(NULL));
    size = rand()%3+4;
}

void showDice(int dice){

    printf("your dice=> (");

    switch (dice)
    {
        case 1: printf(".");
        break;

        case 2: printf(":");
        break;

        case 3: printf(":.");
        break;

        case 4: printf("::");
        break;

        case 5: printf("::.");
        break;

        case 6: printf(":::");
        break;
    }

    printf(")\n");
}



void table()
{
    int V3right=204;
    int V3left=185;
    int H3up=202;
    int H3down=203;
    int vertical=186;
    int horizental=205;
    int cornerDR=188;
    int cornerDL=200;
    int cornerUR=187;
    int cornerUL=201;
    int middle=206;
    int iPosition=1;
    int first=1;

    for(int row=0;row<size;row++)
    {

        if(first==1)
        {
            printf("%c",cornerUL);
            for(int i=1;i<size;i++)
                printf("%c%c%c%c%c%c%c%c",horizental,horizental,horizental,horizental,horizental,horizental,horizental,H3down);
            printf("%c%c%c%c%c%c%c%c",horizental,horizental,horizental,horizental,horizental,horizental,horizental,cornerUR);
            first=0;
        }
        else
        {
            printf("%c",V3right);
            for(int i=1;i<size;i++)
                printf("%c%c%c%c%c%c%c%c",horizental,horizental,horizental,horizental,horizental,horizental,horizental,middle);
            printf("%c%c%c%c%c%c%c%c",horizental,horizental,horizental,horizental,horizental,horizental,horizental,V3left);
        }
        printf("\n%c",vertical);

        for(int col=0;col<size;col++)
        {
            if(cell[row][col].value!=0)
               printf("%d\t%c",cell[row][col].value,vertical);
            else
               printf("\t%c",vertical);
        }

        printf("\n%c",vertical);

        for(int col=0;col<size;col++)
        {
            if(cell[row][col].value!=0)
               printf("%s\t%c",cell[row][col].player,vertical);
            else
               printf("\t%c",vertical);
        }

        printf("\n");

    }

        printf("%c",cornerDL);
        for(int i=1;i<size;i++)
            printf("%c%c%c%c%c%c%c%c",horizental,horizental,horizental,horizental,horizental,horizental,horizental,H3up);
        printf("%c%c%c%c%c%c%c%c",horizental,horizental,horizental,horizental,horizental,horizental,horizental,cornerDR);

    printf("\n");
    printf("HELP\n");
    printf("Character=> 'A'/'B' : value was made by player 1/2\n");
    printf("key=> S:(Save)  N:(New Game)   E:(Exit)   Else:(Dice)\n\n");

}

void savePlayer(struct Player plyr){
   FILE *fp;
   fp = fopen("player.txt", "ab");
   fprintf(fp, "%s\n",plyr.username);
   fprintf(fp, "%s\n",plyr.firstName);
   fprintf(fp, "%s\n",plyr.lastName);
   fprintf(fp, "%d\n",plyr.winRec);
   fprintf(fp, "%d\n",plyr.lostRec);
   fprintf(fp, "%d\n",plyr.EqRec);
   fclose(fp);

}


void refresh(){
    system("cls");
    table();
    getReport();
}


void saveRecord(struct Player plyr)
{
      FILE *fptr1, *fptr2;
      int line = 0;
      char str[MAX];
      char temp[MAX] = "temp.txt";
      char fname[MAX]="player.txt";
      fptr1 = fopen(fname, "r");
      fptr2 = fopen(temp, "w");
      char *pos;
      // copy all contents to the temporary file other except specific line
      while (!feof(fptr1))
      {
         strcpy(str, "\0");
         fgets(str, MAX, fptr1);
         if (!feof(fptr1))
         {
            fprintf(fptr2, "%s", str);
            pos=strchr(str, '\n');
            *pos = '\0';
            if(line%6==0 && strcmp(plyr.username,str)==0)
            {
               fgets(str, MAX, fptr1);
               fprintf(fptr2, "%s", str);
               fgets(str, MAX, fptr1);
               fprintf(fptr2, "%s", str);
               fgets(str, MAX, fptr1);
               fprintf(fptr2, "%d\n", plyr.winRec);
               fgets(str, MAX, fptr1);
               fprintf(fptr2, "%d\n", plyr.lostRec);
               fgets(str, MAX, fptr1);
               fprintf(fptr2, "%d\n", plyr.EqRec);
               while (!feof(fptr1)) {
                  fgets(str, MAX, fptr1);
                  if (!feof(fptr1)){
                     fprintf(fptr2, "%s", str);
                  }
               }
            }
            line++;
         }
      }
      fclose(fptr1);
      fclose(fptr2);
      remove(fname);
      rename(temp, fname);
}

int checkStatus(){
   int sum;

   for(int i=0;i<size;i++){
      sum=0;
      for(int j=0;j<size;j++){
         sum+=cell[i][j].value;
      }
      if(sum!=0 && sum%5==0)
         return 1;
   }

   for(int j=0;j<size;j++){
      sum=0;
      for(int i=0;i<size;i++){
         sum+=cell[i][j].value;
      }
      if(sum!=0 && sum%5==0)
         return 1;
   }
   
   int sumU;
   int sumD;
   int j;
   for(int k=0;k<size;k++){
      sumU=0;
      sumD=0;
      j=k;
      for(int i=0;i<size-k;i++){
         sumU+=cell[i][j].value;
         sumD+=cell[j][i].value;
         j++;
      }
      if(sumD!=0 && sumD%5==0)
         return 1;
      if(sumU!=0 && sumU%5==0)
         return 1;
   }

   for(int k=0;k<size;k++){
      sumU=0;
      sumD=0;
      j=size;
      for(int i=0;i<size-k;i++){
         sumU+=cell[i][j].value;
         sumD+=cell[j][i].value;
         j--;
      }
      if(sumD!=0 && sumD%5==0)
         return 1;
      if(sumU!=0 && sumU%5==0)
         return 1;
   }
   return 0;
}


void input(){

    switch(getch()){

        case 'e':
        case 'E':
            system("cls");
            exit(0);
            break;

        case 's':
        case 'S':
            saveRecord(player[0]);
            saveRecord(player[1]);
            printf("\nsaved successfully! do you want continue? press Enter...\n");
            if(getch()!=13)
                exit(0);
            break;

        case 'n':
        case 'N':
            system("cls");
            main();
            break;
    }
}


void getReport(){

   printf("    (Username / Firstname / Lastname / Win / lost / equal)\n");

   char m;
   for(int i=0; i<2; i++) {
      m='A'+i;
      printf("%c=> (%s / %s / %s / %d / %d / %d)\n",m,player[i].username,player[i].firstName,player[i].lastName,player[i].winRec,player[i].lostRec,player[i].EqRec);
   }
}

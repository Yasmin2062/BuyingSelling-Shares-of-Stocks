
/*----------------------------------------------------------------------------------------------------------------------
                                          STUDENT NAME: YASMIN MUIN JWABREH
                                                 STUDENT ID: 1180815
                                                      PROJECT #2
                                                 STOCKS TRANSACTIONS
--------------------------------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct date {
    int dd;
    int mm;
    int yy;
} date;

struct node {
    int numOfShares;
    float price;
    char comName[50];
    struct date Date;
    struct node *Next;
} *top, *top1, *temp, *temptop, *temptop1, *temp2;

/* Pointers To Struct That Holds The Values Of The Queue */
struct node *head = NULL;
struct node *tail = NULL;

struct node *front = NULL;
struct node *rear = NULL;

/* Function That Reads The Data From The Shares.txt File */
void read(char *);

/* To Creat The Node */
void Create();

/* Functions That Enqueue On The Original Queue And The Temporary One (Insert First) */
void Enqueue(int , float , char * , int , int , int );
void EnqueueSORT(int , float , char *, int , int , int );

/* Functions That Dequeue From The Original Queue And The Temporary One (Delete First) */
void Dequeue();
void tempDequeue();

/* Functions That Push On The Original Stack And The Temporary One (Insert First) */
void push(int , float , char * , int , int , int );
void pushTEMP(int , float , char *, int , int , int );

/* Functions That Pop From The Original Stack And The Temporary One (Delete Last) */
void pop();
void tempPOP();

/* Functions That Display From The Original Stack And Queue */
void DisplayStack();
void DisplayQueue();

/* Functions To Buy New Shares, And Sell Either New Shares Or Old Ones By The Two Principles (FIFO,LIFO) */
void BuyShares();
void SellShares();


/* Function The Convert Date Strings To Integers Values (Day,Month,Year) */
int * month(char* str);

/* Functions To Sell Recording To The Wanted Principle */
void FIFOSell(int , float , char *);
void LIFOSell(int , float , char *);

/* Function That Print The Total Gain From Selling Stocks */
void PrintGain();

/* Function That Prints The Main Menu */
void menu();

/* Function To Bubble Sort The Given Linked List */
void SortStack(struct node *);
void SortQueue(struct node *);

/* Function To Store The Last Updated Data Back To Shares.txt File */
void StoreStack(char *);
void StoreQueue(char *);

/* To Hold The Number Of Stack Nodes, Queue Nodes, The Total Gain From Selling Shares */
int numOfStack = 0;
int numOfQueue = 0;
float gain = 0;
//void DispTempQueue();
//void DisTempStack();
// -------------------------------------------------- Main Function ------------------------------------------------- //

int main() {
    Create();
    printf("\n---------------- WELCOME TO MY FIFO LIFO PROGRAM! -------------------\n");
    menu();
}

// ------------------------------------------------------ Menu ------------------------------------------------------ //

void menu() {
    int choice, choice1;
    //printf("---------------------------------------------------------------------\n");
    printf("                      ->         MENU         <-                      \n");
    printf("         PLEASE ENTER THE NUMBER OF THE OPTION YOU WANT TO RUN NEXT.         \n");
    printf("         1. Read stocks transaction data from 'shares' file.                       \n");
    printf("         2. Buy new shares.                                                         \n");
    printf("         3. Sell Shares OF STOCKS( FIFO OR LIFO ).                                  \n");
    printf("         4. Print total capital gain/loss.                              \n");
    printf("         5. Store the held shares back to 'shares' file.                             \n");
    printf("         6. Exit the program.                        \n");
    scanf("%d", &choice);
    char *filename = "shares.txt";

    switch (choice) {
        case 1:
            read(filename);
            break;
        case 2:
            BuyShares();
            break;
        case 3:
            SellShares();
            break;
        case 4:
            PrintGain();
            break;
        case 5: {
            printf("         CHOOSE THE PRINCIPLE YOU WANT TO FOLLOW\n");
            printf("         1. LIFO - SELL FIRST\n");
            printf("         2. FIFO - SELL OLD\n");
            scanf("%d", &choice1);
            if (choice1 == 1)
                StoreStack(filename);
            else if (choice1 == 2)
                StoreQueue(filename);
            break;
        }
        case 6:
            printf("         EXITING FROM THE PROGRAM..");
            exit(1);
        default:
            printf("         OOPS! YOU ENTERED AN INVALID NUMBER\n");
    }
}
// -------------------------------------- Read from the file the required data -------------------------------------- //

void read(char *filename) {
    FILE *fp;
    char str[50];
    fp = fopen(filename, "r");
    char trim[] = "; -";

    if (fp == NULL) {
        printf("         Could not open file %s\n", filename);
        printf("         Make Sure That The File Exists!\n");
        return;
    }
    char *token;
    int  numOf,  day, year;
    float Price;
    char comName[50];
    int monthNum;
    int * pointer;
    while (!feof(fp)) {
        if (fgets(str, 50, fp) != NULL ) {
            token = strtok(str, ";");
            numOf = atoi(token);
            token = strtok(NULL, ";");
            Price = (float) atoll(token);
            token = strtok(NULL, ";");
            strcpy(comName, token);
            token = strtok(NULL, ";");
            pointer = month(token);

         day = pointer[0];
            monthNum = pointer[1];
            year = pointer[2];
            push(numOf,Price,comName,day,monthNum,year);
            Enqueue(numOf,Price,comName,day,monthNum,year);

        }
    }
    fclose(fp);
    printf("      --------------------------------------------------------------------------\n");
    printf("       YOU READ THE STOCKS TRANSACTIONS DATA FROM SHARES.TXT FILE SUCCESSFULLY!\n\n");
    SortStack(top);
    SortQueue(front);
    DisplayStack();
    DisplayQueue();
    menu();
}

// ------------------------------------------- Convert month name to int -------------------------------------------- //

int * month(char *str) {
    char *token = strtok(str, "-");
int i=0;
static int arr[3];
    while (token != NULL)
    {
    //    printf("%s\n", token);
        if(i==0)
            arr[0]=atoi(token);
        if(i==1)
            arr[1]=atoi(token);
        if(i==2)
            arr[2]=atoi(token);
        token = strtok(NULL, "-");
        i++;
    }
    return arr;

}

// --------------------------------------------------- Creat Node --------------------------------------------------- //

void Create() {
    /* To Start From Null */
    top = NULL;
    top1 = NULL;
}

// ----------------------------------- Sort Transactions Depending On Their Dates ----------------------------------- //

/* Bubble sort the given struct */

void SortStack(struct node *start) {
    int temporary;
    struct node *Pointer;
    struct node *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do {
        temporary = 0;
        Pointer = start;

        while (Pointer->Next != lptr) {
            if ((Pointer->Date.yy == Pointer->Next->Date.yy && Pointer->Date.mm == Pointer->Next->Date.mm &&
                    Pointer->Date.dd < Pointer->Next->Date.dd) ||
                (Pointer->Date.yy == Pointer->Next->Date.yy && Pointer->Date.mm < Pointer->Next->Date.mm) ||
                (Pointer->Date.yy < Pointer->Next->Date.yy)) {

                int day, month, year,  numOf;
                float price1;
                char name[50];
                year = Pointer->Date.yy;
                month = Pointer->Date.mm;
                day = Pointer->Date.dd;
                numOf = Pointer->numOfShares;
                price1 = Pointer->price;
                strcpy(name, Pointer->comName);
                Pointer->Date.dd = Pointer->Next->Date.dd;
                Pointer->Date.mm = Pointer->Next->Date.mm;
                Pointer->Date.yy = Pointer->Next->Date.yy;
                Pointer->numOfShares = Pointer->Next->numOfShares;
                Pointer->price = Pointer->Next->price;
                strcpy(Pointer->comName, Pointer->Next->comName);
                Pointer->Next->Date.dd = day;
                Pointer->Next->Date.mm = month;
                Pointer->Next->Date.yy = year;
                Pointer->Next->numOfShares = numOf;
                Pointer->Next->price = price1;
                strcpy(Pointer->Next->comName, name);
                temporary = 1;

            }
            Pointer = Pointer->Next;
        }
        lptr = Pointer;
    } while (temporary);
}

void SortQueue(struct node *start) {
    int swapped;
    struct node *ptr1;
    struct node *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = start;

        while (ptr1->Next != lptr) {
            if ((ptr1->Date.yy == ptr1->Next->Date.yy && ptr1->Date.mm == ptr1->Next->Date.mm &&
                 ptr1->Date.dd > ptr1->Next->Date.dd) ||
                (ptr1->Date.yy == ptr1->Next->Date.yy && ptr1->Date.mm > ptr1->Next->Date.mm) ||
                (ptr1->Date.yy > ptr1->Next->Date.yy)) {

                int day, month, year,  numOf;
                float price1;
                char name[50];
                year = ptr1->Date.yy;
                month = ptr1->Date.mm;
                day = ptr1->Date.dd;
                numOf = ptr1->numOfShares;
                price1 = ptr1->price;
                strcpy(name, ptr1->comName);
                ptr1->Date.dd = ptr1->Next->Date.dd;
                ptr1->Date.mm = ptr1->Next->Date.mm;
                ptr1->Date.yy = ptr1->Next->Date.yy;
                ptr1->numOfShares = ptr1->Next->numOfShares;
                ptr1->price = ptr1->Next->price;
                strcpy(ptr1->comName, ptr1->Next->comName);
                ptr1->Next->Date.dd = day;
                ptr1->Next->Date.mm = month;
                ptr1->Next->Date.yy = year;
                ptr1->Next->numOfShares = numOf;
                ptr1->Next->price = price1;
                strcpy(ptr1->Next->comName, name);
                swapped = 1;

            }
            ptr1 = ptr1->Next;
        }
        lptr = ptr1;
    } while (swapped);
}

// --------------------------------------------- PUSH ON TEMPORARY STACK -------------------------------------------- //

void pushTEMP(int numOfSh, float price1, char *company, int day1, int mon, int year) {
    if (temptop == NULL) {
        temptop = (struct node *) malloc(1 * sizeof(struct node));
        temptop->Next = NULL;
        temptop->numOfShares = numOfSh;
        temptop->price = price1;
        strcpy(temptop->comName, company);
        temptop->Date.dd = day1;
        temptop->Date.mm = mon;
        temptop->Date.yy = year;
    } else {
        temp2 = (struct node *) malloc(1 * sizeof(struct node));
        temp2->Next = temptop;
        temp2->numOfShares = numOfSh;
        temp2->price = price1;
        strcpy(temp2->comName, company);
        temp2->Date.dd = day1;
        temp2->Date.mm = mon;
        temp2->Date.yy = year;
        temptop = temp2;
    }
    // numOfStack++;
}
// ------------------------------------------------------ PUSH ------------------------------------------------------ //

void push(int numOfSh, float price1, char *company, int day1, int mon, int year) {
    if (top == NULL) {
        top = (struct node *) malloc(1 * sizeof(struct node));
        top->Next = NULL;
        top->numOfShares = numOfSh;
        top->price = price1;
        strcpy(top->comName, company);
        top->Date.dd = day1;
        top->Date.mm = mon;
        top->Date.yy = year;
    } else {
        temp = (struct node *) malloc(1 * sizeof(struct node));
        temp->Next = top;
        temp->numOfShares = numOfSh;
        temp->price = price1;
        strcpy(temp->comName, company);
        temp->Date.dd = day1;
        temp->Date.mm = mon;
        temp->Date.yy = year;
        top = temp;
    }
    numOfStack++;
}

// --------------------------------------------- ENQUEUE FOR TEMPORARY ----------------------------------------------- //
void EnqueueSORT(int numOfSh, float price1, char *str1, int day1, int mon, int year) {
    struct node *Need = (struct node *) malloc(sizeof(struct node));
    Need->numOfShares = numOfSh;
    Need->price = price1;
    strcpy(Need->comName, str1);
    Need->Date.dd = day1;
    Need->Date.mm = mon;
    Need->Date.yy = year;
    Need->Next = NULL;
    if (head == NULL && tail == NULL) {
        head = tail = Need;
        return;
    }
    tail->Next = Need;
    tail = Need;
}

// ------------------------------------------ Display FOR TEMPORARY QUEUE ------------------------------------------- //
/*
void DispTempQueue() {
    int i = 0;
    struct node *tem = head;
    printf("\n---------------------------------------------------------------------\n");

    if (tem == NULL)
        printf("\n The Temporary Queue List Is Empty! \n\n");
    else printf("\n The Temporary Queue List Is As Follows: \n\n");

    while (tem != NULL) {
        printf("\n         The [ %d ] node: \n\n", i + 1);
        printf("         Num Of Shares-> %d \n", tem->numOfShares);
        printf("         Price-> %d \n", tem->price);
        printf("         Company Name-> %s \n", tem->comName);
        printf("         Day-> %d \n", tem->Date.dd);
        printf("         Month-> %d \n", tem->Date.mm);
        printf("         Year-> %d \n", tem->Date.yy);
        tem = tem->Next;
        i++;
    }
    printf("\n");
}
*/
// ---------------------------------------------------- ENQUEUE ----------------------------------------------------- //

void Enqueue(int numOfSh, float price1, char *company, int day1, int mon, int year) {
    struct node *tem = (struct node *) malloc(sizeof(struct node));
    tem->numOfShares = numOfSh;
    tem->price = price1;
    strcpy(tem->comName, company);
    tem->Date.mm = mon;
    tem->Date.dd = day1;
    tem->Date.yy = year;
    tem->Next = NULL;
    if (front == NULL && rear == NULL) {
        front = rear = tem;
        numOfQueue++;
        return;
    }
    rear->Next = tem;
    rear = tem;
    numOfQueue++;
}

// ------------------------------------------- DISPLAY THE TEMPORARY STACK ------------------------------------------ //
/*
void DisTempStack() {
    int i = 0;
    temptop1 = temptop;

    if (temptop1 == NULL) {
        printf("Temporary Stack is empty");
        return;
    }
    printf("---------------------------------------------------------------------\n");

    printf("\n The Temporary Stack List Is As Follows: \n");

    while (temptop1 != NULL) {
        printf("\n         The [ %d ] node: \n\n", i + 1);
        printf("         Num Of Shares-> %d \n", temptop1->numOfShares);
        printf("         Price-> %f \n", temptop1->price);
        printf("         Company Name-> %s \n", temptop1->comName);
        printf("         Day-> %d \n", temptop1->Date.dd);
        printf("         Month-> %d \n", temptop1->Date.mm);
        printf("         Year-> %d \n", temptop1->Date.yy);
        temptop1 = temptop1->Next;
        i++;
    }
}
*/

// ------------------------------------------------ DISPLAY THE STACK ----------------------------------------------- //

void DisplayStack() {
    int i = 0;
    top1 = top;

    if (top1 == NULL) {
        printf("The Stack Is Empty! \n");
        return;
    }
    printf("___________________________________________________________\n");
    printf("                        STACK LIST:                   \n\n");


    while (top1 != NULL) {
        // printf("\nThe %d Stock Info:\n", i + 1);
        printf("         %d\t", top1->numOfShares);
        printf("%.2f\t", top1->price);
        printf("%s\t", top1->comName);
        printf("         %d-%d-%d ", top1->Date.dd, top1->Date.mm, top1->Date.yy);
        printf("\n");
        top1 = top1->Next;
        i++;
    }
    printf("\n");

}

// ------------------------------------------------ DISPLAY THE QUEUE------------------------------------------------ //

void DisplayQueue() {
    int i = 0;
    struct node *tem = front;
    printf("___________________________________________________________\n");

    if (tem == NULL)
        printf("\n The Queue List Is Empty! \n\n");
    else printf("                        QUEUE LIST:                   \n\n");

    while (tem != NULL) {
        // printf("\nThe %d Stock Info: \n", i + 1);
        printf("         %d\t", tem->numOfShares);
        printf("%.2f\t", tem->price);
        printf("%s\t", tem->comName);
        printf("         %d-%d-%d ", tem->Date.dd, tem->Date.mm, tem->Date.yy);
        tem = tem->Next;
        printf("\n");

        i++;
    }
    printf("\n");
}

// ------------------------------------------ DEQUEUE FROM TEMPORARY QUEUE ------------------------------------------ //

void tempDequeue() {
    struct node *tem = head;
    if (head == NULL) {
        printf("Queue is Empty\n");
        return;
    }
    if (head == tail) {
        head = tail = NULL;
    } else {
        head = head->Next;
    }
    free(tem);
}

// ---------------------------------------------- Delete First (FIFO) ----------------------------------------------- //
void Dequeue() {
    struct node *tem = front;
    if (front == NULL) {
        printf("Queue is Empty\n");
        return;
    }
    if (front == rear) {
        front = rear = NULL;
    } else {
        front = front->Next;
    }
    free(tem);
    numOfQueue--;
}

// ----------------------------------------------- Delete Last (LIFO) ----------------------------------------------- //

void pop() {
    top1 = top;

    if (top1 == NULL) {
        printf("\n Error : Trying to pop from empty stack");
        return;
    } else
        top1 = top1->Next;
    // printf("\n Popped value : %d", top->numOfShares);
    free(top);
    top = top1;
    numOfStack--;
}

void tempPOP() {
    temptop1 = temptop;

    if (temptop1 == NULL) {
        printf("\n Error : Trying to pop from empty stack");
        return;
    } else
        temptop1 = temptop1->Next;
    //  printf("\n Popped value : %d", temptop->numOfShares);
    free(temptop);
    temptop = temptop1;

}

// ------------------------------------------------- BUY NEW SHARES ------------------------------------------------- //

void BuyShares() {
    int numOf,  day;
    float price;
    int monthString, year;
    char name[20];
    printf("\n         WRITE DOWN THE REQUIRED DATA TO THE NEW SHARES \n");
    printf("         THE NUMBER OF THE NEW SHARES IS : \n");
    scanf("%d", &numOf);
    printf("         THE PRICE OF THE SHARES IS : \n");
    scanf("%f", &price);
    printf("         THE NAME OF THE COMPANY OF NEW SHARES IS :                     \n");
    scanf("%s", name);
    printf("         THE DATE OF THE NEW SHARES IS :                     \n");
    scanf("%d%d%d", &day, &monthString, &year);
    printf("---------------------------------------------------------------------\n");

    push(numOf, price, name, day, monthString, year);
    Enqueue(numOf, price, name, day, monthString, year);
    printf("         YOU BOUGHT THE SHARES SUCCESSFULLY!                        \n");
    printf("         The Last Update Of The Transactions Data Is As Follows: \n");
    SortStack(top);
    SortQueue(front);
    DisplayStack();
    DisplayQueue();
    menu();
}

// ------------------------------------------------- SELLING SHARES ------------------------------------------------- //

void SellShares() {
    int numOf;
    float price;
    char name[50];
    printf("\n         WRITE DOWN THE REQUIRED DATA OF THE NEW SHARES \n");
    printf("         THE NUMBER OF THE NEW SHARES IS : \n");
    scanf("%d", &numOf);
    printf("         THE PRICE OF THE SHARES IS : \n");
    scanf("%f", &price);
    printf("         THE NAME OF THE COMPANY OF NEW SHARES IS :                     \n");
    getchar();
    gets(name);
    int choice;
    printf("\n---------------------------------------------------------------------\n");
    printf("\nWOULD YOU LIKE TO SELL NEW OR OLD SHARES?\n");
    printf("PRESS 1 FOR NEW\n");
    printf("PRESS 2 FOR OLD\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            LIFOSell(numOf, price, name);
            break;
        case 2:
            FIFOSell(numOf, price, name);
            break;
        default:
            printf("         OOPS! YOU ENTERED AN INVALID NUMBER\n");
            break;
    }


}

// -------------------------------------------------- FIFO SELLING -------------------------------------------------- //

void FIFOSell(int numOfSh, float price1, char *str1) {
    int i;
    if (front == NULL) {
        printf("\n Queue Is Empty! \n");
        printf("        No Stocks To Sell From! \n");

        return;
    }
    int sizearr = 0;
    struct node *tempo = front;
    // To Decide The Number Of The Needed Queue Nodes
    while (tempo != NULL) {
        if (strcasecmp(str1, tempo->comName) == 0)
            sizearr++;
        tempo = tempo->Next;
    }
    if (sizearr == 0) {
        printf("\n You Can't Sell From This Company Name! \n");
        printf("\n        This Company Isn't Exist      \n");
        menu();
        return;
    }
    // Array To Hold All The Original Values In The Queue
    int ArrQueSize = numOfQueue - sizearr;
    struct node ArrQueue[ArrQueSize];
    tempo = front;
    i = 0;
    while (tempo != NULL && i < ArrQueSize) {
        if (strcasecmp(tempo->comName, str1) != 0) {
            ArrQueue[i] = *tempo;
            i++;
        }
        tempo = tempo->Next;
    }

    // TempArr contains the nodes with the same entered company name
    /*  struct node *TempArr[sizearr];
      //tempo = front;
       //i=0;
      while (tempo != NULL) {
          if (strcasecmp(str1, tempo->comName) == 0) {
              TempArr[i] = tempo;
              i++;
          }
          tempo = tempo->Next;
      }
      */

    int tempNUM, tempDAY, tempMONTH, tempYear;
    float tempPRICE;
    char tempNAME[50];
    tempo = front;
    while (tempo != NULL) {
        if (strcasecmp(tempo->comName, str1) == 0) {
            tempNUM = tempo->numOfShares;
            tempPRICE = tempo->price;
            strcpy(tempNAME, tempo->comName);
            tempDAY = tempo->Date.dd;
            tempMONTH = tempo->Date.mm;
            tempYear = tempo->Date.yy;
            EnqueueSORT(tempNUM, tempPRICE, tempNAME, tempDAY, tempMONTH, tempYear);
        }
        tempo = tempo->Next;
    }
    int remain = numOfSh;
    struct node *Tempo = head;
    while (remain != 0) {
        if (remain <= Tempo->numOfShares) {
            Tempo->numOfShares -= numOfSh;
            gain += (price1 - Tempo->price) * (remain);
            remain = 0;
            //    printf("The total gain is: %f", gain);
            if (Tempo->numOfShares == 0) {
                tempDequeue();
            }
            Tempo=Tempo->Next;
        } else {
            numOfSh -= Tempo->numOfShares;
            remain = numOfSh;//50
            gain += (price1 - Tempo->price) * (Tempo->numOfShares);
            //     printf("The total gain is: %f", gain);
            tempDequeue();
        }
    }
    //   -------- Dequeue all the values in the queue, to enqueue the new values after selling .//
    i = numOfQueue;
    while (i != 0) {
        Dequeue();
        i--;
    }
    Tempo = head;
    /* Enqueue Back The Nodes With The Other Company Names From The Temporary Queue */
    while (Tempo != NULL) {
        tempNUM = Tempo[i].numOfShares;
        tempPRICE = Tempo[i].price;
        strcpy(tempNAME, Tempo[i].comName);
        tempDAY = Tempo[i].Date.dd;
        tempMONTH = Tempo[i].Date.mm;
        tempYear = Tempo[i].Date.yy;
        Enqueue(tempNUM, tempPRICE, tempNAME, tempDAY, tempMONTH, tempYear);
        Tempo = Tempo->Next;
    }
/* Enqueue Back The Nodes With The Same Company Names From The ArrQueue Array Of Queues */
    for (i = 0; i < ArrQueSize; i++) {
        if (strcasecmp(ArrQueue[i].comName, str1) != 0) {
            tempDAY = ArrQueue[i].Date.dd;
            tempNUM = ArrQueue[i].numOfShares;
            tempPRICE = ArrQueue[i].price;
            tempMONTH = ArrQueue[i].Date.mm;
            tempYear = ArrQueue[i].Date.yy;
            strcpy(tempNAME, ArrQueue[i].comName);
            Enqueue(tempNUM, tempPRICE, tempNAME, tempDAY, tempMONTH, tempYear);
        }
    }
    /* Sort The Final Queue Nodes Depending On Thair Dates */
SortQueue(front);
/* Display The Final Queue After Selling */
    DisplayQueue();
    /* Go Back To The Main Menu */

    menu();

}
// -------------------------------------------------- LIFO SELLING -------------------------------------------------- //

void LIFOSell(int numOfSh, float price1, char *str1) {
    int i;
    /* Checks If The Stack Is Empty */
    if (top == NULL) {
        printf("Stack Is Empty! \n");
        return;
    }
    int sizearr = 0;
    top1 = top;
    // struct node *tempo = front;
    /* To Decide The Number Of The Needed Stack Nodes */
    while (top1 != NULL) {
        if (strcasecmp(str1, top1->comName) == 0)
            sizearr++;
        top1 = top1->Next;
    }
    /* Checks if Their Is The Same Entered Company Number Shares */
    if (sizearr == 0) {
        printf("\n        You Can't Sell From This Company Name! \n");
        printf("        This Company Doesn't Exist      \n");
        menu();
        return;
    }
    /* Array To Hold All The Other Company Name Nodes In The Stack */
    int ArrstkSize = numOfStack - sizearr;
    struct node ArrStack[ArrstkSize];
    top1 = top;
    i = 0;
    while (top1 != NULL && i < ArrstkSize) {
        if (strcasecmp(top1->comName, str1) != 0) {
            ArrStack[i] = *top1;
            i++;
        }
        top1 = top1->Next;
    }

    // TempArr contains the nodes with the same entered company name
    /*  struct node *TempArr[sizearr];
      //tempo = front;
       //i=0;
      while (tempo != NULL) {
          if (strcasecmp(str1, tempo->comName) == 0) {
              TempArr[i] = tempo;
              i++;
          }
          tempo = tempo->Next;
      }
      */
    int tempNUM, tempDAY, tempMONTH, tempYear;
    float tempPRICE;
    char tempNAME[50];
    top1 = top;
    /* Push The Nodes With The Same Company Names To The Temporary Stack */
    while (top1 != NULL) {
        if (strcasecmp(top1->comName, str1) == 0) {
            tempNUM = top1->numOfShares;
            tempPRICE = top1->price;
            strcpy(tempNAME, top1->comName);
            tempDAY = top1->Date.dd;
            tempMONTH = top1->Date.mm;
            tempYear = top1->Date.yy;
            pushTEMP(tempNUM, tempPRICE, tempNAME, tempDAY, tempMONTH, tempYear);
        }
        top1 = top1->Next;
    }
  //  DisTempStack();
  /* To Sort The Stack Before Selling */
    SortStack(temptop);
   // DisTempStack();
    int remain = numOfSh;
    temptop1 = temptop;
    /* Start The Selling Operation Until All The Stocks Are Sold */
    while (remain != 0 && temptop1 !=NULL) {
        if (remain <= temptop1->numOfShares) {
            temptop1->numOfShares -= numOfSh;

            gain += (price1 - temptop1->price) * (remain);
            remain -= numOfSh;//50
            //  printf("The total gain is: %f", gain);
            if (temptop1->numOfShares == 0) {
                tempPOP();
            }
            temptop1=temptop1->Next;
        } else {
            numOfSh -= temptop1->numOfShares;
            remain -= numOfSh;//50
            gain += (price1 - temptop1->price) * (temptop1->numOfShares);
              //   printf("The total gain is: %f", gain);
            tempPOP();
        }
    }
//    DisTempStack();
    //   -------- POP all the values in the Stack, to Push the new values after selling .//
    i = numOfStack;
    while (i != 0) {
        pop();
        i--;
    }
/* Push The Nodes With The Same Company Names After Selling Back To The Stack From The Temporary One */

    temptop1 = temptop;
    while (temptop1 != NULL) {
        tempNUM = temptop1[i].numOfShares;
        tempPRICE = temptop1[i].price;
        strcpy(tempNAME, temptop1[i].comName);
        tempDAY = temptop1[i].Date.dd;
        tempMONTH = temptop1[i].Date.mm;
        tempYear = temptop1[i].Date.yy;
        push(tempNUM, tempPRICE, tempNAME, tempDAY, tempMONTH, tempYear);
        temptop1 = temptop1->Next;
    }
    /* Push The Nodes With The Other Company Names Back To The Stack From The ArrStack Array */

    for (i = 0; i < ArrstkSize; i++) {
        if (strcasecmp(ArrStack[i].comName, str1) != 0) {
            tempDAY = ArrStack[i].Date.dd;
            tempNUM = ArrStack[i].numOfShares;
            tempPRICE = ArrStack[i].price;
            tempMONTH = ArrStack[i].Date.mm;
            tempYear = ArrStack[i].Date.yy;
            strcpy(tempNAME, ArrStack[i].comName);
            push(tempNUM, tempPRICE, tempNAME, tempDAY, tempMONTH, tempYear);
        }
    }
    /* Sort Stack Again After Selling */
    SortStack(top);
    /* Display The Final STack After Selling */
    DisplayStack();
    /* Going Back To The Main Menu */
    menu();
}
// ------------------------------------------------ PRINT TOTAL GAIN ------------------------------------------------ //

void PrintGain() {
    printf("---------------------------------------------------------------------\n");
    if (gain > 0)
        /*Check If There Is A Gain Or Loss After The Selling Opertaion */
        printf("\n         Your Total Gain Is: %.2f$ \n", gain);
    else
        printf("\n         Your Total Loss Is: %.2f$\n", gain);
    menu();

}

// ------------------------------------------------- EDIT THE FILE -------------------------------------------------- //

void StoreQueue(char *filename) {
    FILE* fp;
    /* Open Back The Shares.txt File */
    fp = fopen(filename, "w");
    /* Checks if The List Is Empty */
    int i = 0;
    if (head == NULL) {
        printf("LIST IS EMPTY!\n");
        return;
    }
    struct node* tempo;
    tempo = head;
/* Loop To Store Back The New Stocks Info After The Operation Done */
    while (tempo != NULL) {
        // printf("\nThe %d Stock Info:\n", i + 1);
        fprintf(fp,"%d;", tempo->numOfShares);
        fprintf(fp,"%.2f;", tempo->price);
        fprintf(fp,"%s;", tempo->comName);
        fprintf(fp,"%d-%d-%d\n", tempo->Date.dd, tempo->Date.mm, tempo->Date.yy);
        fprintf(fp,"\n");
        tempo = tempo->Next;
        i++;
    }
    printf("\n         Your Stocks File Is Updated Now!  \n");
    /* Going Back To The Main Menu */
    menu();
}

void StoreStack(char *filename) {
    FILE* fp;
    /* Open Back The Shares.txt File */
    fp = fopen(filename, "w");
    int i = 0;
    top1 = top ;
    /* Checks if The List Is Empty */


    if (top1 == NULL) {
        printf("LIST IS EMPTY!\n");
        return;
    }
/* Loop To Store Back The New Stocks Info After The Operation Done */
    while (top1 != NULL) {
        // printf("\nThe %d Stock Info:\n", i + 1);
        fprintf(fp,"%d;", top1->numOfShares);
        fprintf(fp,"%.2f;", top1->price);
        fprintf(fp,"%s;", top1->comName);
        fprintf(fp,"%d-%d-%d\n", top1->Date.dd, top1->Date.mm, top1->Date.yy);
        top1 = top1->Next;
        i++;
    }
    printf("\n         Your Stocks File Is Updated Now!  \n");
    /* Going Back To The Main Menu */

    menu();
}
// ------------------------------------------------------------------------------------------------------------------ //

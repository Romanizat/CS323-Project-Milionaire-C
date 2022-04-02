#include <stdio.h>
#include <string.h>
#include <malloc.h>

/**
 * CS323 Projekat - Konzolna aplikacija Milioner - Marko Josifovic 4494
 *
 * Postoji 12 pitanja rasporedjenih u 3 nivoa
 * Dakle za svaki nivo imacemo jednu listu pitanja
 * Pravilo: nakon nekih nivoa korisniku se cuva ta suma, preko nekog nivoa korisnik moze da odluci da zadrzi trenutnu sumu ili nastavi dalje???
 * Tri pomoci: Pomoc prijatelja (program random bira jedno od ponudjenih odgovora), Pomoc publike (random procenat za 4 ponudjena odgovora), Pola pola (program ostavi tacan i jedan netacan odgovor)
 * Pomoci se mogu iskoristiti tek nakon predjenog 1. nivoa
 *
 *
 */


typedef struct question {
    char questionText[500];
    char correctAnswer[100];
    char wrongAnswer1[100];
    char wrongAnswer2[100];
    char wrongAnswer3[100];
    int level; //1,2,3
} Question;

typedef struct player {
    char username[100];
    char score[100];
} Player;

void getUserByUsername(char *username[], Player *player) {
    //TODO
}

void getAllQuestions(Question *question[]) {
    //TODO
}

Question *getAllQuestionsForLevel(Question *question[], int arraySize, int level, int *newSize) {
    Question *questionsForLevel;
    int i = 0, count = 0;
    for (i; i < arraySize; ++i) {
        if (question[i]->level == level) {
            count++;
        }
    }
    questionsForLevel = (Question *) malloc(count * sizeof(Question));
    int j=0;
    for (i = 0; i < arraySize; ++i) {
        if(question[i]->level==level){
            questionsForLevel[j] = *question[i];
        }
    }
    *newSize = j;
    return questionsForLevel;
}

int main() {
    printf("Welcome to Millionaire!\n");
    printf("Are you a new player? (Y/N)\n");
    char newPlayer;
    char username[100];
    scanf("%c", &newPlayer);
    Player player;
    if (newPlayer == 'Y' || newPlayer == 'y') {
        printf("Welcome new player!\n");
        printf("Please enter the username you will be using:\n");
        scanf("%s", &username);
        strcpy(player.username, username);
        //TODO: boolean method to check if username already exists
        printf("Welcome %s!\n", &player.username);
    } else if (newPlayer == 'N' || newPlayer == 'n') {
        printf("Welcome back!\n");
        printf("Please enter your username:\n");
        scanf("%s", &username);
        //TODO: get user from file and tell user of his previous score
    } else {
        printf("Unknown command\n Please try again...");
        return -1;
    }

    return 0;
}

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

char *getScoreLevelValues() {
    FILE *f = fopen("score_values.txt", "r");
    int stagesSize = 12;
    char *scores = (char *) malloc(12 * 50);
    for (int i = 0; i < stagesSize; ++i) {
        fgets(&scores[i * 50], 50, f);
    }
    fclose(f);
    return scores;
}

Question *getAllQuestions(int *newSize) {
    FILE *f = fopen("questions.txt", "r");

    int questionCount;

    fscanf_s(f, "%d", &questionCount);
    *newSize = questionCount;
    Question *questionList;
    questionList = (Question *) malloc(questionCount * sizeof(Question));
    int maxCount = 500;

    for (int i = 0; i < questionCount; i++) {
        Question question;
        fgets(question.questionText, maxCount, f);
        fgets(question.correctAnswer, maxCount, f);
        fgets(question.wrongAnswer1, maxCount, f);
        fgets(question.wrongAnswer2, maxCount, f);
        fgets(question.wrongAnswer3, maxCount, f);
//        fscanf(f, "%s\n", &question.correctAnswer);
//        fscanf(f, "%s\n", &question.wrongAnswer1);
//        fscanf(f, "%s\n", &question.wrongAnswer2);
//        fscanf(f, "%s\n", &question.wrongAnswer3);
        fscanf(f, "%d\n", &question.level);

        questionList[i] = question;
    }
    return questionList;
}

Question *getAllQuestionsForLevel(Question *questions, int arraySize, int level, int *newSize) {
    Question *questionsForLevel;
    int i = 0, count = 0;
    for (i; i < arraySize; ++i) {
        if (questions[i].level == level) {
            count++;
        }
    }
    questionsForLevel = (Question *) malloc(count * sizeof(Question));
    int j = 0;
    for (i = 0; i < arraySize; ++i) {
        if (questions[i].level == level) {
            questionsForLevel[j] = questions[i];
        }
    }
    *newSize = j;
    return questionsForLevel;
}

void printQuestionsForTestingPurposes(Question *questions, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%s\n", questions[i].questionText);
        printf("%s\n", questions[i].correctAnswer);
        printf("%s\n", questions[i].wrongAnswer1);
        printf("%s\n", questions[i].wrongAnswer2);
        printf("%s\n", questions[i].wrongAnswer3);
        printf("%d\n", questions[i].level);
    }
}

int main() {
    int numberOfQuestions;
    int numberOfLevel1Questions;
    int numberOfLevel2Questions;
    int numberOfLevel3Questions;
    Question *questions = getAllQuestions(&numberOfQuestions);

//    debugging
    Question question1 = questions[0];
    printf("%s\n", question1.questionText);
    printf("%s\n", question1.correctAnswer);
    printf("%s\n", question1.wrongAnswer1);
    printf("%s\n", question1.wrongAnswer2);
    printf("%s\n", question1.wrongAnswer3);
    printf("%d\n", question1.level);
//    ................................
//    printQuestionsForTestingPurposes(questions, numberOfQuestions);
//    int numberOfQuestions = sizeof(&questions) / sizeof(questions[0]);

    Question *questionsLevel1 = getAllQuestionsForLevel(questions, numberOfQuestions, 1, &numberOfLevel1Questions);
    Question *questionsLevel2 = getAllQuestionsForLevel(questions, numberOfQuestions, 2, &numberOfLevel2Questions);
    Question *questionsLevel3 = getAllQuestionsForLevel(questions, numberOfQuestions, 3, &numberOfLevel3Questions);

    printf("%d\n", numberOfLevel1Questions);
    printf("%d\n", numberOfLevel2Questions);
    printf("%d\n", numberOfLevel3Questions);

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

    char *scores = getScoreLevelValues();
    for (int i = 0; i < 12; ++i) {
        printf("Question number %d for %s\n", i, &scores[i * 50]);
        // TODO: read questions and offer answers
    }

    return 0;
}

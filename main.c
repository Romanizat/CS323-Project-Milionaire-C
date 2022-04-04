#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

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
    char answers[4][100];
    int correctAnswerIndex;
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
        for (int j = 0; j < 4; ++j) {
            fgets(question.answers[j], maxCount, f);
        }
        fscanf(f, "%d\n", &question.correctAnswerIndex);
        fscanf(f, "%d\n", &question.level);

        questionList[i] = question;
    }
    fclose(f);
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
        printf("%s\n", questions[i].answers[0]);
        printf("%s\n", questions[i].answers[1]);
        printf("%s\n", questions[i].answers[2]);
        printf("%s\n", questions[i].answers[3]);
        printf("%d\n", questions[i].correctAnswerIndex);
        printf("%d\n", questions[i].level);
    }
}

Question getRandomQuestionForLevel(Question *questions, int size) {
    srand(time(NULL));

    int random = (rand() % (size + 1));
    return questions[random];
}

void shuffle(int *array, size_t n) {
    srand(time(NULL));
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main() {
    int numberOfQuestions;
    int numberOfLevel1Questions;
    int numberOfLevel2Questions;
    int numberOfLevel3Questions;
    Question *questions = getAllQuestions(&numberOfQuestions);
    int answerIndexes[] = {1, 2, 3, 4};


    printQuestionsForTestingPurposes(questions, numberOfQuestions);
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
        char currentScore[100];
        Question question;
        strcpy(currentScore, &scores[i * 50]);
        printf("Question number %d for %s\n", i + 1, currentScore);
        if (i < 5) {
            question = getRandomQuestionForLevel(questionsLevel1, numberOfLevel1Questions);
        } else if (i < 9) {
            question = getRandomQuestionForLevel(questionsLevel2, numberOfLevel2Questions);

        } else {
            question = getRandomQuestionForLevel(questionsLevel3, numberOfLevel3Questions);
        }
        printf("%s", question.questionText);
        shuffle(answerIndexes, 4);
        printf("a. %s\n", question.answers[answerIndexes[0]]);
        printf("b. %s\n", question.answers[answerIndexes[1]]);
        printf("c. %s\n", question.answers[answerIndexes[2]]);
        printf("d. %s\n", question.answers[answerIndexes[3]]);

    }

    return 0;
}

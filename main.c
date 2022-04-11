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

    fscanf_s(f, "%d\n", &questionCount);
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
    int count = 0;
    for (int i = 0; i < arraySize; i++) {
        if (questions[i].level == level) {
            count++;
        }
    }
    questionsForLevel = (Question *) malloc(count * sizeof(Question));
    int j = 0;
    for (int i = 0; i < arraySize; i++) {
        if (questions[i].level == level) {
            questionsForLevel[j] = questions[i];
            j++;
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

void removeItemFromArray(Question *questions, int size, int index) {
    for (int i = index; i < size - 1; i++) {
        questions[i] = questions[i + 1];
    }
}

int getIndexOfQuestionInArray(Question *questions, int size, Question question) {
    for (int i = 0; i < size; i++) {
        if (questions[i].level == question.level
            && questions[i].correctAnswerIndex == question.correctAnswerIndex
            && strcmp(questions[i].questionText, question.questionText) == 0) {
            return i;
        }
    }
    return -1;
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


int getArrayIndexFromAnswerLetter(char letter) {
    switch (letter) {
        case 'A':
        case 'a':
            return 0;
        case 'B':
        case 'b':
            return 1;
        case 'C':
        case 'c':
            return 2;
        case 'D':
        case 'd':
            return 3;
        default:
            return -1;
    }
}

int main() {
    int numberOfQuestions;
    int numberOfLevel1Questions;
    int numberOfLevel2Questions;
    int numberOfLevel3Questions;
    Question *questions = getAllQuestions(&numberOfQuestions);
    int answerIndexes[] = {0, 1, 2, 3};

    Question *questionsLevel1 = getAllQuestionsForLevel(questions, numberOfQuestions, 1, &numberOfLevel1Questions);
    Question *questionsLevel2 = getAllQuestionsForLevel(questions, numberOfQuestions, 2, &numberOfLevel2Questions);
    Question *questionsLevel3 = getAllQuestionsForLevel(questions, numberOfQuestions, 3, &numberOfLevel3Questions);

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
        char guaranteedScore[100]; //TODO implement logic for guaranteed score
        Question question;
        strcpy(currentScore, &scores[i * 50]);
        printf("Question number %d for %s\n", i + 1, currentScore);
        if (i < 5) {
            question = getRandomQuestionForLevel(questionsLevel1, numberOfLevel1Questions);
            removeItemFromArray(questionsLevel1, numberOfLevel1Questions,
                                getIndexOfQuestionInArray(questionsLevel1, numberOfLevel1Questions, question));
            numberOfLevel1Questions--;
        } else if (i < 9) {
            question = getRandomQuestionForLevel(questionsLevel2, numberOfLevel2Questions);
            removeItemFromArray(questionsLevel2, numberOfLevel2Questions,
                                getIndexOfQuestionInArray(questionsLevel2, numberOfLevel2Questions, question));
            numberOfLevel2Questions--;
        } else {
            question = getRandomQuestionForLevel(questionsLevel3, numberOfLevel3Questions);
            removeItemFromArray(questionsLevel3, numberOfLevel3Questions,
                                getIndexOfQuestionInArray(questionsLevel3, numberOfLevel3Questions, question));
            numberOfLevel3Questions--;
        }
        printf("%s", question.questionText);
        char correctAnswer[100];
        strcpy(correctAnswer, question.answers[question.correctAnswerIndex]);
        shuffle(answerIndexes, 4);
        printf("A) %s\n", question.answers[answerIndexes[0]]);
        printf("B) %s\n", question.answers[answerIndexes[1]]);
        printf("C) %s\n", question.answers[answerIndexes[2]]);
        printf("D) %s\n", question.answers[answerIndexes[3]]);
        printf("Final answer? (A, B, C, D)\n");
        //TODO implement logic to give player chance to give up and walk away with the current score
        //TODO implement logic to give player chance to use one of the jokers
        char playerAnswerLetter;
        while (getchar() != '\n');
        scanf("%c", &playerAnswerLetter);
        char playerAnswer[100];
        strcpy(playerAnswer, question.answers[answerIndexes[getArrayIndexFromAnswerLetter(playerAnswerLetter)]]);
        if (strcmp(playerAnswer, correctAnswer) == 0) {
            printf("Your final answer for %s was correct!\n", currentScore);
            if (i == 5 || i == 9) {
                printf("You have achieved a guaranteed value!\n");
                strcpy(guaranteedScore, currentScore);
            } else if (i == 11) {
                printf("You have won one million euros!\n");
//                TODO implement writing user to file and end game
            }
        } else {
            printf("Your final answer for %s was incorrect!\n", currentScore);
            printf("Your answer was %s\n", playerAnswer);
            printf("The correct answer was %s\n", correctAnswer);
            break;
        }


//        todo: finish the rest of answering logic and save the score to file
    }

    return 0;
}

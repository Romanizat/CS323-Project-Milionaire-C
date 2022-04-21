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

Player *getAllPlayers(int *newSize) {
    FILE *f = fopen("players.txt", "r");

    int playerCount;

    fscanf_s(f, "%d\n", &playerCount);
    *newSize = playerCount;
    Player *playerList;
    playerList = (Player *) malloc(playerCount * sizeof(Player));
    int maxCount = 500;

    for (int i = 0; i < playerCount; i++) {
        Player player;
        fgets(player.username, maxCount, f);
        fgets(player.score, maxCount, f);

        playerList[i] = player;
    }
    fclose(f);
    return playerList;
}

void writePlayersToFile(Player *players, int size) {
    FILE *f = fopen("players.txt", "w");
    fprintf(f, "%d\n", size);
    for (int i = 0; i < size; ++i) {
        fprintf(f, "%s", players[i].username);
        fprintf(f, "%s", players[i].score);
    }
    fclose(f);
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

char getLetterFromNumber(int number) {
    switch (number) {
        case 0:
            return 'A';
        case 1:
            return 'B';
        case 2:
            return 'C';
        case 3:
            return 'D';
        default:
            return -1;
    }
}

int getRandomNumber(int min, int max) {
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

int randomPercentage() {
    srand(time(NULL));
    return rand() % 101;
}

void phoneFriendJoker() {
    printf("------------------------------------------------------\n");
    printf("Friend: I think it's %c\n", getLetterFromNumber(getRandomNumber(0, 3)));
    printf("You: How certain are you?\n");
    printf("Friend: I'm %d%% certain\n", randomPercentage());
    printf("------------------------------------------------------\n");
}

int *getRandomNumbersThatAddUpTo100() {
    int *numbers = (int *) malloc(sizeof(int) * 4);
    int sum = 0;
    numbers[0] = getRandomNumber(0, 100);
    while (sum != 100) {
        sum = 0;
        sum += numbers[0];
        numbers[1] = getRandomNumber(0, 100 - sum);
        sum += numbers[1];
        numbers[2] = getRandomNumber(0, 100 - sum);
        sum += numbers[2];
        numbers[3] = getRandomNumber(0, 100 - sum);
        sum += numbers[3];

    }
    return numbers;
}

void askTheAudience() {
    int *arr = getRandomNumbersThatAddUpTo100();
    printf("------------------------------------------------------\n");
    printf("A) %d%%\n", arr[0]);
    printf("B) %d%%\n", arr[1]);
    printf("C) %d%%\n", arr[2]);
    printf("D) %d%%\n", arr[3]);
    printf("------------------------------------------------------\n");
}

int getPlayerIndexByUsername(Player *players, int size, char *username) {
    for (int i = 0; i < size; i++) {
        if (strcmp(players[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

void printPlayerScore(Player player) {
    printf("Your previous score was: %s", &player.score);
}

int main() {
    int numberOfQuestions;
    int numberOfLevel1Questions;
    int numberOfLevel2Questions;
    int numberOfLevel3Questions;
    int numberOfPlayers;
    int playerIndexInList;
    Question *questions = getAllQuestions(&numberOfQuestions);
    int answerIndexes[] = {0, 1, 2, 3};
    Player *players = getAllPlayers(&numberOfPlayers);

    Question *questionsLevel1 = getAllQuestionsForLevel(questions, numberOfQuestions, 1, &numberOfLevel1Questions);
    Question *questionsLevel2 = getAllQuestionsForLevel(questions, numberOfQuestions, 2, &numberOfLevel2Questions);
    Question *questionsLevel3 = getAllQuestionsForLevel(questions, numberOfQuestions, 3, &numberOfLevel3Questions);

    int jokerFriend = 1;
    int jokerFiftyFifty = 1;
    int jokerAudience = 1;
    int jokerChangeQuestion = 1; // after level 1

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
        playerIndexInList = getPlayerIndexByUsername(players, numberOfPlayers, username);
        if (playerIndexInList == -1) {
            strcpy(player.username, username);
            printf("Welcome %s!\n", &player.username);
        } else {
            player = players[playerIndexInList];
            printf("Player already exists!\nWill continue game with player %s\n.", &player.username);
            printPlayerScore(player);
        }
    } else if (newPlayer == 'N' || newPlayer == 'n') {
        printf("Please enter your username:\n");
        scanf("%s", &username);
        playerIndexInList = getPlayerIndexByUsername(players, numberOfPlayers, username);
        if (playerIndexInList == -1) {
            printf("User with username %s not found\n", &username);
            strcpy(player.username, username);
            printf("Player doesn't exist!\nWill continue game as new player %s.\n", &player.username);
            printf("Welcome %s!\n", &player.username);
        } else {
            player = players[playerIndexInList];
            printf("Welcome back %s!\n", &player.username);
            printPlayerScore(player);
        }
    } else {
        printf("Unknown command\n Please try again...\n");
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
        printf("\n");
        printf("Would you like to use a joker? (Y/N)\n");
        char useJoker;
        scanf("%c", &useJoker);
        scanf("%c", &useJoker);
        if (useJoker == 'Y' || useJoker == 'y') {
            printf("Which joker would you like to use?\n");
            if (jokerFriend == 1) {
                printf("1. Phone a Friend\n");
            }
            if (jokerFiftyFifty == 1) {
                printf("2. Fifty-Fifty\n");
            }
            if (jokerAudience == 1) {
                printf("3. Ask the Audience\n");
            }
            if (i >= 5 && jokerChangeQuestion == 1) {
                printf("4. Change the Question\n");
            }
            char joker;
            while (getchar() != '\n');
            scanf("%c", &joker);
            switch (joker) {
                case '1':
                    if (jokerFriend == 1) {
                        phoneFriendJoker();
                        printf("You have used the Phone a friend joker!\n");
                        jokerFriend = 0;
                    } else {
                        printf("You have already used the Phone a friend joker!\n");
                    }
                    break;
                case '2':
                    if (jokerFiftyFifty == 1) {
                        // TODO: implement logic for fifty-fifty joker
                        printf("You have used the Fifty-fifty joker!\n");
                        jokerFiftyFifty = 0;
                    } else {
                        printf("You have already used the Fifty-fifty joker!!\n");
                    }
                    break;
                case '3':
                    if (jokerAudience == 1) {
                        askTheAudience();
                        printf("You have used the Ask the Audience joker!\n");
                        jokerAudience = 0;
                    } else {
                        printf("You have already used the Ask the Audience joker!\n");
                    }
                    break;
                case '4':
                    if (jokerChangeQuestion == 1) {
                        // TODO: implement logic for change question joker
                        printf("You have used your change question joker!\n");
                        jokerChangeQuestion = 0;
                    } else {
                        printf("You have already used your change question joker!\n");
                    }
                    break;
                default:
                    printf("Unknown command...\n");
                    return -1;
            }
        }
        printf("Final answer? (A, B, C, D)\n");
        //TODO implement logic to give player chance to give up and walk away with the current score
        char playerAnswerLetter;
        while (getchar() != '\n');
        scanf("%c", &playerAnswerLetter);
        char playerAnswer[100];
        strcpy(playerAnswer, question.answers[answerIndexes[getArrayIndexFromAnswerLetter(playerAnswerLetter)]]);
        printf("-----------------------------------------------------\n");
        if (strcmp(playerAnswer, correctAnswer) == 0) {
            printf("Your final answer for %s was correct!\n", currentScore);
            if (i == 4 || i == 8) {
                printf("You have achieved a guaranteed value!\n");
                strcpy(guaranteedScore, currentScore);
            } else if (i == 11) {
                printf("You have won one million euros!\n");
//                TODO implement writing user to file and end game
                strcpy(player.score, currentScore);
                writePlayersToFile(&player, numberOfPlayers);
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

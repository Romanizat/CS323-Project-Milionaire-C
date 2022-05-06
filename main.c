#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * CS323 Projekat - Konzolna aplikacija Milioner - Marko Josifovic 4494
 *
 */


typedef struct question {
    char questionText[500];
    char answers[4][100];
    int correctAnswerIndex;
    int level; //1,2,3
} Question;

#define PLAYER_ATTR_MAX 100
typedef struct player {
    char username[PLAYER_ATTR_MAX];
    char score[PLAYER_ATTR_MAX];
} Player;

FILE *players_file;

// wrapper for debugger
void m_printf(const char *text, ...) {
    va_list args;
    va_start(args, text);
    vprintf(text, args);
    fflush(stdout);
    va_end(args);
}

char *getScoreLevelValues() {
    FILE *f = fopen("score_values.txt", "r");
    int stagesSize = 12;
    char *scores = (char *) calloc(12, 50);
    for (int i = 0; i < stagesSize; ++i) {
        fgets(&scores[i * 50], 50, f);
        scores[(strlen(&scores[i * 50]) + i * 50) - 1] = '\0';
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

    int playerCount;

    fscanf(players_file, "%d\n", &playerCount);
    if (playerCount > 20 || playerCount < 0) {
        playerCount = 0;
    }
    *newSize = playerCount;
    Player *playerList = (Player *) malloc(playerCount * sizeof(Player));

    for (int i = 0; i < playerCount; i++) {
        Player player;
        fgets(player.username, PLAYER_ATTR_MAX, players_file);
        player.username[strlen(player.username) - 1] = '\0';
        fgets(player.score, PLAYER_ATTR_MAX, players_file);
        player.score[strlen(player.score) - 1] = '\0';
        playerList[i] = player;
    }
    return playerList;
}

void writePlayersToFile(Player *players, int size) {
    ftruncate(fileno(players_file), 0);
    fprintf(players_file, "%d\n", size);
    for (int i = 0; i < size; ++i) {
        fprintf(players_file, "%s\n", players[i].username);
        fprintf(players_file, "%s\n", players[i].score);
    }
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
        m_printf("%s\n", questions[i].questionText);
        m_printf("%s\n", questions[i].answers[0]);
        m_printf("%s\n", questions[i].answers[1]);
        m_printf("%s\n", questions[i].answers[2]);
        m_printf("%s\n", questions[i].answers[3]);
        m_printf("%d\n", questions[i].correctAnswerIndex);
        m_printf("%d\n", questions[i].level);
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
    m_printf("------------------------------------------------------\n");
    m_printf("Friend: I think it's %c\n", getLetterFromNumber(getRandomNumber(0, 3)));
    m_printf("You: How certain are you?\n");
    m_printf("Friend: I'm %d%% certain\n", randomPercentage());
    m_printf("------------------------------------------------------\n");
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
    m_printf("------------------------------------------------------\n");
    m_printf("A) %d%%\n", arr[0]);
    m_printf("B) %d%%\n", arr[1]);
    m_printf("C) %d%%\n", arr[2]);
    m_printf("D) %d%%\n", arr[3]);
    m_printf("------------------------------------------------------\n");
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
    m_printf("Your previous score was: %s", &player.score);
}

void addNewPlayerToList(Player **players, int *size, Player player) {
    *size += 1;
    *players = (Player *) realloc(*players, sizeof(Player) * (*size));
    (*players)[*size - 1] = player;
}

void printPlayers(Player *players, int size) {
    for (int i = 0; i < size; i++) {
        m_printf("%s\n", players[i].username);
    }
}

int areJokersAvailable(int level, int jokerFriend,
                       int jokerFiftyFifty,
                       int jokerAudience,
                       int jokerChangeQuestion) {
    if (level < 5) {
        return (jokerFriend == 1 || jokerFiftyFifty == 1 || jokerAudience == 1);
    } else {
        return (jokerFriend == 1 || jokerFiftyFifty == 1 || jokerAudience == 1 || jokerChangeQuestion == 1);
    }
}

int main() {
    int numberOfQuestions;
    int numberOfLevel1Questions;
    int numberOfLevel2Questions;
    int numberOfLevel3Questions;
    int numberOfPlayers = 0;
    int playerIndexInList;
    Question *questions = getAllQuestions(&numberOfQuestions);
    int answerIndexes[] = {0, 1, 2, 3};
    players_file = fopen("players.txt", "a+");
    Player *players = getAllPlayers(&numberOfPlayers);

    Question *questionsLevel1 = getAllQuestionsForLevel(questions, numberOfQuestions, 1, &numberOfLevel1Questions);
    Question *questionsLevel2 = getAllQuestionsForLevel(questions, numberOfQuestions, 2, &numberOfLevel2Questions);
    Question *questionsLevel3 = getAllQuestionsForLevel(questions, numberOfQuestions, 3, &numberOfLevel3Questions);

    int jokerFriend = 1;
    int jokerFiftyFifty = 1;
    int jokerAudience = 1;
    int jokerChangeQuestion = 1; // after level 1

    m_printf("Welcome to Millionaire!\n");
    m_printf("Are you a new player? (Y/N)\n");
    fflush(stdout);
    char newPlayer;
    char username[100];
    scanf("%c", &newPlayer);
    Player player;
    if (newPlayer == 'Y' || newPlayer == 'y') {
        m_printf("Welcome new player!\n");
        m_printf("Please enter the username you will be using:\n");
        scanf("%s", &username);
        playerIndexInList = getPlayerIndexByUsername(players, numberOfPlayers, username);
        if (playerIndexInList == -1) {
            strcpy(player.username, username);
            m_printf("Welcome %s!\n", &player.username);
        } else {
            player = players[playerIndexInList];
            m_printf("Player already exists!\nWill continue game with player %s.\n", &player.username);
            printPlayerScore(player);
        }
    } else if (newPlayer == 'N' || newPlayer == 'n') {
        m_printf("Please enter your username:\n");
        scanf("%s", &username);
        playerIndexInList = getPlayerIndexByUsername(players, numberOfPlayers, username);
        if (playerIndexInList == -1) {
            m_printf("User with username %s not found\n", &username);
            strcpy(player.username, username);
            m_printf("Player doesn't exist!\nWill continue game as new player %s.\n", &player.username);
            m_printf("Welcome %s!\n", &player.username);
        } else {
            player = players[playerIndexInList];
            m_printf("Welcome back %s!\n", &player.username);
            printPlayerScore(player);
        }
    } else {
        m_printf("Unknown command\n Please try again...\n");
        return -1;
    }

    m_printf("To quit and walk away with the current score enter Q when answering the question\n");
    char *scores = getScoreLevelValues();
    char currentQuestionScore[100];
    char currentScore[100];
    char guaranteedScore[100];
    strcpy(guaranteedScore, "0€");
    strcpy(currentScore, "0€");
    for (int i = 0; i < 12; ++i) {
        Question question;
        strcpy(currentQuestionScore, &scores[i * 50]);
        m_printf("Question number %d for %s\n", i + 1, currentQuestionScore);
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
        m_printf("%s", question.questionText);
        char correctAnswer[100];
        strcpy(correctAnswer, question.answers[question.correctAnswerIndex]);
        shuffle(answerIndexes, 4);
        m_printf("A) %s\n", question.answers[answerIndexes[0]]);
        m_printf("B) %s\n", question.answers[answerIndexes[1]]);
        m_printf("C) %s\n", question.answers[answerIndexes[2]]);
        m_printf("D) %s\n", question.answers[answerIndexes[3]]);
        m_printf("\n");
        if (areJokersAvailable(i, jokerFriend, jokerFiftyFifty, jokerAudience, jokerChangeQuestion)) {
            m_printf("Would you like to use a joker? (Y/N)\n");
            char useJoker;
            scanf("%c", &useJoker);
            scanf("%c", &useJoker);
            if (useJoker == 'Y' || useJoker == 'y') {
                m_printf("Which joker would you like to use?\n");
                if (jokerFriend == 1) {
                    m_printf("1. Phone a Friend\n");
                }
                if (jokerFiftyFifty == 1) {
                    m_printf("2. Fifty-Fifty\n");
                }
                if (jokerAudience == 1) {
                    m_printf("3. Ask the Audience\n");
                }
                if (i >= 5 && jokerChangeQuestion == 1) {
                    m_printf("4. Change the Question\n");
                }
                char joker;
                while (getchar() != '\n');
                scanf("%c", &joker);
                switch (joker) {
                    case '1':
                        if (jokerFriend == 1) {
                            phoneFriendJoker();
                            m_printf("You have used the Phone a friend joker!\n");
                            jokerFriend = 0;
                        } else {
                            m_printf("You have already used the Phone a friend joker!\n");
                        }
                        break;
                    case '2':
                        if (jokerFiftyFifty == 1) {
                            // TODO: implement logic for fifty-fifty joker
                            m_printf("You have used the Fifty-fifty joker!\n");
                            jokerFiftyFifty = 0;
                        } else {
                            m_printf("You have already used the Fifty-fifty joker!!\n");
                        }
                        break;
                    case '3':
                        if (jokerAudience == 1) {
                            askTheAudience();
                            m_printf("You have used the Ask the Audience joker!\n");
                            jokerAudience = 0;
                        } else {
                            m_printf("You have already used the Ask the Audience joker!\n");
                        }
                        break;
                    case '4':
                        if (jokerChangeQuestion == 1) {
                            // TODO: implement logic for change question joker
                            m_printf("You have used your change question joker!\n");
                            jokerChangeQuestion = 0;
                        } else {
                            m_printf("You have already used your change question joker!\n");
                        }
                        break;
                    default:
                        m_printf("Unknown command...\n");
                        return -1;
                }
            }
        }
        m_printf("Final answer? (A, B, C, D)\n");
        char playerAnswerLetter;
        while (getchar() != '\n');
        scanf("%c", &playerAnswerLetter);
        if (playerAnswerLetter == 'q' || playerAnswerLetter == 'Q') {
            m_printf("You have decided to quit :(\nYou have won %s\n", currentScore);
            m_printf("Goodbye!\n");
            strcpy(player.score, currentQuestionScore);
            if (playerIndexInList == -1) {
                addNewPlayerToList(&players, &numberOfPlayers, player);
            } else {
                strcpy(players[playerIndexInList].score, guaranteedScore);
            }
            writePlayersToFile(players, numberOfPlayers);

            break;
        }
        char playerAnswer[100];
        strcpy(playerAnswer, question.answers[answerIndexes[getArrayIndexFromAnswerLetter(playerAnswerLetter)]]);
        m_printf("-----------------------------------------------------\n");
        if (strcmp(playerAnswer, correctAnswer) == 0) {
            m_printf("Your final answer for %s was correct!\n", currentQuestionScore);
            strcpy(currentScore, currentQuestionScore);
            if (i == 4 || i == 8) {
                m_printf("You have achieved a guaranteed value!\n");
                strcpy(guaranteedScore, currentQuestionScore);
            } else if (i == 11) {
                m_printf("You have won one million euros!\n");
                strcpy(player.score, currentQuestionScore);
                if (playerIndexInList == -1) {
                    addNewPlayerToList(&players, &numberOfPlayers, player);
                } else {
                    strcpy(players[playerIndexInList].score, guaranteedScore);
                }
                writePlayersToFile(players, numberOfPlayers);
            }
        } else {
            m_printf("Your final answer for %s was incorrect!\n", currentQuestionScore);
            m_printf("Your answer was %s\n", playerAnswer);
            m_printf("The correct answer was %s\n", correctAnswer);
            m_printf("You have earned a guaranteed sum of %s\n", guaranteedScore);
            strcpy(player.score, guaranteedScore);
            if (playerIndexInList == -1) {
                addNewPlayerToList(&players, &numberOfPlayers, player);
//                printPlayers(players, numberOfPlayers);
            } else {
                strcpy(players[playerIndexInList].score, guaranteedScore);
            }
            writePlayersToFile(players, numberOfPlayers);
            break;
        }
    }
    fflush(players_file);
    fclose(players_file);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // For sleep function

#define HIGHSCORE_FILE "highscore.txt"

void showCrackers() {
    printf("\n");
    for (int i = 0; i < 1; i++) {
        printf("\033[1;31m      *  *       *  *       *  *  \n");
        printf("    *      *   *      *   *      *\n");
        printf("  *          *          *         *\n");
        printf("    *      *   *      *   *      *\n");
        printf("      *  *       *  *       *  *  \033[0m\n");
        sleep(1);
        printf("\n\033[1;33m      *  *       *  *       *  *  \n");
        printf("    *      *   *      *   *      *\n");
        printf("  *          *          *         *\n");
        printf("    *      *   *      *   *      *\n");
        printf("      *  *       *  *       *  *  \033[0m\n");
        sleep(1);
        printf("\n\033[1;32m      *  *       *  *       *  *  \n");
        printf("    *      *   *      *   *      *\n");
        printf("  *          *          *         *\n");
        printf("    *      *   *      *   *      *\n");
        printf("      *  *       *  *       *  *  \033[0m\n");
        sleep(1);
    }
    printf("\n\033[1;36mCONGRATULATIONS! YOU WON\033[0m\n");
}

// Function to calculate the target score
int targetSet(int overs, int wickets, int run[]) {
    int totalBalls = 6 * overs;
    int remainingWickets = wickets;
    int target = 0;

    while (totalBalls > 0 && remainingWickets > 0) {
        int idx1 = rand() % 5;
        int idx2 = rand() % 5;

        if (run[idx1] != run[idx2]) {
            target += run[idx2];
        } else {
            remainingWickets--;
        }
        totalBalls--;
    }

    return target;
}

void printBanner() {
    printf("\033[1;33m*\033[0m\n");
    printf("\033[1;32m        WELCOME TO CRICKET GAME       \033[0m\n");
    printf("\033[1;33m*\033[0m\n");
}

void printDivider() {
    printf("\033[1;34m-----------------------------------\033[0m\n");
}

void updateHighScore(int score) {
    int highScore = 0;
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    if (file) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }

    if (score > highScore) {
        file = fopen(HIGHSCORE_FILE, "w");
        if (file) {
            fprintf(file, "%d", score);
            fclose(file);
            printf("\033[1;32mNew High Score: %d!\033[0m\n", score);
        }
    } else {
        printf("\033[1;33mCurrent High Score: %d\033[0m\n", highScore);
    }
}

void displayScoreboard(int userScore, int ballsPlayed, int sixes, int fours) {
    float runRate = (ballsPlayed > 0) ? ((float)userScore / ballsPlayed) * 6 : 0;
    printDivider();
    printf("\033[1;36m\n         FINAL SCOREBOARD\033[0m\n");
    printDivider();
    printf("\033[1;34m| %-20s | %-10s |\033[0m\n", "Stat", "Value");
    printDivider();
    printf("\033[1;34m| %-20s | %-10d |\033[0m\n", "Total Runs", userScore);
    printf("\033[1;34m| %-20s | %-10d |\033[0m\n", "Balls Played", ballsPlayed);
    printf("\033[1;34m| %-20s | %-10d |\033[0m\n", "Sixes Hit", sixes);
    printf("\033[1;34m| %-20s | %-10d |\033[0m\n", "Fours Hit", fours);
    printf("\033[1;34m| %-20s | %-10.2f |\033[0m\n", "Run Rate", runRate);
    printDivider();
}

int main() {
    srand(time(0));
    int run[5] = {1, 2, 3, 4, 6};
    int userRuns, computerChoice;

    while (1) {
        printBanner();
        int totalOvers, totalWickets, userScore = 0, target;
        int totalBalls, ballsPlayed = 0, sixes = 0, fours = 0;

        printf("\033[1;34mEnter number of overs: \033[0m");
        scanf("%d", &totalOvers);
        printf("\033[1;34m\nEnter number of wickets: \033[0m");
        scanf("%d", &totalWickets);

        target = targetSet(totalOvers, totalWickets, run);
        printf("\033[1;32m\nYour target is %d runs\033[0m\n", target);

        totalBalls = 6 * totalOvers;

        while (totalBalls > 0 && totalWickets > 0) {
            printDivider();
            printf("\033[1;34m\nEnter your run for this ball: \033[0m");
            scanf("%d", &userRuns);

            // Check if userRuns is between 1 and 6, and not 5
            if (userRuns < 0 || userRuns > 6 || userRuns == 5) {
                printf("\033[1;31mInvalid entry! Please enter a value between 1 and 6 (excluding 5).\033[0m\n");
                continue;  // Skip this ball and ask again for valid input
            }

            computerChoice = run[rand() % 5];
            printf("Computer's choice: \033[1;33m%d\033[0m\n", computerChoice);

            if (computerChoice != userRuns) {
                userScore += userRuns;
                ballsPlayed++;
                if (userRuns == 6) sixes++;
                if (userRuns == 4) fours++;
                printf("Runs scored: \033[1;32m%d\033[0m\n", userScore);

                if (userScore > target) {
                    printf("\033[1;32m\nCongratulations! You have won the match!\033[0m\n");
                    showCrackers();
                    break;
                }
            } else {
                totalWickets--;
                printf("\033[1;31m\nOops! You are out!\033[0m\n");
                if (totalWickets == 0) {
                    printf("\033[1;31m\nGame Over! You've lost all wickets.\033[0m\n");
                    break;
                }
            }
            totalBalls--;
        }

        displayScoreboard(userScore, ballsPlayed, sixes, fours);
        updateHighScore(userScore);

        char choice;
        printDivider();
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            printf("\033[1;34mThank you for playing! Goodbye.\033[0m\n");
            break;
        }
    }
    return 0;
}

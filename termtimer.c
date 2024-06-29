#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIGITS_COUNT 8  // HH:MM:SS

#define DIGIT_ROWS 5
#define DIGIT_COLS 3

#define TIMER_HEIGHT DIGIT_ROWS
#define TIMER_WIDTH (DIGIT_COLS * DIGITS_COUNT)

const int DIGIT_BITMAPS[11] = {
    31599,
    4681,
    29671,
    29647,
    23497,
    31183,
    31215,
    29257,
    31727,
    31695,
    1040,  // colon
};

void display_timer(int remaining_seconds) {
    int hours = remaining_seconds / 3600;
    int minutes = (remaining_seconds % 3600) / 60;
    int seconds = remaining_seconds % 60;

    int timer_digits[DIGITS_COUNT];
    timer_digits[0] = hours / 10;
    timer_digits[1] = hours % 10;
    timer_digits[2] = 10;  // colon
    timer_digits[3] = minutes / 10;
    timer_digits[4] = minutes % 10;
    timer_digits[5] = 10;  // colon
    timer_digits[6] = seconds / 10;
    timer_digits[7] = seconds % 10;

    for (int y = 0; y < TIMER_HEIGHT; y++) {
        for (int x = 0; x < TIMER_WIDTH; x++) {
            int i = x / DIGIT_COLS;
            int digit_bitmap = DIGIT_BITMAPS[timer_digits[i]];
            int dx = x % DIGIT_COLS;
            int bit_pos = (DIGIT_ROWS - y) * DIGIT_COLS - dx - 1;
            int bit = (digit_bitmap >> bit_pos) & 1;
            if (bit) {
                printf("██");
            } else {
                printf("  ");
            }
            // Add padding between digits
            if (dx == 2 && (i % 3 == 0)) {
                printf(" ");
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void clear_display() {
    printf("\x1b[%dA\x1b[%dD", TIMER_HEIGHT, TIMER_WIDTH);
}

void show_cursor() {
    printf("\x1b[?25h");
}

void hide_cursor() {
    printf("\x1b[?25l");
}

void sigint_handler(int signo) {
    show_cursor();
    exit(signo);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seconds>\n", argv[0]);
        return 1;
    }

    int total_seconds = atoi(argv[1]);
    if (total_seconds <= 0) {
        fprintf(
            stderr, "%s: invalid number of seconds: '%s'\n", argv[0], argv[1]
        );
        return 1;
    }

    signal(SIGINT, sigint_handler);

    hide_cursor();

    int remaining_seconds = total_seconds;
    while (remaining_seconds >= 0) {
        display_timer(remaining_seconds);
        if (remaining_seconds) {
            clear_display();
        }
        sleep(1);
        remaining_seconds--;
    }

    show_cursor();

    return 0;
}

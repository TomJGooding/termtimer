#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TIMER_HEIGHT 1
#define TIMER_WIDTH 8

void display_timer(int remaining_seconds) {
    int hours = remaining_seconds / 3600;
    int minutes = (remaining_seconds % 3600) / 60;
    int seconds = remaining_seconds % 60;

    char digit_chars[11] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':'
    };

    int timer_digits[8];
    timer_digits[0] = hours / 10;
    timer_digits[1] = hours % 10;
    timer_digits[2] = 10;  // colon
    timer_digits[3] = minutes / 10;
    timer_digits[4] = minutes % 10;
    timer_digits[5] = 10;  // colon
    timer_digits[6] = seconds / 10;
    timer_digits[7] = seconds % 10;

    printf("\r");
    // printf("%02d:%02d:%02d", hours, minutes, seconds);
    for (size_t y = 0; y < TIMER_HEIGHT; y++) {
        for (size_t x = 0; x < TIMER_WIDTH; x++) {
            printf("%c", digit_chars[timer_digits[x]]);
        }
    }
    fflush(stdout);
}

void show_cursor() {
    printf("\x1b[?25h");
}

void hide_cursor() {
    printf("\x1b[?25l");
}
void sigint_handler(int signo) {
    printf("\n");
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
        sleep(1);
        remaining_seconds--;
    }

    printf("\n");
    show_cursor();

    return 0;
}

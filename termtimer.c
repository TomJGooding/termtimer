#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    int remaining_seconds = total_seconds;
    while (remaining_seconds >= 0) {
        int hours = remaining_seconds / 3600;
        int minutes = (remaining_seconds % 3600) / 60;
        int seconds = remaining_seconds % 60;

        printf("%02d:%02d:%02d\n", hours, minutes, seconds);

        sleep(1);
        remaining_seconds--;
    }

    return 0;
}

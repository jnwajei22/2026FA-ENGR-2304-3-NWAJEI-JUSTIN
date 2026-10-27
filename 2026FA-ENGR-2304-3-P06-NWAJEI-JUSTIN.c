/* 2026FA-ENGR-2304-3-P06-NWAJEI-JUSTIN */
/* Programming Assignment 06: Statistical Data Analysis */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ROWS 20000
#define BALLS 6

typedef struct {
    int year, month, day;
    int ball[BALLS];
} Draw;

static Draw draws[MAX_ROWS];
static int count = 0;

int load_data(const char *filename) {
    FILE *fp = fopen(filename, "r");
    char line[256];
    int bad = 0;
    if (!fp) { perror("Unable to open data file"); return 0; }
    count = 0;
    while (fgets(line, sizeof(line), fp)) {
        Draw d;
        int n = sscanf(line, " %d , %d , %d , %d , %d , %d , %d , %d , %d",
            &d.year, &d.month, &d.day, &d.ball[0], &d.ball[1],
            &d.ball[2], &d.ball[3], &d.ball[4], &d.ball[5]);
        if (n != 9) {
            n = sscanf(line, " %d %d %d %d %d %d %d %d %d",
                &d.year, &d.month, &d.day, &d.ball[0], &d.ball[1],
                &d.ball[2], &d.ball[3], &d.ball[4], &d.ball[5]);
        }
        if (n != 9) {
            if (strncmp(line, "Year", 4) != 0 && line[0] != '\n') bad++;
            continue;
        }
        if (count >= MAX_ROWS) { puts("Row limit reached."); break; }
        draws[count++] = d;
    }
    fclose(fp);
    printf("Loaded %d draws; skipped %d invalid lines.\n", count, bad);
    return count;
}

void print_draw(const Draw *d) {
    printf("%04d-%02d-%02d :", d->year, d->month, d->day);
    for (int j = 0; j < BALLS; j++) printf(" %d", d->ball[j]);
    putchar('\n');
}

void report(void) {
    if (!count) { puts("Load a file first."); return; }
    puts("\nSTATISTICAL REPORT (each column analyzed separately)");
    puts("Column    Count     Mean      Std Dev    Min   Max");
    for (int j = 0; j < BALLS; j++) {
        double sum = 0.0, sumsq = 0.0;
        int min = draws[0].ball[j], max = min;
        for (int i = 0; i < count; i++) {
            int x = draws[i].ball[j];
            sum += x;
            sumsq += (double)x * x;
            if (x < min) min = x;
            if (x > max) max = x;
        }
        double mean = sum / count;
        double variance = count > 1 ? (sumsq - sum * sum / count) / (count - 1) : 0.0;
        if (variance < 0) variance = 0;
        printf("L%-8d %-9d %-9.3f %-10.3f %-5d %d\n",
               j + 1, count, mean, sqrt(variance), min, max);
    }
    puts("Standard deviation is the sample standard deviation (n-1).");
}

void search_date(void) {
    int y, m, d, matches = 0;
    printf("Enter date (YYYY MM DD): ");
    if (scanf("%d %d %d", &y, &m, &d) != 3) { puts("Invalid date."); return; }
    for (int i = 0; i < count; i++) {
        if (draws[i].year == y && draws[i].month == m && draws[i].day == d) {
            print_draw(&draws[i]); matches++;
        }
    }
    if (!matches) puts("No draws found for that date.");
}

void search_number(void) {
    int target, matches = 0;
    printf("Enter lottery number to search: ");
    if (scanf("%d", &target) != 1) { puts("Invalid number."); return; }
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < BALLS; j++) {
            if (draws[i].ball[j] == target) {
                print_draw(&draws[i]); matches++; break;
            }
        }
    }
    printf("Matching draws: %d\n", matches);
}

int main(void) {
    int choice;
    char filename[256];
    puts("ENGR 2304 - P06 Statistical Data Analysis");
    puts("Enter CSV or space-delimited data filename:");
    if (!fgets(filename, sizeof(filename), stdin)) return 1;
    filename[strcspn(filename, "\r\n")] = '\0';
    load_data(filename);
    for (;;) {
        puts("\n1. Load/reload data file\n2. Statistical report\n3. Search by date\n4. Search by number\n5. Show first five draws\n0. Exit");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { puts("Invalid menu input."); return 1; }
        switch (choice) {
            case 1:
                printf("Filename: ");
                if (scanf("%255s", filename) == 1) load_data(filename);
                break;
            case 2: report(); break;
            case 3: search_date(); break;
            case 4: search_number(); break;
            case 5:
                for (int i = 0; i < count && i < 5; i++) print_draw(&draws[i]);
                break;
            case 0: puts("Goodbye."); return 0;
            default: puts("Choose an option from the menu.");
        }
    }
}

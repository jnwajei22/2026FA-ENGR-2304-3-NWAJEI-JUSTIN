/* 2026FA-ENGR-2304-3-P04-NWAJEI-JUSTIN */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double truncate3(double value)
{
    return trunc(value * 1000.0) / 1000.0;
}

int correctAnswer(double userValue, double correctValue)
{
    userValue = truncate3(userValue);
    correctValue = truncate3(correctValue);

    return fabs(userValue - correctValue) < 0.000001;
}

int main(void)
{
    int choice = 1;

    srand((unsigned int)time(NULL));

    while (choice == 1)
    {
        int Vs;
        int R1;
        int R2;
        int R3;

        double D;
        double Di1;
        double Di2;

        double i1;
        double i2;

        double v1;
        double v2;
        double v3;

        double userI1;
        double userI2;
        double userV1;
        double userV2;
        double userV3;

        /*
         * Crowder requires:
         * Vs <= 10 V
         * R1, R2, R3 <= 20 ohms
         *
         * Values begin at 1 so no resistor is zero.
         */

        Vs = rand() % 10 + 1;

        R1 = rand() % 20 + 1;
        R2 = rand() % 20 + 1;
        R3 = rand() % 20 + 1;

        /*
         * Matrix:
         *
         * | R1 + R2    -R2      | | i1 |   | Vs |
         * | -R2        R2 + R3  | | i2 | = | 0  |
         *
         * Solve using Cramer's Rule.
         */

        D =
            (R1 + R2) * (R2 + R3)
            - (R2 * R2);

        Di1 =
            Vs * (R2 + R3);

        Di2 =
            Vs * R2;

        i1 = Di1 / D;
        i2 = Di2 / D;

        /*
         * Voltage calculations using Ohm's Law.
         */

        v1 = R1 * i1;

        /*
         * Current through R2 is the difference
         * between the two mesh currents.
         */

        v2 = R2 * (i1 - i2);

        v3 = R3 * i2;

        /*
         * Assignment requires truncation,
         * not rounding.
         */

        i1 = truncate3(i1);
        i2 = truncate3(i2);

        v1 = truncate3(v1);
        v2 = truncate3(v2);
        v3 = truncate3(v3);

        printf("\n");
        printf("ENGR 2304 - Electrical Circuit Generator\n");
        printf("-----------------------------------------\n");

        printf("Vs = %d V\n", Vs);
        printf("R1 = %d ohms\n", R1);
        printf("R2 = %d ohms\n", R2);
        printf("R3 = %d ohms\n", R3);

        printf("\n");
        printf("Enter each answer truncated to three decimal places.\n");

        printf("i1 (A): ");
        scanf("%lf", &userI1);

        printf("i2 (A): ");
        scanf("%lf", &userI2);

        printf("v1 (V): ");
        scanf("%lf", &userV1);

        printf("v2 (V): ");
        scanf("%lf", &userV2);

        printf("v3 (V): ");
        scanf("%lf", &userV3);

        printf("\n");
        printf("Results\n");
        printf("-------\n");

        if (correctAnswer(userI1, i1))
            printf("i1: Correct\n");
        else
            printf("i1: Incorrect. Correct answer = %.3f A\n", i1);

        if (correctAnswer(userI2, i2))
            printf("i2: Correct\n");
        else
            printf("i2: Incorrect. Correct answer = %.3f A\n", i2);

        if (correctAnswer(userV1, v1))
            printf("v1: Correct\n");
        else
            printf("v1: Incorrect. Correct answer = %.3f V\n", v1);

        if (correctAnswer(userV2, v2))
            printf("v2: Correct\n");
        else
            printf("v2: Incorrect. Correct answer = %.3f V\n", v2);

        if (correctAnswer(userV3, v3))
            printf("v3: Correct\n");
        else
            printf("v3: Incorrect. Correct answer = %.3f V\n", v3);

        printf("\n");
        printf("1. Generate another circuit\n");
        printf("2. Exit\n");
        printf("Select an option: ");

        scanf("%d", &choice);

        while (choice != 1 && choice != 2)
        {
            printf("Invalid choice. Enter 1 or 2: ");
            scanf("%d", &choice);
        }
    }

    printf("\nExiting program.\n");

    return 0;
}
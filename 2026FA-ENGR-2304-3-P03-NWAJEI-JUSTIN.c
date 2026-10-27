/* 2026FA-ENGR-2304-3-P03-NWAJEI-JUSTIN */

#include <stdio.h>
#include <math.h>

#define EPSILON 1e-9

double det2(double a, double b, double c, double d)
{
    return a * d - b * c;
}

double det3(double m[3][3])
{
    return m[0][0] * det2(m[1][1], m[1][2], m[2][1], m[2][2])
         - m[0][1] * det2(m[1][0], m[1][2], m[2][0], m[2][2])
         + m[0][2] * det2(m[1][0], m[1][1], m[2][0], m[2][1]);
}

void solve2(void)
{
    double a1, b1, c1;
    double a2, b2, c2;
    double D, Dx, Dy;

    printf("\nEnter coefficients for equations of the form ax + by = c.\n");

    printf("Equation 1 (a b c): ");
    scanf("%lf %lf %lf", &a1, &b1, &c1);

    printf("Equation 2 (a b c): ");
    scanf("%lf %lf %lf", &a2, &b2, &c2);

    D  = det2(a1, b1, a2, b2);
    Dx = det2(c1, b1, c2, b2);
    Dy = det2(a1, c1, a2, c2);

    printf("\nD  = %.3f\n", D);
    printf("Dx = %.3f\n", Dx);
    printf("Dy = %.3f\n", Dy);

    if (fabs(D) < EPSILON) {
        printf("No unique solution exists because D = 0.\n");
        return;
    }

    printf("x = %.3f\n", Dx / D);
    printf("y = %.3f\n", Dy / D);
}

void copyMatrix(double dest[3][3], double src[3][3])
{
    int r, c;

    for (r = 0; r < 3; ++r) {
        for (c = 0; c < 3; ++c) {
            dest[r][c] = src[r][c];
        }
    }
}

void solve3(void)
{
    double A[3][3];
    double B[3];
    double temp[3][3];

    double D, Dx, Dy, Dz;
    int r;

    printf("\nEnter coefficients for equations of the form ax + by + cz = d.\n");

    for (r = 0; r < 3; ++r) {
        printf("Equation %d (a b c d): ", r + 1);

        scanf("%lf %lf %lf %lf",
              &A[r][0],
              &A[r][1],
              &A[r][2],
              &B[r]);
    }

    D = det3(A);

    copyMatrix(temp, A);
    for (r = 0; r < 3; ++r)
        temp[r][0] = B[r];

    Dx = det3(temp);

    copyMatrix(temp, A);
    for (r = 0; r < 3; ++r)
        temp[r][1] = B[r];

    Dy = det3(temp);

    copyMatrix(temp, A);
    for (r = 0; r < 3; ++r)
        temp[r][2] = B[r];

    Dz = det3(temp);

    printf("\nD  = %.3f\n", D);
    printf("Dx = %.3f\n", Dx);
    printf("Dy = %.3f\n", Dy);
    printf("Dz = %.3f\n", Dz);

    if (fabs(D) < EPSILON) {
        printf("No unique solution exists because D = 0.\n");
        return;
    }

    printf("x = %.3f\n", Dx / D);
    printf("y = %.3f\n", Dy / D);
    printf("z = %.3f\n", Dz / D);
}

int main(void)
{
    int choice;

    do {
        printf("\nENGR 2304 - Cramer's Rule Solver\n");
        printf("1. Solve two equations\n");
        printf("2. Solve three equations\n");
        printf("3. Exit\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                solve2();
                break;

            case 2:
                solve3();
                break;

            case 3:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid selection.\n");
        }

    } while (choice != 3);

    return 0;
}
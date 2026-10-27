/* 2026FA-ENGR-2304-3-P02-NWAJEI-JUSTIN */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void decimalToSignedMagnitude(int value) {
    int magnitude = value < 0 ? -value : value;
    int bits[8] = {0};
    int i;

    bits[0] = value < 0 ? 1 : 0;

    for (i = 7; i >= 1; --i) {
        bits[i] = magnitude % 2;
        magnitude /= 2;
    }

    printf("8-bit signed-magnitude binary: ");
    for (i = 0; i < 8; ++i) {
        printf("%d", bits[i]);
    }
    printf("\n");
}

int binarySignedMagnitudeToDecimal(const char *binary, int *valid) {
    int i, magnitude = 0;
    size_t len = strlen(binary);

    *valid = 0;

    if (len != 8) {
        return 0;
    }

    for (i = 0; i < 8; ++i) {
        if (binary[i] != '0' && binary[i] != '1') {
            return 0;
        }
    }

    for (i = 1; i < 8; ++i) {
        magnitude = magnitude * 2 + (binary[i] - '0');
    }

    *valid = 1;

    return binary[0] == '1' ? -magnitude : magnitude;
}

int hexValue(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }

    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    return -1;
}

void printNibble(int value) {
    int bit;

    for (bit = 3; bit >= 0; --bit) {
        putchar((value & (1 << bit)) ? '1' : '0');
    }
}

void hexToBinary(const char *input) {
    const char *p = input;
    const char *q;

    int before = 0;
    int after = 0;
    int seenPoint = 0;

    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        p += 2;
    } else {
        printf("Hex input must begin with 0x.\n");
        return;
    }

    if (*p == '\0') {
        printf("Invalid hexadecimal input.\n");
        return;
    }

    for (q = p; *q; ++q) {
        if (*q == '.') {
            if (seenPoint) {
                printf("Invalid hexadecimal input.\n");
                return;
            }

            seenPoint = 1;
        }
        else if (hexValue(*q) >= 0) {
            if (seenPoint) {
                ++after;
            } else {
                ++before;
            }
        }
        else {
            printf("Invalid hexadecimal input.\n");
            return;
        }
    }

    if (before > 8 || after > 8) {
        printf("Hex input exceeds 8 digits on one side of the decimal point.\n");
        return;
    }

    printf("Binary: ");

    for (q = p; *q; ++q) {
        if (*q == '.') {
            putchar('.');
        } else {
            printNibble(hexValue(*q));
        }
    }

    printf("\n");
}

int binaryGroupValue(const char *bits, int start, int count) {
    int i;
    int value = 0;

    for (i = 0; i < count; ++i) {
        value = value * 2 + (bits[start + i] - '0');
    }

    return value;
}

void binaryToHex(const char *input) {
    char left[33] = {0};
    char right[33] = {0};

    int leftLen = 0;
    int rightLen = 0;
    int seenPoint = 0;
    int i;

    for (i = 0; input[i]; ++i) {
        if (input[i] == '.') {
            if (seenPoint) {
                printf("Invalid binary input.\n");
                return;
            }

            seenPoint = 1;
        }
        else if (input[i] == '0' || input[i] == '1') {
            if (!seenPoint) {
                if (leftLen >= 32) {
                    printf("Binary input exceeds 32 bits before the decimal point.\n");
                    return;
                }

                left[leftLen++] = input[i];
            }
            else {
                if (rightLen >= 32) {
                    printf("Binary input exceeds 32 bits after the decimal point.\n");
                    return;
                }

                right[rightLen++] = input[i];
            }
        }
        else {
            printf("Invalid binary input.\n");
            return;
        }
    }

    if (leftLen == 0 && rightLen == 0) {
        printf("Invalid binary input.\n");
        return;
    }

    int leftPad = (4 - leftLen % 4) % 4;
    int rightPad = (4 - rightLen % 4) % 4;

    char paddedLeft[37] = {0};
    char paddedRight[37] = {0};

    int pos = 0;

    for (i = 0; i < leftPad; ++i) {
        paddedLeft[pos++] = '0';
    }

    for (i = 0; i < leftLen; ++i) {
        paddedLeft[pos++] = left[i];
    }

    int paddedLeftLen = pos;

    pos = 0;

    for (i = 0; i < rightLen; ++i) {
        paddedRight[pos++] = right[i];
    }

    for (i = 0; i < rightPad; ++i) {
        paddedRight[pos++] = '0';
    }

    int paddedRightLen = pos;

    printf("Hexadecimal: 0x");

    if (paddedLeftLen == 0) {
        printf("0");
    }
    else {
        for (i = 0; i < paddedLeftLen; i += 4) {
            int value = binaryGroupValue(paddedLeft, i, 4);
            printf("%X", value);
        }
    }

    if (rightLen > 0) {
        putchar('.');

        for (i = 0; i < paddedRightLen; i += 4) {
            int value = binaryGroupValue(paddedRight, i, 4);
            printf("%X", value);
        }
    }

    printf("\n");
}

int main(void) {
    int choice;
    int decimal;
    char input[100];

    while (1) {
        printf("\nENGR 2304 - Number Conversion Program\n");
        printf("1. Decimal to Binary\n");
        printf("2. Binary to Decimal\n");
        printf("3. Binary to Hexadecimal\n");
        printf("4. Hexadecimal to Binary\n");
        printf("5. Exit\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid menu selection.\n");

            while (getchar() != '\n') {
            }

            continue;
        }

        if (choice == 5) {
            printf("Exiting program.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter a decimal integer from -127 to 127: ");

                if (scanf("%d", &decimal) != 1 ||
                    decimal < -127 ||
                    decimal > 127) {

                    printf("Invalid decimal input.\n");

                    while (getchar() != '\n') {
                    }
                }
                else {
                    decimalToSignedMagnitude(decimal);
                }

                break;

            case 2: {
                int valid;
                int result;

                printf("Enter an 8-bit signed-magnitude binary number: ");
                scanf("%99s", input);

                result = binarySignedMagnitudeToDecimal(input, &valid);

                if (!valid) {
                    printf("Invalid binary input. Enter exactly 8 bits.\n");
                }
                else {
                    printf("Decimal: %d\n", result);
                }

                break;
            }

            case 3:
                printf("Enter a positive binary number "
                       "(up to 32 bits on each side of '.'): ");

                scanf("%99s", input);
                binaryToHex(input);

                break;

            case 4:
                printf("Enter a positive hexadecimal number beginning with 0x "
                       "(up to 8 digits on each side of '.'): ");

                scanf("%99s", input);
                hexToBinary(input);

                break;

            default:
                printf("Invalid menu selection.\n");
        }
    }

    return 0;
}
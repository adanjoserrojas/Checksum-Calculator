#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes
unsigned int CheckSum8Bit(const char * message);
unsigned int CheckSum16Bit(const char * message);
unsigned int CheckSum32Bit(const char * message);
void Print(const char * plain);
char * Padding(const char * txt, int padding);

// Function to calculate the 8-bit checksum
unsigned int CheckSum8Bit(const char * message) {

    if(message == NULL) return 0;
    unsigned int checksum = 0;

    for (int i = 0; i < strlen(message); i++) { // add the ascii to the checksum
        checksum += message[i];
    }
    return checksum;
}

// Function to calculate the 16-bit checksum
unsigned int CheckSum16Bit(const char * message) {

    if(message == NULL) return 0;
    unsigned short checksum = 0;

    // Iterate every in clocks of two
    for (int i = 0; i < strlen(message); i += 2) {
        unsigned int value;

        if (i + 1 < strlen(message)) { //check if is in the length iof the padded string
            value = (message[i] << 8) + message[i + 1]; // combine both characters, 1st shifted 8 bits
        } else {
            value = (message[i] << 8); //if there is not next, just shift curr
        }
        checksum += value;
    }
    return checksum;
}

// Function to calculate the 32-bit checksum
unsigned int CheckSum32Bit(const char * message) {

    if(message == NULL) return 0; // Check for NULL
    unsigned int checksum = 0;

    // For the length of the padded message iterate in blocks of 4
    for (int i = 0; i < strlen(message); i += 4) {
        unsigned int value = (message[i] << 24); // 32 bit block, shifted 8 bit to the right

        if (i + 1 < strlen(message)) { // if the second character in the block exists, shift by 16 and add
            value += message[i + 1] << 16;
        } if (i + 2 < strlen(message)) { // if the third character in the block exists, shift by 8 and add
            value += message[i + 2] << 8;
        } if (i + 3 < strlen(message)) { // if the fourth character in the block exists, add
            value += message[i + 3];
        }
        checksum += value;
    }
    return checksum;
}

// Function to print the input message with 80 characters per line
void Print(const char * plain) {

    if(plain == NULL) return; // Check for Null, great habit

    printf("\n"); //  First thing in the output is a \n
    for (int i = 0; i < strlen(plain); i++) {
        printf("%c", plain[i]);
        if ((i + 1) % 80 == 0) { // print character by character and new line when 80 characters per line
            printf("\n");
        }
    }
    if (strlen(plain) % 80 != 0) {
        printf("\n"); // formating when last line is not multiple of 80 basically
    }
}

// Function to pad the txt with 'X' characters for checksum calculations
char * Padding(const char * txt, int padding) {

    unsigned long long len = strlen(txt);
    char * padded_txt = calloc(len + padding + 1, sizeof(char));

    strcpy(padded_txt, txt);
    for (int i = 0; i < padding; i++) {
        padded_txt[len + i] = 'X';
    }
    return padded_txt;
}

int main(int argc, char *argv[]) {

    // Variables :)
    const char * file1 = argv[1];
    int s = atoi(argv[2]);
    char * plain_txt = NULL;
    int txt_len;
    unsigned long result;

    // Open file and assign to file
    FILE * file = fopen(file1, "r");

    // Get the lwngth and read the file
    fseek(file, 0, 2);
    txt_len = ftell(file);
    fseek(file, 0, 0);

    // Initalize ptr plain_txt, and read into plaintxt, and close
    plain_txt = calloc(txt_len + 1, sizeof(char));
    fread(plain_txt, 1, txt_len, file);
    fclose(file);

    // Print and calculate the checksum based on the s, 8, 16 0r 32
    if (s == 8) {
        result = CheckSum8Bit(plain_txt);
        Print(plain_txt);
        printf("%2d bit checksum is %8lx for all %4lu chars\n", s, result & 0xFF, strlen(plain_txt));

    } else if (s == 16) {
        //Call the padding before checksum
        if (strlen(plain_txt) % 2 != 0) {
            char * padded_message = Padding(plain_txt, 1);
            free(plain_txt);
            plain_txt = padded_message;
        }
        result = CheckSum16Bit(plain_txt);
        Print(plain_txt);
        printf("%2d bit checksum is %8lx for all %4lu chars\n", s, result & 0xFFFF, strlen(plain_txt));

    } else if (s == 32) {
        //Call the padding before checksum
        int count_p = (4 - (strlen(plain_txt) % 4)) % 4;
        if (count_p > 0) {
            char * padded_message = Padding(plain_txt, count_p);
            free(plain_txt);
            plain_txt = padded_message;
        }
        result = CheckSum32Bit(plain_txt);
        Print(plain_txt);
        printf("%2d bit checksum is %8lx for all %4lu chars\n", s, result & 0xFFFFFFFF, strlen(plain_txt));

    } else { // Else valid formats are 8, 16, 32
        fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
        free(plain_txt);
        return 0;
    }
    free(plain_txt);
    return 0;
}

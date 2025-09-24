#ifndef __linux__
#error This program is only meant to run on linux
#endif


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define RANDOM_HIGH_QUALITY "/dev/random"
#define RANDOM_LOWER_QUALITY "/dev/urandom"

#define ENTROPY_POOL_SIZE 4096
char entropy_pool[ENTROPY_POOL_SIZE];

int64_t bits = 0;
char random_file[64];

void check_file(const char* arg) {
    if (strncmp(arg, "high", 8) == 0) {
        strcpy(random_file,RANDOM_HIGH_QUALITY);
        return;
    }
    if (strncmp(arg, "low", 8) == 0) {
        strcpy(random_file,RANDOM_LOWER_QUALITY);
        return;
    }
    printf("Invalid quality identifier!\n");
    exit(1);
}
void warn_user(char* warning) {
    printf("WARNING: %s\n", warning);
}
void help_message() {
    printf("Usage: ./keymaker num_bits quality(high/low)\n");
    exit(1);
}

void check_help(int argc, const char* argv[]) {
    if (argc >= 2 && strncmp(argv[1], "--help", 8) == 0) {
        help_message();
    }
}

void chip_security_checks() {
#ifdef __x86_64__
    warn_user(
        "Be wary using modern intel or amd chips, they have a known management engine backdoor. If this secret is of life or death stakes, do not use this machine");
#elifdef __aarch64__
    warn_user(
        "Be wary using modern arm chips, many of them have a known management engine backdoor. If this secret is of life or death stakes, do not use this machine");
#endif
}


int main(const int argc, const char* argv[]) {

    check_help(argc, argv);

    if (argc > 3 || argc < 3) {
        help_message();
    }

    chip_security_checks();

    char* endptr;

    bits = strtol(argv[1], &endptr, 10);

    if (bits < 0) {
        printf("Invalid bit count! Are you sure it's a numerical value?\n");
        exit(1);
    }

    if (bits % 8 != 0) {
        printf(
            "Num bits required must be divisible by 8! Who the fuck even needs sub byte quantities of bits for a key!\n");
        exit(1);
    }

    uint64_t bytes = bits / 8;

    check_file(argv[2]);


    FILE* f = fopen(random_file, "rb");

    if (f == NULL) {
        printf("Failed to open file %s\n", random_file);
        exit(1);
    }

    int64_t ret = fread(entropy_pool,ENTROPY_POOL_SIZE, bytes, f);

    if (ret < 0) {
        printf("Failed to read file %s\n", random_file);
        exit(1);
    }
    printf("Your key is 0x");
    for (int i = 0; i < bytes; i++) {
        printf("%02X", entropy_pool[i] & 0xFF);
    }

    return 0;
}

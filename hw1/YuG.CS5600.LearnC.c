/*
* YuG.CS5600.LearnC.c / Program in C
*
* Guoqing Yu / CS5600 / Northeastern University
* Fall 2023 / Sep 10, 2023
*
*/
#include <stdio.h>
#include "mpg2km.h"

/* This function checks if two floats are close enough */
int checkEqual(float expected, float actual) {
    float diff = expected - actual;
    if (diff > -0.01 && diff < 0.01) {
        printf("  Pass\n");
    } else {
        printf("  Fail\n");
    }
    return 0;
}

int testMpg2kml(float mpg, float expectedKml) {
    float actualKml = mpg2kml(mpg);
    if (actualKml == -1) {
        printf("%.4f mpg is invalid!", mpg);
    } else {
        printf("%.4f miles per gallon -> kilometers per liter: expected %.4f, actual %.4f", mpg, expectedKml, actualKml);
    }
    checkEqual(expectedKml, actualKml);
    return 0;
}

int testMpg2lphm(float mpg, float expectedLphm) {
    float actualLphm = mpg2lphm(mpg);
    if (actualLphm == -1) {
        printf("%.4f mpg is invalid!", mpg);
    } else {
        printf("%.4f miles per gallon -> liters per 100 kilometers: expected %.4f, actual %.4f", mpg, expectedLphm, actualLphm);
    }
    checkEqual(expectedLphm, actualLphm);
    return 0;
}

int testLph2mpg(float lph, float expectedMpg) {
    float actualMpg = lph2mpg(lph);
    if (actualMpg == -1) {
        printf("%.4f lph is invalid!", lph);
    } else {
        printf("%.4f liters per 100 kilometers -> miles per gallon: expected %.4f, actual %.4f", lph, expectedMpg, actualMpg);
    }
    checkEqual(expectedMpg, actualMpg);
    return 0;
}

int main() {
    printf("Testing mpg2kml...\n");
    testMpg2kml(0, 0);
    testMpg2kml(-5, -1);
    testMpg2kml(10, 4.25);
    testMpg2kml(27.6, 11.73);
    testMpg2kml(87.91, 37.37);
    printf("\n");

    printf("Testing mpg2lphm...\n");
    testMpg2lphm(0, -1);
    testMpg2lphm(-5, -1);
    testMpg2lphm(10, 23.52);
    testMpg2lphm(18.6, 12.65);
    testMpg2lphm(29.57, 7.95);
    printf("\n");

    printf("Testing lph2mpg...\n");
    testLph2mpg(0, -1);
    testLph2mpg(-5, -1);
    testLph2mpg(16, 14.70);
    testLph2mpg(31.2, 7.54);
    testLph2mpg(51.74, 4.55);
    printf("\n");
    return 0;
}
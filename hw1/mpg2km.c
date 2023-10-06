/* This funtion takes miles per gallon as an argument and returns the equivalent kilometers per liter */
float mpg2kml(float mpg) {
    if (mpg < 0) {
        return -1;
    } 
    return 0.425144 * mpg;
}

/* This function takes miles per gallon as an argument and returns the equivalent number of liters per 100 kilometers */
float mpg2lphm(float mpg) {
    if (mpg > -0.001 && mpg < 0.001 || mpg < 0) {
        return -1;
    } 
    return 235.214583 / mpg;
}

/* This function takes the liters per 100 kilometers and returns the equivalent miles per gallon*/
float lph2mpg(float lph) {
    if (lph > -0.001 && lph < 0.001 || lph < 0) {
        return -1;
    } 
    return 235.214583 / lph;
}

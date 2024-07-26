#include <iostream>
#include <string>
using namespace std;

#include "API.h"

void log(const string& text) {
    cerr << text << endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "START");
    API::clearAllText();

    int height = 16;
    int width = 16;
    int manhattan[height][width];
    
    int x = 0;
    int y = 0;

    int cy1 = height / 2;
    int cy2 = cy1 - 1;
    int cx1 = width / 2;
    int cx2 = cx1 - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            manhattan[i][j] = min(abs(i - cy1), abs(i - cy2)) + min(abs(j - cx1), abs(j - cx2));
        }
    } 

    while (true) {
        
    }
}

#include <iostream>
#include <fstream>
#include <math.h>
#include "fractals.h"

using namespace std;
using namespace fractals;

int main() {

    const int IMAGE_WIDTH = 1080;
    const int IMAGE_HEIGHT = 1080;

    const int MAX_ITERATIONS = 1000;

    const double SCALE = 3.6e-3;

    const Complex OFFSET = Complex(-0.77568377, 0.136467737);

    const Complex MAX = Complex(1.0, 1.0) * SCALE + OFFSET;
    const Complex MIN = Complex(-1.0, -1.0) * SCALE + OFFSET;

    Mandelbrot man = Mandelbrot(IMAGE_HEIGHT, IMAGE_WIDTH, MAX, MIN);

    man.loadIterationMap(MAX_ITERATIONS);

    ofstream fout("out.ppm");
    fout << "P3" << endl;
    fout << IMAGE_WIDTH << " " << IMAGE_HEIGHT << endl;
    fout << "256" << endl;

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < IMAGE_WIDTH; x++)
        {
            int maxIter = man.getIterationFromMap(x, y);
            
            int r = ((int)(maxIter * log2(maxIter)) % 256);
            int g = (maxIter % 256);
            int b = ((int)(maxIter * log10(maxIter))% 256);

            fout << r << " " << g << " " << b << " ";

        }

        fout << endl;
        
    }
    fout.close();
    cout << "Finished..." << endl;
    return EXIT_SUCCESS;
}
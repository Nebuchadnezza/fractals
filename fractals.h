#include <fstream>
#include <iostream>
#include <math.h>

#ifndef FRACTALS_H
#define FRACTALS_H

namespace fractals
{

    typedef unsigned short int uint16;
    typedef uint16** Array2D;

    /**
     * A class with simple implementations for complex number arithmetic
     * 
     * 
     * @param real real part of the complex number
     * @param imag imaginary part of the complex nuber
     */
    class Complex {

        public:
            Complex(double real = 0.0, double imag = 0.0)
                : _real(real), _imag(imag) {}

            Complex(const Complex&) = default;

            double real() const { return _real; }
            double imag() const { return _imag; }

            Complex operator+(const Complex &other) {
                return Complex(_real + other._real, _imag + other._imag);
            }

            Complex operator-(const Complex &other)
            {
                return Complex(_real - other._real, _imag - other._imag);
            }

            Complex operator*(const Complex &other) {
                return Complex(_real * other._real - _imag * other._imag, _real * other._imag + other._real * _imag);
            }

            double sqmagnitude() const {
                return _real * _real + _imag * _imag;
            }

        private:
            double _real, _imag;
    };

    // Function that returns the number of iterations in the mandelbrot set for a given complex
    //  number or 0 if it goes to infinity
    
    int getMandelbrotIterations(Complex c, int max_iterations) {

        int iterations = 0;
        Complex z;

        while (iterations < max_iterations && z.sqmagnitude() <= 4)
        {
            z = z * z + c;
            ++iterations;
        }
        
        return iterations;
    } 

    Complex mapPixelToComplex(int x, int y, int width, int height, Complex min, Complex max) {
        return Complex(
            static_cast<double>(x * ((max.real() - min.real()) / width) + min.real()),
            static_cast<double>(y * ((max.imag() - min.imag()) / height) + min.imag()));
    }

    void allocateBidimensionalDynamicArray(Array2D &array, int height, int width) {

        array = new uint16 *[height];
        for (int i = 0; i < height; i++)
        {
            array[i] = new uint16[width];
            for (int j = 0; j < width; j++)
            {
                array[i][j] = 0;
            }
        }        
    }

    void freeBidimensionalDynamicArray(Array2D &array, int height) {

        for (int i = 0; i < height; i++)
        {
            delete[] array[i];
            array[i] = nullptr;
        }
        delete array;
        array = nullptr;
    }

    /**
     * @brief A class with the basic functionality needed to generate the mandelbrot set
     * 
     */
    class Mandelbrot {

        public:
            Mandelbrot(int height, int width)
            {
                _height = height;
                _width = width;
                _lowerBounds = Complex(-2,-1);
                _upperBounds = Complex(1,1);
                allocateBidimensionalDynamicArray(_iterationMap, height, width);
            }

            Mandelbrot(int height, int width, Complex upperBounds, Complex lowerBounds) {
                _height = height;
                _width = width;
                _lowerBounds = upperBounds;
                _upperBounds = lowerBounds;
                allocateBidimensionalDynamicArray(_iterationMap, height, width);
            }

            ~Mandelbrot() {
                freeBidimensionalDynamicArray(_iterationMap, _height);
            }

            void loadIterationMap(int maxIterations) {

                for (int y = 0; y < _height; y++)
                {
                    for (int x = 0; x < _width; x++)
                    {
                        
                        _iterationMap[y][x] = getMandelbrotIterations( x, y, maxIterations);

                    }
                }
            }

            int getIterationFromMap(int x, int y) {
                return _iterationMap[y][x];
            }

            /**
             * @brief Will print a ppm image of the mandelbrot on a certain point
             * 
             * @param point point where the image will center
             * @param scale zoom into the point
             * @param upperBounds frame of mathematical calculations, must preserve a ratio with lowerBounds equal to image ratio
             * @param lowerBounds frame of mathematical calculations, must preserve a ratio with upperBounds equal to image ratio
             * @param image_width width of the ppm image
             * @param image_height height of the ppm image
             * @param max_iterations max number of iterations before breaking the calcualtions loop
             */
            void printImage(Complex point, double scale, Complex upperBounds, Complex lowerBounds, int image_width, int image_height, int max_iterations) {

                const Complex OFFSET = point * scale;

                const Complex MAX = upperBounds * scale + OFFSET;

                const Complex MIN = lowerBounds * scale + OFFSET;

                Mandelbrot mandelbrot = Mandelbrot(image_height, image_width, MAX, MIN);

                mandelbrot.loadIterationMap(max_iterations);

                ofstream fout("out.ppm");
                fout << "P3" << endl;
                fout << image_width << " " << image_height << endl;
                fout << "256" << endl;

                for (int y = 0; y < image_height; y++)
                {
                    for (int x = 0; x < image_width; x++)
                    {
                        int maxIter = mandelbrot.getIterationFromMap(x, y);

                        int r = ((int)(maxIter * log2(maxIter)) % 256);
                        int g = (maxIter % 256);
                        int b = ((int)(maxIter * log10(maxIter)) % 256);

                        fout << r << " " << g << " " << b << " ";
                    }

                    fout << endl;
                }
                fout.close();
                cout << "Finished..." << endl;
            }

        private:

            uint16 getMandelbrotIterations(Complex c, int max_iterations) {

                int iterations = 0;
                Complex z;

                while (iterations < max_iterations && z.sqmagnitude() <= 4)
                {
                    z = z * z + c;
                    ++iterations;
                }

                return iterations;
            }

            uint16 getMandelbrotIterations(int x, int y, int max_iterations) {

                return getMandelbrotIterations(
                    mapPixelToComplex(x, y, _width, _height, _lowerBounds, _upperBounds),
                    max_iterations
                );

            }

            Complex _lowerBounds, _upperBounds;
            int _width, _height; 
            Array2D _iterationMap;
    };

    
}

#endif
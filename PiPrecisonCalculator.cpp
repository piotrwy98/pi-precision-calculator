#include <iostream>
#include <omp.h>
#include <iomanip>
#include <fstream>

using namespace std;

void calcuclate(long long precision, int threads, int iterations, ofstream& file)
{
	omp_set_num_threads(threads);

	double totalTime = 0;
	double pi;

	for (int x = 0; x < iterations; x++)
	{
		pi = 0;
		double start = omp_get_wtime();

		#pragma omp parallel for reduction(+:pi)
		for (long long i = 0; i < precision; i++)
		{
			pi += 1.0 / (1.0 + (i * 2.0)) * (i % 2 == 0 ? 1.0 : -1.0);
		}

		double end = omp_get_wtime();
		totalTime += end - start;
	}

	double averageTime = totalTime / iterations;

	cout << "Threads: " << setfill(' ') << setw(2) << threads << ",  ";
	cout << "t = " << fixed << setprecision(5) << averageTime << " s,  ";
	cout << "PI = " << setprecision(8) << pi * 4.0 << endl;
	file << threads << ";" << setprecision(5) << averageTime << endl;
}

int main(int argc, char* argv[])
{
	long long precision = atoll(argv[1]);
	int threads = atoi(argv[2]);
	int interations = atoi(argv[3]);
	ofstream file;

	file.open("results.csv");
	file << "Threads; Time [s]" << endl;
	cout << "Precision: " << precision << endl;

	for (int i = 1; i <= threads; i++)
	{
		calcuclate(precision, i, interations, file);
	}

	file.close();

	return 0;
}
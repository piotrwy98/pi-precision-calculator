#include <iostream>
#include <omp.h>
#include <iomanip>
#include <fstream>

using namespace std;

void calcuclate(long long precision, int threads, int iterations, ofstream& file)
{
	if (threads > 0)
	{
		omp_set_num_threads(threads);
	}

	double totalTime = 0;
	double pi;

	for (int x = 0; x < iterations; x++)
	{
		pi = 0;
		double start = omp_get_wtime();

		if (threads > 0)
		{
			#pragma omp parallel for reduction(+:pi)
			for (long long i = 0; i < precision; i++)
			{
				pi += 1.0 / (1.0 + (i * 2.0)) * (i % 2 == 0 ? 1.0 : -1.0);
			}
		}
		else
		{
			for (long long i = 0; i < precision; i++)
			{
				pi += 1.0 / (1.0 + (i * 2.0)) * (i % 2 == 0 ? 1.0 : -1.0);
			}
		}

		double end = omp_get_wtime();
		totalTime += end - start;
	}

	double averageTime = totalTime / iterations;

	if (threads > 0)
	{
		cout << "Parallel threads: " << setfill(' ') << setw(2) << threads << ",\t";
	}
	else
	{
		cout << "Sequential,\t\t";
	}

	cout << "t = " << fixed << setprecision(5) << averageTime << " s,\t  ";
	cout << "PI = " << setprecision(15) << pi * 4.0 << endl;
	file << threads << ";" << setprecision(5) << averageTime << endl;
}

int main(int argc, char* argv[])
{
	long long precision = atoll(argv[1]);
	int threads = atoi(argv[2]);
	int interations = atoi(argv[3]);
	ofstream file;

	file.open("results.csv");
	file << "Threads; Time [s];" << endl;

	calcuclate(precision, 0, interations, file);

	for (int i = 1; i <= threads; i++)
	{
		calcuclate(precision, i, interations, file);
	}

	file.close();

	return 0;
}

// Tworzenie puli wątków też kosztuje
// Procesor jest 4 rdzeniowy i tam powinno być wyhamowanie, ale jest multithreading (wielowątkowość) i to daje 1 - 2 dodatkowe wątki na wyhamowanie
// Można wyłączyć w BIOSIE multithreading i zobaczyć wtedy wyniki
// Czym się różni blokujące od nieblokującego
// Co to sekcja krytyczna, bariery
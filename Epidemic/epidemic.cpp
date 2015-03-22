﻿//---В связи с эпидемией гриппа в больницу направляется А больных гриппом "А" и В больных гриппом "В".Больных гриппом "А" нельзя помещать в одну палату с больными гриппом "В".Имеется информация об 
//общем количестве палат P в больнице, пронумерованных от 1 до P, и о распределении уже имеющихся там больных.
//Необходимо определить максимальное количество больных M, которое больница в состоянии принять.При размещении новых больных не разрешается переселять уже имеющихся больных из палаты в палату.
//---Входные данные находятся в текстовом файле с именем input.txt  и имеют следующую структуру :
//•	в первой строке находится целое число A(0≤A≤100);
//•	во второй строке – целое число B(0≤B≤100);
//•	в третьей строке – натуральное число P(P≤20);
//•	в каждой из последующих P строк находятся 3 числа n, a, b, разделенных пробелом, где n - вместимость палаты, a - количество уже имеющихся в палате больных гриппом "А", b - количество уже 
//имеющихся в палате больных гриппом "В".Информация о вместимости палат вводится последовательно для палат с номерами 1, 2, ..., P.Числа n, a, b - целые неотрицательные, меньшие 100.
//---Выходные данные должны быть записаны в текстовый файл с именем output.txt и иметь следующий формат :
//•	в первой строке должно находиться число M;
//•	если все поступившие больные размещены, то во второй строке должны находиться номера палат, разделенные пробелом, куда помещаются больные гриппом "А" (в порядке возрастания).

#include <fstream>

using namespace std;

struct HospitalWard{

	int beds;
	int patientsA;
	int patientsB;

	HospitalWard() {

		this->beds = 0;
		this->patientsA = 0;
		this->patientsB = 0;
	}

	HospitalWard(int beds, int patientsA, int patientsB) {

		this->beds = beds;
		this->patientsA = patientsA;
		this->patientsB = patientsB;
	}
};

class Epidemic {

public:

	int A; //number of patients A
	int B; //number of patients B
	int P; //number of hospital wards
	HospitalWard *wards;

	int bedsForA; //number of free beds for patientsA
	int bedsForB; //number of free beds for patientsB

	Epidemic(int A, int B, int P) {

		this->A = A;
		this->B = B;
		this->P = P;
		this->wards = new HospitalWard[this->P];
	}

	void busyWards() {

		for (int i = 0; i < P; i++) {

			if (wards[i].patientsA != 0) {

				bedsForA += wards[i].beds - wards[i].patientsA;
			}
			else if (wards[i].patientsB != 0) {

				bedsForB += wards[i].beds - wards[i].patientsB;
			}
		}
	}

	bool isSatisfiedA() {

		if (bedsForA >= A) {

			return true;
		}
		else {

			return false;
		}
	}

	bool isSatisfiedB() {

		if (bedsForB >= B) {

			return true;
		}
		else {

			return false;
		}
	}

	int videWards() {

		int bedsInFreeWards = 0;

		for (int i = 0; i < P; i++) {

			if (wards[i].patientsA == 0 && wards[i].patientsB == 0) {

				bedsInFreeWards += wards[i].beds;
			}
		}

		return bedsInFreeWards;
	}
};


int main() {

	ifstream fin("input.txt");
	ofstream fout("output.txt");

	int A = 0; //number of patients A
	int B = 0; //number of patients B
	int P = 0; //number of hospital wards
	int n = 0; //number of beds
	int a = 0; //number of patients A in a hospital ward
	int b = 0; //number of patients B in a hospital ward
	int bedsInFreeWards = 0;

	fin >> A;
	fin >> B;
	fin >> P;

	Epidemic epidemic(A, B, P);

	for (int i = 0; i < P; i++) {

		fin >> n;
		fin >> a;
		fin >> b;

		epidemic.wards[i] = HospitalWard(n, a, b);
	}

	epidemic.busyWards();

	if (epidemic.isSatisfiedA() && epidemic.isSatisfiedB()) {

		return 0;
	}

	bedsInFreeWards = epidemic.videWards();





	return 0;
}
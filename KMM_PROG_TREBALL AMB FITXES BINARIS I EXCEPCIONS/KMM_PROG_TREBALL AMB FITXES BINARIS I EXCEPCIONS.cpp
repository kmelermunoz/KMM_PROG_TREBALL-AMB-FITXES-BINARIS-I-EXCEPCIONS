#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

using namespace std;

// Definicion de la estructura Pelicula
struct Pelicula {
    string titol;
    int any_estrena;
    int num_escenes;
    int personatges_per_escena;
};

// Funcion para desear las datos al fitxer
void desar_dades(const vector<Pelicula>& pelicules) {
    ofstream fitxer("pelicules_dades.bin", ios::binary);
    if (!fitxer.is_open()) {
        throw runtime_error("No se ha podido abrir el fitxer para la escritura.");
    }

    for (const Pelicula& pelicula : pelicules) {
        fitxer.write(reinterpret_cast<const char*>(&pelicula), sizeof(Pelicula));
    }
}

// Funcion para cargar las datos del fitxer
vector<Pelicula> carregar_dades() {
    vector<Pelicula> pelicules;
    ifstream fitxer("pelicules_dades.bin", ios::binary);
    if (!fitxer.is_open()) {
        return pelicules;
    }

    Pelicula pelicula;
    while (fitxer.read(reinterpret_cast<char*>(&pelicula), sizeof(Pelicula))) {
        pelicules.push_back(pelicula);
    }

    return pelicules;
}

// Funcion para añadir una nueva pelicula
void afegir_pelicula(vector<Pelicula>& pelicules, const string& titol, int any_estrena, int num_escenes, int personatges_per_escena) {
    pelicules.push_back({ titol, any_estrena, num_escenes, personatges_per_escena });
    desar_dades(pelicules);
}

// Funcion para eliminar una pelicula
void eliminar_pelicula(vector<Pelicula>& pelicules, const string& titol) {
    pelicules.erase(remove_if(pelicules.begin(), pelicules.end(),
        [&titol](const Pelicula& p) { return p.titol == titol; }),
        pelicules.end());
    desar_dades(pelicules);
}

// Funcion para calcular las estadisticas de las peliculas
void calcular_estadisticas(const vector<Pelicula>& pelicules) {
    if (pelicules.empty()) {
        cout << "No hay datos disponibles." << endl;
        return;
    }

    vector<int> counts_personatges;
    for (const Pelicula& pelicula : pelicules) {
        counts_personatges.push_back(pelicula.personatges_per_escena);
    }

    double suma = 0;
    for (int count : counts_personatges) {
        suma += count;
    }
    double mitjana_personatges = suma / counts_personatges.size();

    auto moda = [&counts_personatges]() {
        int valor_moda = 0;
        int max_count = 0;
        for (int valor : counts_personatges) {
            int count = count_if(counts_personatges.begin(), counts_personatges.end(), [&valor](int x) { return x == valor; });
            if (count > max_count) {
                max_count = count;
                valor_moda = valor;
            }
        }
        return valor_moda;
        };

    sort(counts_personatges.begin(), counts_personatges.end());
    int mig = counts_personatges.size() / 2;
    double mediana_personatges = (counts_personatges.size() % 2 != 0) ? counts_personatges[mig] : (counts_personatges[mig - 1] + counts_personatges[mig]) / 2.0;

    cout << "Mitjana de personatges per escena: " << mitjana_personatges << endl;
    cout << "Moda de personatges per escena: " << moda() << endl;
    cout << "Mediana de personatges per escena: " << mediana_personatges << endl;
}

int main() {
    vector<Pelicula> pelicules = carregar_dades();

    while (true) {
        cout << "\n1. Afegir pelicula" << endl;
        cout << "2. Esborrar pelicula" << endl;
        cout << "3. Calcular estadisticas" << endl;
        cout << "4. Sortir" << endl;

        int opcio;
        cout << "Selecciona una opcion: ";
        cin >> opcio;

        if (opcio == 1) {
            string titol;
            int any_estrena, num_escenes, personatges_per_escena;
            cout << "Introdueix el titol de la pelicula: ";
            cin.ignore();
            getline(cin, titol);
            cout << "Introdueix l'any d'estrena: ";
            cin >> any_estrena;
            cout << "Introdueix el nombre d'escenes: ";
            cin >> num_escenes;
            cout << "Introdueix el nombre de personatges per escena: ";
            cin >> personatges_per_escena;
            afegir_pelicula(pelicules, titol, any_estrena, num_escenes, personatges_per_escena);
        }
        else if (opcio == 2) {
            string titol;
            cout << "Introdueix el titol de la pelicula a esborrar: ";
            cin.ignore();
            getline(cin, titol);
            eliminar_pelicula(pelicules, titol);
        }
        else if (opcio == 3) {
            calcular_estadisticas(pelicules);
        }
        else if (opcio == 4) {
            break;
        }
        else {
            cout << "Opcion no valida." << endl;
        }
    }

    return 0;
}

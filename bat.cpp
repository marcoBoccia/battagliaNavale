#include <iostream>
#include <random>
#include <time.h>
#include <Windows.h>
#include <process.h>

using namespace std;
const int DIMENSIONE = 10;
const char VUOTO = ' ';
const int numeroNavi = 7;

struct nave
{
    string tipo;
    int lunghezza;
};

nave *creaFlotta()
{
    nave *navi = new nave[numeroNavi];
    int n = 0;
    while (n < 2)
    {
        navi[n].lunghezza = 5;
        navi[n].tipo = "corazzata" + to_string(n);
        n++;
    }
    while (n < 4)
    {
        navi[n].lunghezza = 3;
        navi[n].tipo = "incrociatore" + to_string(n);
        n++;
    }
    while (n < numeroNavi)
    {
        navi[n].lunghezza = 2;
        navi[n].tipo = "sottomarino" + to_string(n);
        n++;
    }
    return navi;
}

void inizializza(char matrice[][DIMENSIONE])
{
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            matrice[i][j] = VUOTO;
        }
    }
}

void stampaConsole(char matriceGiocatore[][DIMENSIONE], char matricePC[][DIMENSIONE])
{
    cout << "   |";
    for (int i = 'A'; i < 'A' + DIMENSIONE; i++)
    {
        cout << " " << (char)i << " ";
    }
    cout << "        |";
    for (int i = 'A'; i < 'A' + DIMENSIONE; i++)
    {
        cout << " " << (char)i << " ";
    }
    cout << endl;
    int trattini = DIMENSIONE * 3 + 3;
    for (int i = 0; i < trattini; i++)
    {
        cout << '-';
    }
    cout << "      ";
    for (int i = 0; i < trattini; i++)
    {
        cout << '-';
    }
    cout << endl;
    for (int i = 0; i < DIMENSIONE; i++)
    {
        cout << i + 1 << ((i < 9) ? "  |" : " |");
        for (int j = 0; j < DIMENSIONE; j++)
        {
            cout << " " << matriceGiocatore[i][j] << " ";
        }
        cout << "     " << i + 1 << ((i < 9) ? "  |" : " |");
        for (int j = 0; j < DIMENSIONE; j++)
        {
            cout << " " << matricePC[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool posiziona(char matrice[][DIMENSIONE], nave ship, int x, int y, int v)
{
    srand(time(NULL));
    bool check = true;
    switch (v)
    {
    case 0: // ORIZZONTALE
    {
        if (x >= DIMENSIONE - ship.lunghezza)
        {
            return false;
        }
        else
        {
            int index = 0;
            int i = x;
            while (index < ship.lunghezza)
            {
                if (matrice[y][i] != VUOTO)
                {
                    return false;
                }
                index++;
                i++;
            }
            index = 0;
            i = x;
            while (index < ship.lunghezza)
            {
                
                matrice[y][i] = ship.tipo[ship.tipo.length()-1];
                i++;
                index++;
            }
        }
    }
    break;
    case 1: // VERTICALE
    {
        if (y >= DIMENSIONE - ship.lunghezza)
        {
            return false;
        }
        else
        {
            int index = 0;
            int i = y;
            while (index < ship.lunghezza)
            {
                if (matrice[i][x] != VUOTO)
                {
                    return false;
                }
                i++;
                index++;
            }
            index = 0;
            i = y;
            while (index < ship.lunghezza)
            {
                matrice[i][x] = ship.tipo[ship.tipo.length()-1];
                index++;
                i++;
            }
        }
    }
    break;
    }
    return check;
}

void impostaNavi(char matrice[][DIMENSIONE], nave navi[])
{
    int coordX = rand() % DIMENSIONE;
    int coordY = rand() % DIMENSIONE;
    int verso = rand() % 2;

    for (int i = 0; i < 7; i++)
    {
        while (!posiziona(matrice, navi[i], coordX, coordY, verso))
        {
            coordX = rand() % DIMENSIONE;
            coordY = rand() % DIMENSIONE;
            verso = rand() % 2;
        }
    }
}

void stampaConDelay(string frase)
{
    for (int i = 0; i < frase.length(); i++)
    {
        cout << frase[i];
        Sleep(15);
    }
}

bool checkCoordinate(char let, int num)
{
    if (((int)let < 'A') or ((int)let > 'A' + DIMENSIONE) or (num < 1) or (num > 10))
    {
        return false;
    }
    else
    {
        return true;
    }
}

char rendiMaiuscola(char c)
{
    if (((int)c >= 'a') and ((int)c <= 'z'))
    {
        return (int)c - 32;
    }
    else
    {
        return c;
    }
}

bool turnoGiocatore(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE])
{
    char lettera;
    int numero;
    stampaConDelay("Dammi le coordinate dell'obiettivo da colpire, prima la lettera e poi il numero...\n");
    cin >> lettera;
    cin >> numero;
    lettera = rendiMaiuscola(lettera);
    while (!checkCoordinate(lettera, numero))
    {
        stampaConDelay("Coordinate non valide, riprova. Prima la lettera, poi il numero...\n");
        cin >> lettera;
        lettera = rendiMaiuscola(lettera);
        cin >> numero;
    }
    int coordinata = (int)lettera - 65;
    if (matriceNascosta[numero - 1][coordinata] != VUOTO)
    {
        matriceVisibile[numero - 1][coordinata] = 'O';
        return true;
    }
    else
    {
        matriceVisibile[numero - 1][coordinata] = 'X';
        return false;
    }
}

bool finePartita(char matrice[][DIMENSIONE])
{
    int count = 0;
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matrice[i][j] == 'O')
            {
                count++;
            }
        }
    }
    return (count == 22);
}

bool isColpitoEAffondato(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE])
{
    int numC = 0;
    int numI = 0;
    int numS = 0;
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matriceVisibile[i][j] == 'O')
            {
                if (matriceNascosta[i][j] == 'c')
                {
                    numC++;
                }
                else if (matriceNascosta[i][j] == 'i')
                {
                    numI++;
                }
                else if (matriceNascosta[i][j] == 's')
                {
                    numS++;
                }
            }
        }
    }
    if ((numC % 5 == 0) and (numC != 0))
    {
        return true;
    }
    else if ((numI % 3 == 0) and (numI != 0))
    {
        return true;
    }
    else if ((numS % 2 == 0) and (numS != 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    bool turno = true;
    string giocatore = "Riccardo";
    nave *flotta = creaFlotta();
    char grigliaGiocatore[DIMENSIONE][DIMENSIONE];
    char grigliaPC[DIMENSIONE][DIMENSIONE];
    char grigliaAvversario[DIMENSIONE][DIMENSIONE];
    inizializza(grigliaGiocatore);
    inizializza(grigliaPC);
    inizializza(grigliaAvversario);
    stampaConDelay("STO POSIZIONANDO LE NAVI..........");
    impostaNavi(grigliaGiocatore, flotta);
    impostaNavi(grigliaPC, flotta);
    stampaConDelay("FLOTTE PRONTE ALLA BATTAGLIA!");
    cout << endl
         << endl;
    Sleep(1500);
    stampaConsole(grigliaGiocatore, grigliaAvversario);
    // stampaConsole(grigliaGiocatore, grigliaPC);
    bool diNuovo = false;
    do
    {
        if (turno)
        {
            Sleep(1500);
            while (turnoGiocatore(grigliaPC, grigliaAvversario))
            {
                Sleep(1500);
                if (isColpitoEAffondato(grigliaPC, grigliaAvversario))
                {
                    stampaConDelay("Colpito e affondato!\n");
                }
                stampaConsole(grigliaGiocatore, grigliaAvversario);
            }
            stampaConsole(grigliaGiocatore, grigliaAvversario);
            turno = !turno;
        }
        else
        {
            stampaConDelay("Tocca di nuovo a te...");
            turno = !turno;
        }
    } while (!finePartita(grigliaGiocatore) and !finePartita(grigliaAvversario));
    system("pause");
}
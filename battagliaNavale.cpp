#include <iostream>
#include <random>
#include <time.h>
#include <Windows.h>
#include <process.h>
#include <string>

using namespace std;

const int DIMENSIONE = 10;
const char VUOTO = ' ';
const int NUMERONAVI = 7;

/*
LEGENDA:

LETTERA 'X': NAVE COLPITA
LETTERA 'O': BERSAGLIO MANCATO
LETTERA 'c': SIMBOLO DELLA NAVE CORAZZATA
LETTERA 'i': SIMBOLO DELLA NAVE INCROCIATORE
LETTERA 's': SIMBOLO DELLA NAVE SOTTOMARINO
NUMERI DA 0 A 6: IDENTIFICANO LE NAVI SULLA GRIGLIA DI GIOCO (0 E 1 PER LE CORAZZATE, 2 E 3 PER GLI INCROCIATORI, DA 4 A 6 PER I SOTTOMARINI)

OGNI PUNTO DELLA NAVE COLPITA SOSTITUISCE IL SUO NUMERO CORRISPONDENTE CON IL CARATTERE 'X' (ES. '0''0''X''0''0' PER UNA CORAZZATA)

PER OGNI NAVE AFFONDATA VENGONO SOSTITUITI I CARATTERI 'X' CON LE LETTERE IDENTIFICATIVE (ES. DA 'X''X''X' A 'i''i''i' PER UNA CORAZZATA) 
*/

struct nave
{
    string tipo;
    int lunghezza;
};

void stampaConDelay(string frase)
{
    for (int i = 0; i < frase.length(); i++)
    {
        cout << frase[i];
        Sleep(15);
    }
}

nave *creaFlotta() // RESTITUISCE UN ARRAY CON LE NAVI CHE COMPONGONO LE DUE FLOTTE (2 DA CINQUE, 2 DA TRE E 3 DA DUE)
{
    nave *navi = new nave[NUMERONAVI];
    int n = 0;
    while (n < 2)
    {
        navi[n].lunghezza = 5;
        navi[n].tipo = "corazzata" + to_string(n); // CONCATENO AL TIPO DI NAVE ANCHE L'INDICE CHE SCORRE L'ARRAY, PER DIVERSIFICARE TUTTE LE NAVI
        n++;
    }
    while (n < 4)
    {
        navi[n].lunghezza = 3;
        navi[n].tipo = "incrociatore" + to_string(n);
        n++;
    }
    while (n < NUMERONAVI)
    {
        navi[n].lunghezza = 2;
        navi[n].tipo = "sottomarino" + to_string(n);
        n++;
    }
    return navi;
}

void inizializza(char matrice[][DIMENSIONE]) // IMPOSTA LA MATRICE DI GIOCO CON CARATTERE COSTANTE VUOTO
{
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            matrice[i][j] = VUOTO;
        }
    }
}

char rendiMaiuscola(char c) // SE L'UTENTE INSERISCE COME COORDINATA UNA LETTERA MINUSCOLA ALLORA VIENE CAMBIATA IN MAIUSCOLA
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

bool checkCoordinate(char let, int num) // CONTROLLA SE L'UTENTE HA INSERITO COORDINATE INTERNE ALLA GRIGLIA DI GIOCO
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

void impostaGiocatoreVisibile(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE]) // COPIA NELLA MATRICE VISIBILE LE NAVI DELLA GRIGLIA NASCOSTA
{
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matriceNascosta[i][j] != VUOTO)
            {
                matriceVisibile[i][j] = matriceNascosta[i][j];
            }
        }
    }
}

void stampaConsole(char matriceGiocatore[][DIMENSIONE], char matricePC[][DIMENSIONE]) // STAMPA LE DUE MATRICI DI GIOCO SULLO SCHERMO
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

bool posiziona(char matrice[][DIMENSIONE], nave ship, int x, int y, int v) // DEFINISCE LA POSIZIONE DI UNA NAVE
{
    srand(time(NULL)); // GENERA UN NUOVO SEME PER L'ESTRAZIONE DI UN NUMERO CASUALE
    bool check = true;
    switch (v) // INDICA IL VERSO: ORIZZONTALE O VERTICALE
    {
    case 0: // ORIZZONTALE
    {
        if (x >= DIMENSIONE - ship.lunghezza) // CONTROLLA SE LA POSIZIONE DELLA NAVE VA OLTRE I LIMITI DELLA GRIGLIA
        {
            return false;
        }
        else
        {
            int index = 0;
            int i = x;
            while (index < ship.lunghezza) // CONTROLLA CHE LA NAVE NON NE INCROCI UN'ALTRA
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
            while (index < ship.lunghezza) // INSERISCE LA NAVE SCRIVENDO COME CARATTERE IL NUMERO PRESENTE ALLA FINE DELL'ATTRIBUTO "TIPO" DI SHIP
            {

                matrice[y][i] = ship.tipo[ship.tipo.length() - 1];
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
                matrice[i][x] = ship.tipo[ship.tipo.length() - 1];
                index++;
                i++;
            }
        }
    }
    break;
    }
    return check; // RITORNA TRUE SE LA NAVE E' STATA POSIZIONATA SECONDO LE COORDINATE E IL VERSO IN INPUT, FALSE ALTRIMENTI
}

void impostaNavi(char matrice[][DIMENSIONE], nave navi[]) // IMPOSTA LE NAVI SULLA GRIGLIA DI GIOCO
{
    int coordX = rand() % DIMENSIONE; // STABILISCE LE COORDINATE E IL VERSO IN MANIERA CASUALE
    int coordY = rand() % DIMENSIONE;
    int verso = rand() % 2;

    for (int i = 0; i < 7; i++) // PER OGNI NAVE
    {
        while (!posiziona(matrice, navi[i], coordX, coordY, verso)) // CICLO CHE IMPOSTA LE NAVI SULLA GRIGLIA (ENTRA E RICHIEDE NUOVE COORDINATE SE NON E' RIUSCITO A POSIZIONARE LA NAVE)
        {
            coordX = rand() % DIMENSIONE;
            coordY = rand() % DIMENSIONE;
            verso = rand() % 2;
        }
    }
}

bool turnoGiocatore(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE]) // MOSSA DEL GIOCATORE (RESTITUISCE TRUE SE HA COLPITO UNA NAVE, FALSE ALTRIMENTI)
{
    char lettera; // COORDINATE
    int numero;
    stampaConDelay("Dammi le coordinate dell'obiettivo da colpire, prima la lettera e poi il numero...\n");
    cin >> lettera;
    cin >> numero;
    lettera = rendiMaiuscola(lettera);        // SE LA COORDINATA INSERITA E' MINUSCOLA LA RENDE MAIUSCOLA
    while (!checkCoordinate(lettera, numero)) // CONTROLLA ED EVENTUALMENTE RICHIEDE NUOVE COORDINATE SE FUORI DAL RANGE DELLA GRIGLIA
    {
        stampaConDelay("Coordinate non valide, riprova. Prima la lettera, poi il numero...\n");
        cin >> lettera;
        lettera = rendiMaiuscola(lettera);
        cin >> numero;
    }
    int coordinata = (int)lettera - 65;
    Sleep(1000);
    if (matriceVisibile[numero - 1][coordinata] == VUOTO) // CONTROLLA NELLA MATRICE VISIBILE SU SCHERMO SE QUESTO PUNTO NON E' MAI STATO BOMBARDATO
    {
        if (matriceNascosta[numero - 1][coordinata] != VUOTO) // CONTROLLA NELLA MATRICE NASCOSTA SE IN QUESTO PUNTO E' PRESENTE UNA NAVE
        {
            matriceVisibile[numero - 1][coordinata] = 'X'; // SE COLPITO SOSTITUISCE CON CARATTERE 'X'
            return true;
        }
        else
        {
            matriceVisibile[numero - 1][coordinata] = 'O'; // SE NON COLPITO SOSTITUISCE CON CARATTERE 'O'
            return false;
        }
    }
    else
    {
        stampaConDelay("Hai gia' bombardato in quel punto, fai piu' attenzione al prossimo turno...\n");
        return false;
    }
}

bool turnoPC(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE]) // MOSSA CASUALE DEL PC (RESTITUISCE TRUE SE HA COLPITO UNA NAVE, FALSE ALTRIMENTI)
{
    srand(time(NULL));
    int coordX = rand() % 10; // STABILISCE LE COORDINATE IN MANIERA CASUALE
    int coordY = rand() % 10;
    /*
    int coordX;
    int coordY;
    cin >> coordX;
    cin >> coordY;
    */
    Sleep(1000);
    int tentativi = 1;
    char indiceNumero;
    // cout << "Tentativo numero: " << tentativi << "     Coordinate:  " << indiceNumero << " " << coordY+1 << endl;
    while (((matriceVisibile[coordY][coordX] == 'X') or (matriceVisibile[coordY][coordX] == 'O') or (matriceVisibile[coordY][coordX] == 'c') or (matriceVisibile[coordY][coordX] == 'i') or (matriceVisibile[coordY][coordX] == 's')) and (tentativi < 1000)) // SE IL PUNTO E' GIA' STATO BOMBARDATO
    {
        coordX = rand() % 10;
        coordY = rand() % 10;
        tentativi++;
        // cout << "Tentativo numero: " << tentativi << "     Coordinate:  " << indiceNumero << " " << coordY + 1 << endl;
    }
    indiceNumero = ((char)coordX + 65);
    stampaConDelay("Il computer si prepara a colpire in ");
    cout << indiceNumero << " " << coordY + 1;
    stampaConDelay("...\n");
    if (matriceNascosta[coordY][coordX] != VUOTO) // CONTROLLA NELLA MATRICE NASCOSTA SE IN QUESTO PUNTO E' PRESENTE UNA NAVE
    {
        matriceVisibile[coordY][coordX] = 'X'; // SE COLPITO SOSTITUISCE CON CARATTERE 'X'
        return true;
    }
    else
    {
        matriceVisibile[coordY][coordX] = 'O'; // SE NON COLPITO SOSTITUISCE CON CARATTERE 'O'
        return false;
    }
}

int *checkBersagli(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE]) // RESTITUISCE UN ARRAY LUNGO QUANTO IL NUMERO DI NAVI, CHE PER OGNI NAVE INDICA QUANTE VOLTE E' STATA COLPITA
{
    int *numeri = new int[NUMERONAVI];
    int indiceNavi = 0;
    for (int i = 0; i < NUMERONAVI; i++) // INIZIALIZZA L'ARRAY CON ZERO
    {
        numeri[i] = 0;
    }
    for (int i = 0; i < DIMENSIONE; i++) // PER OGNI RIGA DELLA MATRICE
    {
        for (int j = 0; j < DIMENSIONE; j++) // PER OGNI COLONNA DELLA MATRICE
        {
            if (matriceVisibile[i][j] == 'X') // SE IL PUNTO CORRISPONDE AD UNA NAVE COLPITA
            {
                indiceNavi = (int)matriceNascosta[i][j] - 48;
                numeri[indiceNavi]++; // INCREMENTA DI UNO LA POSIZIONE DELL'ARRAY CORRISPONDENTE AL NUMERO DELLA NAVE
            }
        }
    }
    return numeri;
}

int isColpitoEAffondato(int bersagli[]) // RESTITUISCE IL NUMERO INTERO POSITIVO CORRISPONDENTE ALLA NAVE CHE E' STATA COLPITA E AFFONDATA, -1 ALTRIMENTI
{
    int n = 0;
    int indice = -1;
    while (n < 2)
    {
        if (bersagli[n] == 5) // CONTROLLA CHE IL NUMERO DI COLPI SUBITI SIA UGUALE AL NUMERO DI COLPI SUFFICIENTI AD ABBATTERLA
        {
            indice = n;
        }
        n++;
    }
    while (n < 4)
    {
        if (bersagli[n] == 3)
        {
            indice = n;
        }
        n++;
    }
    while (n < NUMERONAVI)
    {
        if (bersagli[n] == 2)
        {
            indice = n;
        }
        n++;
    }
    return indice;
}

void mostraNaveAbbattuta(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE], int numeroNave) // SOSTITUISCE IL CARATTERE 'X' CON UN CARATTERE CHE INDICA IL TIPO DELLA NAVE
{
    char car;
    if ((numeroNave == 0) or (numeroNave == 1)) // CONTROLLA QUAL E' IL NUMERO DELLA NAVE ABBATTUTA, E ASSEGNA UN CARATTERE DIVERSO A SECONDA DEL TIPO DI NAVE
    {
        car = 'c';
    }
    else if ((numeroNave == 2) or (numeroNave == 3))
    {
        car = 'i';
    }
    else
    {
        car = 's';
    }

    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matriceNascosta[i][j] == (char)(numeroNave + 48)) // CONTROLLA SE NELLA MATRICE NASCOSTA CI SIA IL NUMERO DELLA NAVE DA MOSTRARE
            {
                matriceVisibile[i][j] = car; // SOSTITUISCE NELLA GRIGLIA VISIBILE IL CARATTERE 'X' CON IL CARATTERE TIPO DI NAVE
            }
        }
    }
}

bool finePartita(char matrice[][DIMENSIONE]) // RESTITUISCE TRUE SE LA PARTITA E' FINITA, FALSE ALTRIMENTI
{
    // SCORRE TUTTA LA GRIGLIA E CONTA QUANTI COLPI ANDATI A BUON FINE CI SONO
    int count = 0;
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if ((matrice[i][j] == 'c') or (matrice[i][j] == 's') or (matrice[i][j] == 'i') or (matrice[i][j] == 'X'))
            {
                count++;
            }
        }
    }
    return (count == 22); // TRUE SE I COLPI ANDATI A BUON FINE SONO 22, FALSE ALTRIMENTI
}

int main()
{
    bool turno = true;                                     // GESTISCE IL TURNO
    string giocatore = "Riccardo";                         // IMPOSTA IL NOME DEL GIOCATORE
    nave *flotta = creaFlotta();                           // INIZIALIZZA UN ARRAY DI TIPO NAVE
    char grigliaGiocatoreNascosta[DIMENSIONE][DIMENSIONE]; // DEFINISCO LE GRIGLIE DI GIOCO: DUE NASCOSTE CON LE NAVI IN POSIZIONE, DUE MOSTRATE A SCHERMO
    char grigliaPCNascosta[DIMENSIONE][DIMENSIONE];
    char grigliaPCVisibile[DIMENSIONE][DIMENSIONE];
    char grigliaGiocatoreVisibile[DIMENSIONE][DIMENSIONE];
    inizializza(grigliaGiocatoreNascosta); // INIZIALIZZA TUTTE E 4 LE MATRICI CON CARATTERE VUOTO
    inizializza(grigliaPCNascosta);
    inizializza(grigliaPCVisibile);
    inizializza(grigliaGiocatoreVisibile);
    stampaConDelay("STO POSIZIONANDO LE NAVI..........");
    impostaNavi(grigliaGiocatoreNascosta, flotta); // IMPOSTA LE NAVI IN ORDINE RANDOM
    impostaNavi(grigliaPCNascosta, flotta);
    /*
    grigliaGiocatoreNascosta[2][2] = '6';
    grigliaGiocatoreNascosta[2][3] = '6';
    grigliaGiocatoreNascosta[4][8] = '0';
    grigliaGiocatoreNascosta[5][8] = '0';
    grigliaGiocatoreNascosta[6][8] = '0';
    grigliaGiocatoreNascosta[7][8] = '0';
    grigliaGiocatoreNascosta[8][8] = '0';
    grigliaGiocatoreNascosta[2][7] = '4';
    grigliaGiocatoreNascosta[3][7] = '4';
    grigliaGiocatoreNascosta[4][7] = '4';
    grigliaPCNascosta[2][2] = '6';
    grigliaPCNascosta[2][3] = '6';
    grigliaPCNascosta[4][8] = '0';
    grigliaPCNascosta[5][8] = '0';
    grigliaPCNascosta[6][8] = '0';
    grigliaPCNascosta[7][8] = '0';
    grigliaPCNascosta[8][8] = '0';
    grigliaPCNascosta[2][7] = '2';
    grigliaPCNascosta[3][7] = '2';
    grigliaPCNascosta[4][7] = '2';
    */
    impostaGiocatoreVisibile(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile); // MOSTRA A SCHERMO LE MIE NAVI
    stampaConDelay("FLOTTE PRONTE ALLA BATTAGLIA!");
    cout << endl
         << endl;
    Sleep(1500);
    stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
    int *bersagliColpiti = new int[NUMERONAVI];
    int affondato = -2;
    while ((!finePartita(grigliaPCVisibile)) and (!finePartita(grigliaGiocatoreVisibile))) // CICLO DELLA PARTITA, CONTINUA FINCHE UNO DEI DUE GIOCATORI NON VINCE
    {
        if (turno) // A SECONDA DEL VALORE GIOCA IL GIOCATORE O IL PC
        {
            Sleep(1500);
            while (turnoGiocatore(grigliaPCNascosta, grigliaPCVisibile) and (!finePartita(grigliaPCVisibile))) // TURNO DEL GIOCATORE, TERMINA SE E' FINITA LA PARTITA OPPURE LA MOSSA NON HA CENTRATO BERSAGLI
            {
                // checkBersagli restituisce un array che per ogni nave indichi quante volte e' stata colpita.
                // isColpitoEAffondato riporta il numero della nave che è stata affondata
                affondato = isColpitoEAffondato(checkBersagli(grigliaPCNascosta, grigliaPCVisibile));
                if (affondato != -1) // SE UNA NAVE E' STATA AFFONDATA
                {
                    mostraNaveAbbattuta(grigliaPCNascosta, grigliaPCVisibile, affondato); // SOSTITUISCE LE 'X' CON IL NUMERO INDICATIVO DELLA NAVE STESSA
                    stampaConDelay("Colpito e affondato!\n");
                }
                else
                {
                    stampaConDelay("Colpito!\n");
                }
                stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
            }
            stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
            turno = !turno; // PASSA IL TURNO AL PC
        }
        else
        {
            Sleep(1500);
            while (turnoPC(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile) and (!finePartita(grigliaGiocatoreVisibile))) // TURNO DEL PC, TERMINA SE E' FINITA LA PARTITA OPPURE LA MOSSA NON HA CENTRATO BERSAGLI
            {
                // checkBersagli restituisce un array che per ogni nave indichi quante volte e' stata colpita.
                // isColpitoEAffondato riporta il numero della nave che è stata affondata
                bersagliColpiti = checkBersagli(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile);
                affondato = isColpitoEAffondato(bersagliColpiti);
                if (affondato != -1) // SE UNA NAVE E' STATA AFFONDATA
                {
                    mostraNaveAbbattuta(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile, affondato); // SOSTITUISCE LE 'X' CON IL NUMERO INDICATIVO DELLA NAVE STESSA
                    stampaConDelay("Ti hanno colpito e affondato una nave...\n");
                }
                else
                {
                    stampaConDelay("Sei stato colpito...\n");
                }
                stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
            }
            stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
            turno = !turno; // PASSA IL TURNO AL GIOCATORE
        }
    }

    if (finePartita(grigliaPCVisibile)) // SE LA PARTITA E' STATA VINTA DAL GIOCATORE
    {
        affondato = isColpitoEAffondato(checkBersagli(grigliaPCNascosta, grigliaPCVisibile));
        mostraNaveAbbattuta(grigliaPCNascosta, grigliaPCVisibile, affondato); // MOSTRA TUTTE LE NAVI DEL PC
        stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
        cout << endl;
        stampaConDelay("HAI VINTO LA BATTAGLIA!!!");
        cout << endl;
    }
    else // SE LA PARTITA E' STATA VINTA DAL PC
    {
        affondato = isColpitoEAffondato(checkBersagli(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile));
        mostraNaveAbbattuta(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile, affondato); // MOSTRA TUTTE LE NAVI DEL GIOCATORE
        stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
        cout << endl;
        stampaConDelay("Peccato... hai perso...");
        cout << endl;
    }
    system("pause");
}
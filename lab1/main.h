#ifndef MERGE_H
#define MERGE_H

# define SUCCESS 0
# define A_NOT_FOUND 1
# define B_NOT_FOUND 2
# define OUT_OF_RANGE 3

typedef struct Row {
	char* context;
	int capacity;
} Row;

typedef struct Block {
	Row* rows;
	int amount;
	int capacity;
} Block;

typedef struct Table {
	Block* blocks;
	int amount;
	int capacity;
} Table;

/*Utworzenie tablicy wskaźników (tablicy głównej) — w tej tablicy będą
*przechowywane wskaźniki na wiersze zmergowanych plików — pierwszy element
*tablicy głównej zawiera wykaz wierszy pierwszej pary zmergowanych plików, drugi
*element dla drugiej pary, itd. Pojedynczy blok wierszy (element wskazywany z tablicy
*głównej), to tablica wskaźników na poszczególne wiersze w zmergowanym pliku
*/
//void create_table();

// Definiowanie sekwencji par plików
void define_seq();

/*Przeprowadzenie zmergowania (dla każdego elementu sekwencji) oraz zapisanie
wyniku zmergowania do pliku tymczasowego
*/
void merge_files(Table* table, const char* filepath_a, const char* filepath_b);

/*Utworzenie, na podstawie zawartość pliku tymczasowego, bloku wierszy — tablicy
wskaźników na wiersze, ustawienie w tablicy głównej (wskaźników) wskazania na ten
blok; na końcu, funkcja powinna zwrócić indeks elementu tablicy (głównej), który
zawiera wskazanie na utworzony blok*/
int create_new_block(Table* table);

// Zwrócenie informacji o ilości wierszy w danym bloku wierszy
int number_of_rows(Table* table, int block_index);

//Usunięcie, z pamięci, bloku (wierszy) o zadanym indeksie
void remove_block(Table* table, int index);

//Usunięcie, z pamięci, określonego wiersza dla podanego bloku wierszy
void remove_row(Table* table, int index);

/*Wypisanie zmergowanych plików, z uwzględnieniem wcześniejszych usuniętych
bloków wierszy / wierszy*/

int write_to_file(Table* table);

#endif

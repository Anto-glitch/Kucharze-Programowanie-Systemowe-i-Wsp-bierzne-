#!/bin/bash

# Sprawdzenie czy plik źródłowy istnieje
if [ ! -f kucharze.c ]; then
    echo "Błąd: Nie znaleziono pliku kucharze.c"
    exit 1
fi

echo "Kompilacja programu kucharze..."
gcc -Wall -pthread -o kucharze kucharze.c

if [ $? -eq 0 ]; then
    echo "Kompilacja zakończona sukcesem. Uruchom wpisując: ./kucharze"
else
    echo "Wystąpił błąd podczas kompilacji."
fi
1. Opis rozwiązania
   Program jest połączeniem dwóch klasycznych problemów synchronizacji:

Problemu Producenta i Konsumenta: Kucharze wytwarzają dania (zajmując miejsce i obciążając stół) oraz je konsumują (zwalniając zasoby). Rola kucharza jest dynamiczna i zależy od stanu układu.

Problemu Ucztujących Filozofów: Dostęp do stołu jest ograniczony przez konieczność posiadania dwóch widelców jednocześnie.

Zarządzanie Zasobami:
-(Semafory) Zastosowano zestaw 4 semaforów do kontroli zasobów:

-Widelce (FORKS): Zasób dostępowy. Aby wykonać jakąkolwiek akcję przy stole (jedzenie lub gotowanie), kucharz musi atomowo pobrać 2 widelce.

-Miejsca (K) i Waga (W): Ograniczenia bufora (stołu). Gotowanie zmniejsza te zasoby (zabiera miejsce i dodaje ciężar), a jedzenie je zwalnia.

-Dania: Licznik dostępnych produktów. Jedzenie wymaga wartości > 0.

Logika Decyzyjna i Zapobieganie Zakleszczeniom:

-Inteligentna decyzja (myslenie): Proces nie losuje ślepo. Sprawdza stan semaforów (semctl GETVAL). Jeśli nie ma dań – musi gotować. Jeśli nie ma miejsca lub wagi – musi jeść. Zapobiega to sytuacji, w której wszyscy chcą jeść przy pustym stole lub gotować przy pełnym.

-Korekta przy stole (przy_stole): Nawet po zajęciu widelców następuje ponowna weryfikacja. Jeśli kucharz chciał gotować, ale w międzyczasie skończyło się miejsce (waga<=1 lub dania==3), program wymusza zmianę akcji na jedzenie. To kluczowy mechanizm zapobiegający zablokowaniu systemu.

Atomowość:
Pobranie dwóch widelców naraz (opuscSemafor(..., 2)) eliminuje klasyczne zakleszczenie filozofów (gdzie każdy trzyma jeden widelec i czeka na drugi).

2. INSTRUKCJA URUCHOMIENIA

Wymagania:

- System operacyjny z rodziny Unix/Linux.
- Kompilator GCC.

Kompilacja:
Aby skompilować program, użyj dołączonego skryptu
Nadaj uprawnienia i uruchom w cmd:
chmod +x make.sh
./make.sh

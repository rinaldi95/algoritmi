# AGENTS.md – Linear Search

## Scopo
- Implementare la ricerca lineare: dato un array e un valore target, trovare la prima posizione che lo contiene (o segnalare assenza).

## Input/Output
- Input: array di interi (lunghezza >= 0), dimensione nota `n`, valore target intero.
- Output: indice della **prima** occorrenza se presente, altrimenti -1 (per C/C++/Python). In assembly: flag/registro indicante trovato e indice concordato.

## Criteri di correttezza
- Se il target è presente almeno una volta, restituisce l’indice più piccolo che lo contiene.
- Se assente, restituisce -1 senza modificare l’array.

## Edge case obbligatori
- Array vuoto.
- Array di un elemento (match / no match).
- Duplicati (deve dare la **prima** occorrenza).
- Target non presente.
- Target in prima e ultima posizione.

## Pseudocodice sintetico
1. per i da 0 a n-1: se A[i] == x, ritorna i
2. se il ciclo termina senza trovare, ritorna -1

## Ordine dei micro-blocchi di codice
1) Definire la firma della funzione `linear_search` (array, n, target) e il valore di ritorno (-1 o indice).
2) Implementare il ciclo for/while con confronto e return immediato alla prima occorrenza.
3) Gestire il caso "non trovato" restituendo -1.
4) Scrivere un piccolo main/driver con input statici e stampa del risultato (test manuali) per ciascun linguaggio.
5) Aggiungere i test minimi elencati sotto.

## Test minimi
- [] vuoto, target qualsiasi -> -1.
- [5], target 5 -> 0 ; [5], target 7 -> -1.
- [1, 2, 3], target 1 -> 0 ; target 3 -> 2 ; target 4 -> -1.
- [4, 4, 4], target 4 -> 0 (prima occorrenza).
- [2, 9, 2, 9], target 9 -> 1 ; target 2 -> 0.

## Note per linguaggio (adatta)
- C: attenzione a limiti dell’array e al tipo di ritorno; test con input statici; nessuna allocazione non necessaria.
- C++: usare `std::vector<int>&` e restituire `int` (index o -1).
- Python: lista di int, restituire index o -1; codice lineare e semplice.
- Assembly: procedere per micro-step; registri per indice, valore corrente, flag per trovato; test con array corto in memoria statica.

## Regole di interazione (da seguire sempre)
- Pochissimo codice per volta; spiegazione di ogni blocco.
- Nessun copia-incolla; l’utente scrive a mano.
- Conferma esplicita prima di passare al blocco successivo.
- Non creare/modificare file automaticamente; proporre soltanto.

## Tracking locale
- Data inizio:
- Stato: spiegato / codifica avviata / test minimi ok
- Linguaggi coperti:
- Problemi aperti / note:

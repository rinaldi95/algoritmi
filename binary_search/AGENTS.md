# AGENTS.md – Binary Search

## Scopo
- Implementare la ricerca binaria su un array ordinato non decrescente per trovare la posizione di un valore target o segnalare l’assenza.

## Input/Output
- Input: array di interi ordinato in modo non decrescente, lunghezza `n` (n ≥ 0), valore target intero.
- Output: indice di un’istanza del target se presente; altrimenti -1. Per coerenza con linear search, usiamo -1 in C/C++/Python; in assembly flag/registro come concordato.

## Criteri di correttezza
- Se il target è presente almeno una volta, restituisce un indice valido che lo contiene (qualsiasi occorrenza). Se si vuole la prima/ultima occorrenza, adattare la variante.
- Se il target è assente, restituisce -1 senza modificare l’array.

## Edge case obbligatori
- Array vuoto.
- Array con 1 elemento (match / no match).
- Target minore del primo elemento, maggiore dell’ultimo.
- Target presente all’inizio, alla fine, al centro.
- Duplicati: verificare che l’indice restituito sia di una posizione corretta (se si vuole la prima occorrenza, specificare la variante).

## Pseudocodice sintetico
1) `low = 0`, `high = n - 1`
2) mentre `low <= high`:
   - `mid = low + (high - low) / 2`
   - se `A[mid] == x` ritorna `mid`
   - se `A[mid] < x` allora `low = mid + 1` altrimenti `high = mid - 1`
3) se termina il ciclo, ritorna -1

## Ordine dei micro-blocchi di codice
1) Definire la funzione `binary_search(array, n, target)` e il valore di ritorno (-1 o indice).
2) Inizializzare low/high e implementare il ciclo con calcolo di mid sicuro (evita overflow: `low + (high - low) / 2`).
3) Gestire i tre casi: trovato, target a destra, target a sinistra; aggiornare i limiti.
4) Restituire -1 se esaurita la ricerca.
5) Scrivere un piccolo main/driver con test statici e stampa dei risultati.
6) Aggiungere i test minimi elencati sotto.

## Test minimi
- [] vuoto, target qualsiasi -> -1.
- [5], target 5 -> indice 0 ; target 7 -> -1.
- [1, 2, 3, 4], target 1 -> 0 ; target 4 -> 3 ; target 3 -> 2 ; target 5 -> -1 ; target 0 -> -1.
- [2, 4, 4, 4, 9], target 4 -> indice tra 1 e 3 (specifica se vuoi la prima occorrenza).
- [1, 3, 5, 7, 9, 11], target 9 -> 4.

## Tracciamento prestazioni
- Misura iterazioni/confronti invece del tempo (ricerca binaria è troppo rapida su input piccoli).
- Prova input con n = 0, 1, 10, 100, 1_000, 10_000 (array ordinati). Per confronto empirico, puoi confrontare con linear search sulle stesse taglie.
- Atteso: O(log n); verifica che il numero di passi cresca lentamente (circa log2(n)).

## Note per linguaggio (adatta)
- C: attenzione agli indici e al calcolo di mid; usare `int` coerenti; nessuna allocazione non necessaria.
- C++: usare `std::vector<int>&`; stesso calcolo di mid; restituire `int` index/-1.
- Python: lista di int; implementazione iterativa semplice; restituire index/-1.
- Assembly: mantenere registri per low/high/mid; aggiornare i limiti con confronto e salto; test su array statico corto.

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

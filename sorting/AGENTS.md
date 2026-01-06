# AGENTS.md – Sorting Module (multiple algorithms)

## Mini-contratto (in parole semplici)
- Operazioni: fornire più algoritmi di ordinamento in place per array di interi: insertion sort, selection sort, merge sort, quicksort, heap sort. Facoltativo: stable flag o varianti.
- Input: puntatore a `int` e lunghezza `n` (n >= 0). L’array viene modificato in ordine non decrescente.
- Output: funzioni `void` (o `int` solo per segnalare errori gravi). Non restituiscono copia; ordinano in place.
- Errori: se `n <= 1`, non fanno nulla. Se il puntatore è NULL con `n > 0`, comportamento indefinito (assumiamo input valido per semplicità).
- Edge case: array vuoto, array di 1 elemento, duplicati, array già ordinato, array inverso.
- Complessità attesa: insertion/selection O(n^2); merge O(n log n) stabile con spazio O(n); quick O(n log n) medio, O(n^2) peggiore; heap O(n log n) in place non stabile.
- Invarianti: dopo la chiamata, l’array è ordinato non decrescente; nessuna perdita di dati.

## Struttura proposta
- `sort.h`: firme di tutti gli algoritmi (insertion_sort, selection_sort, merge_sort, quick_sort, heap_sort). Eventuali helper nascosti restano in `sort.c`.
- `sort.c`: implementazioni; helper statici per merge, partition, heapify.
- `tests.c`: driver comune che genera array di test, clona l’array per ogni algoritmo, verifica correttezza (ordinato e stessi elementi).
- `bench.c` (facoltativo): benchmark comune su input condivisi; stampa CSV `alg,n,case,ns_per_call` o conteggio operazioni.

## Input/Output formali
- Input: `int *a`, `int n` (n >= 0). Array modificato in place.
- Output: `void` (o `int` per segnalare errori di allocazione in merge_sort). Funzioni non restituiscono copia.

## Criteri di correttezza
- Ordine non decrescente dopo l’esecuzione.
- Stessa multiset di elementi (nessuna perdita/duplicazione).
- Niente accessi fuori dai limiti.

## Edge case obbligatori
- n = 0, n = 1.
- Array già ordinato; array inverso.
- Duplicati; tutti uguali.
- Per quicksort: caso pessimo (array ordinato/inverso) e verifica degrado se non c’è pivot robusto.
- Per merge sort: test con n non potenza di 2; verifica che sia stabile se la implementi stabile.

## Pseudocodice sintetico (per ogni algoritmo)
- Insertion: per i=1..n-1, inserisci a[i] nella parte ordinata a[0..i-1].
- Selection: per i=0..n-1, trova min in a[i..n-1], scambia con a[i].
- Merge: dividi, ordina ricorsivamente, fondi con buffer temporaneo; O(n) extra.
- Quick: scegli pivot, partiziona, ricorri su sottoarray; tail recursion o iterativo per limitare stack; pivot mediana o random per evitare pessimi casi.
- Heap: build heap O(n), poi estrai max/min e metti in coda (heap sort in place non stabile).

## Ordine dei micro-blocchi di codice
1) Definire `sort.h` con firme e breve nota su stabilità/complessità.
2) Implementare insertion_sort e selection_sort (baseline O(n^2)).
3) Implementare merge_sort con buffer temporaneo (gestione alloc e free).
4) Implementare quick_sort con partizione e scelta pivot (robusta: mediana di 3 o random); tail recursion o versione iterativa per evitare stack profondo.
5) Implementare heap_sort (build heap, heapify, estrazione). Puoi riusare un heap helper nel file.
6) Scrivere `tests.c` con casi comuni: vuoto/uno, ordinato, inverso, duplicati, casuale; verifica ordinamento e multiset (checksum/somma o confronto con copia ordinata via qsort di libreria).
7) (Opzionale) Scrivere `bench.c` per confrontare tempi/iterazioni su input crescenti e casi degenerati (es. quick con array ordinato). Output CSV.

## Test minimi (per ogni algoritmo)
- n=0, n=1 (no crash, array invariato).
- [1,2,3] (già ordinato) -> invariato.
- [3,2,1] (inverso) -> [1,2,3].
- Duplicati: [4,4,1,1] -> [1,1,4,4]; se stabile, mantieni ordine relativo (solo merge).
- Casualità: array random piccolo (es. n=10) e verifica con riferimento (qsort).
- Edge per quick: array ordinato/inverso per osservare degrado se pivot semplice; per merge: n dispari.

## Tracciamento prestazioni
- Atteso: insertion/selection O(n^2); merge O(n log n) con spazio O(n); quick O(n log n) medio (peggiore O(n^2)); heap O(n log n).
- Empirico: input di taglie n = 10, 100, 1_000, 10_000 (o più) e casi diversi (ordinato, inverso, random, molti duplicati). Stampa `alg,n,case,tempo` o `iterazioni`.
- Confronta andamenti con teoria; nota eventuali pessimi casi di quick.

## Note per linguaggio (adatta)
- C: usa `malloc/free` per merge; evita overflow in indici; per quick, scegli pivot robusto.
- C++: puoi usare template/comparator, ma qui resta su `int` per semplicità.
- Python: implementazioni didattiche; per benchmark evita la sort built-in.
- Assembly: obiettivo minimo = capire i passi logici; implementazione completa è avanzata.

## Regole di interazione (da seguire sempre)
- Pochissimo codice per volta; spiegazione di ogni blocco.
- Nessun copia-incolla; l’utente scrive a mano.
- Conferma esplicita prima di passare al blocco successivo.
- Non creare/modificare file automaticamente; proporre soltanto.

## Tracking locale
- Data inizio:
- Stato: spiegato / codifica avviata / test minimi ok / bench
- Linguaggi coperti:
- Problemi aperti / note:

# AGENTS.md – Min-Heap (Priority Queue)

## Mini-contratto (in parole semplici)
- Operazioni: inserisci un valore, guarda il minimo, estrai il minimo, controlla se vuoto (e size).
- Input: interi (inizio). Duplicati permessi.
- Output: insert successo/errore; peek/extract restituiscono il minimo; se vuoto segnalano errore.
- Edge case: heap vuoto, heap con un solo elemento, duplicati, resize.
- Complessità attesa: peek O(1), insert O(log n), extract-min O(log n), build/heapify O(n).
- Invarianti: in ogni nodo il valore è <= dei figli; rappresentazione in array con indice padre/figli coerente.

## Scopo
- Implementare una struttura Min-Heap con array dinamico (priority queue).

## Input/Output
- Input: valore int.
- Output: `push` ritorna 0/-1; `peek` e `extract_min` ritornano 0/-1 e scrivono in out param.

## Criteri di correttezza
- Il minimo è sempre in cima (indice 0).
- Dopo insert o extract, la proprietà di heap resta vera.

## Edge case obbligatori
- Heap vuoto: peek/extract devono fallire senza crash.
- Un solo elemento: insert, peek, extract, torna vuoto.
- Duplicati: permessi e gestiti correttamente.
- Resize: supera capacità iniziale, tutti i dati restano corretti.

## Pseudocodice sintetico
- `push(x)`: inserisci in fondo; risali (sift-up) finché il padre è <=.
- `peek()`: restituisce data[0].
- `extract_min()`: salva data[0], sposta ultimo in cima, riduci size, scendi (sift-down) finché valido.
- `heapify/build`: da array non ordinato, costruisci heap in O(n) partendo dagli ultimi nodi interni.

## Ordine dei micro-blocchi di codice
1) Definire struct Heap { int *data; int size; int capacity; }.
2) Implementare init/destroy, is_empty, size.
3) Implementare `sift_up` e `sift_down` (helper).
4) Implementare `push` (con resize se necessario).
5) Implementare `peek` e `extract_min`.
6) (Opzionale) implementare `build_heap` da array.
7) Test su edge case e sequenze di inserimenti/estrazioni.

## Test minimi
- Vuoto: peek/extract falliscono.
- Un elemento: insert, peek, extract -> vuoto.
- Sequenza: insert [5,3,8,1,4], estrazioni devono dare 1,3,4,5,8.
- Duplicati: insert [2,2,2], estrazioni tutte 2.
- Resize: capacity iniziale 4, inserisci 5+ elementi.

## Tracciamento prestazioni
- Insert/extract O(log n), peek O(1).
- Empirico: inserisci n = 10,100,1_000,10_000 e misura tempo medio di insert/extract.

## Note per linguaggio (adatta)
- C: gestire realloc; indici padre/figli (parent=(i-1)/2, left=2i+1, right=2i+2).
- C++: vector e funzioni helper; stessi indici.
- Python: lista e helper; oppure `heapq` come riferimento.
- Assembly: avanzato; obiettivo minimo = capire heapify e sift.

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

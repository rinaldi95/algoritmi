# AGENTS.md – Queue (FIFO)

## Scopo
- Implementare una coda FIFO: inserimento in fondo (`enqueue`), rimozione dalla testa (`dequeue`), lettura della testa (`front`/`peek`), `is_empty` (eventuale `size`). In C: usare un buffer circolare con array dinamico e indici di testa/coda.

## Input/Output
- Input: sequenza di operazioni enqueue/dequeue/front su interi (inizio). Capacità si espande con resize.
- Output: valore in testa per `dequeue`/`front`; gestione underflow se la coda è vuota; eventuale codice di errore.

## Criteri di correttezza
- `enqueue` aggiunge in fondo; `dequeue` rimuove e restituisce l’elemento più vecchio (prima inserito ancora presente); `front` legge la testa senza rimuovere; `is_empty` indica se vuota.
- Nessuna perdita di dati; l’ordine FIFO è preservato.
- Underflow gestito in modo sicuro; overflow gestito con resize.

## Edge case obbligatori
- Dequeue/front su coda vuota.
- Sequenze alternate enqueue/dequeue (testare che la lunghezza non diventi negativa).
- Riempire fino alla capacità, poi aggiungere un elemento per forzare il primo resize (es. cap iniziale 4 -> 5 elementi).
- Uso del buffer circolare: enqueue/dequeue che fanno “girare” gli indici (verifica che il wrap-around mantenga l’ordine).
- (Facoltativo) shrink della capacità se si riduce molto la size (con soglia minima).

## Pseudocodice sintetico (buffer circolare)
- Struttura: buffer array, size, capacity, head (indice testa), tail (indice posizione di inserimento).
- `enqueue(x)`: se size == capacity, raddoppia capacity e riallinea il buffer (copia in ordine da head a tail in un nuovo array); inserisci x in tail; tail = (tail+1)%capacity; size++.
- `dequeue()`: se size == 0 -> underflow; altrimenti prendi buffer[head], head = (head+1)%capacity; size--.
- `front()`: simile a dequeue ma senza modificare head/size.
- `is_empty()`: size == 0.

## Ordine dei micro-blocchi di codice
1) Definire struct Queue { int *data; int size; int capacity; int head; int tail; } e funzioni init/destroy.
2) Implementare is_empty e (facoltativo) size accessor.
3) Implementare enqueue con resize e riallineamento (copia in ordine logico nel nuovo buffer), aggiornando head=0, tail=size dopo il resize.
4) Implementare dequeue e front con gestione underflow (codice di errore e out param).
5) Scrivere un main/driver con test statici che coprano wrap-around e resize.
6) Aggiungere tracciamento prestazioni (vedi sotto) se vuoi misure empiriche.

## Test minimi
- Coda vuota: is_empty true; dequeue/front segnalano underflow.
- Un elemento: enqueue, front restituisce l’elemento, dequeue lo rimuove; coda torna vuota.
- Alternanza: enqueue 1, enqueue 2, dequeue -> 1, front -> 2, dequeue -> 2, is_empty true.
- Wrap-around: con capacity 4, enqueue 1,2,3,4; dequeue due volte; enqueue 5,6; verifica ordine: dequeue -> 3,4,5,6.
- Resize: capacity 4, enqueue 5 elementi, verifica ordine FIFO dopo il resize.

## Tracciamento prestazioni
- Operazioni attese O(1) ammortizzato con resize. Sequenze di n enqueue + n dequeue dovrebbero costare O(n) totale.
- Misura iterazioni o tempo su n = 10, 100, 1_000, 10_000; controlla che ogni operazione resti ~costante mediamente.

## Note per linguaggio (adatta)
- C: attenzione a wrap-around con mod; gestione realloc nel resize e riallineamento dati; controllare errori malloc/realloc.
- C++: usare `std::vector<int>` e indici circolari, oppure `std::deque` se vuoi una versione pronta (ma qui meglio implementare a mano per l’esercizio).
- Python: implementare con lista e indici, o `collections.deque` come riferimento; se a mano, gestire wrap-around.
- Assembly: buffer statico e indici testa/coda; attenzione ai confronti e al modulo.

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

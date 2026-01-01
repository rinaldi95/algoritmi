# AGENTS.md – Deque (double-ended queue)

## Scopo
- Implementare una deque (double-ended queue) di interi: inserimento e rimozione sia in testa sia in coda (`push_front`, `push_back`, `pop_front`, `pop_back`), più `front`, `back`, `is_empty`, (eventuale `size`). Buffer circolare con array dinamico e indici testa/coda.

## Input/Output
- Input: sequenza di operazioni su interi (inizio). Capacità si espande con resize.
- Output: valori restituiti da pop/front/back; gestione underflow se vuota; eventuali codici di errore.

## Criteri di correttezza
- `push_front`/`push_back` inseriscono agli estremi corretti; `pop_front`/`pop_back` rimuovono dagli estremi corretti; `front`/`back` leggono senza rimuovere.
- Nessuna perdita di dati; ordine mantenuto; indici coerenti anche con wrap-around e resize.
- Underflow gestito in modo sicuro; resize gestito correttamente con riallineamento del buffer.

## Edge case obbligatori
- Operazioni su deque vuota (pop/front/back falliscono in modo sicuro).
- Sequenze alternate di push/pop su entrambi i lati (no indici negativi, no overflow logico).
- Wrap-around: inserisci, togli, reinserisci per far girare head/tail e verificare ordine.
- Resize: riempi fino alla capacità, poi inserisci oltre; controlla che l’ordine resti corretto dopo il raddoppio.

## Pseudocodice sintetico (buffer circolare)
- Struttura: buffer array, size, capacity, head, tail (head = indice dell’elemento in testa; tail = posizione per inserire in coda).
- `push_back(x)`: se pieno, resize; buffer[tail]=x; tail=(tail+1)%capacity; size++.
- `push_front(x)`: se pieno, resize; head=(head-1+capacity)%capacity; buffer[head]=x; size++.
- `pop_front()`: se vuoto -> underflow; v=buffer[head]; head=(head+1)%capacity; size--.
- `pop_back()`: se vuoto -> underflow; tail=(tail-1+capacity)%capacity; v=buffer[tail]; size--.
- `front()`: legge buffer[head] se non vuoto; `back()`: legge buffer[(tail-1+capacity)%capacity] se non vuoto.
- Resize: alloca nuovo buffer 2x, copia gli elementi in ordine logico partendo da head, poi set head=0, tail=size, capacity=new_capacity.

## Ordine dei micro-blocchi di codice
1) Definire struct Deque { int *data; int size; int capacity; int head; int tail; } e funzioni init/destroy.
2) Implementare is_empty e (facoltativo) size accessor.
3) Implementare `push_back` (coda) e `push_front` (testa) con wrap-around; gestire resize prima di scrivere.
4) Implementare `pop_front` e `pop_back` con underflow sicuro; `front` e `back` (solo lettura).
5) Scrivere main/driver con test che coprano vuota, alternanze, wrap, resize.
6) Aggiungere tracciamento prestazioni se vuoi misure empiriche (conteggio iterazioni o tempo per sequenze di operazioni).

## Test minimi
- Vuota: is_empty true; pop/front/back falliscono.
- Un elemento: push_front, front/back == elem, pop_front lo rimuove; ripeti con push_back/pop_back.
- Alternanza: push_front(a), push_back(b), pop_front -> a, pop_back -> b; e varianti.
- Wrap-around: con cap 4, esegui push_front/push_back misti e pop per far girare gli indici; verifica ordine.
- Resize: cap 4, inserisci 5+ elementi, verifica ordine corretto con pop da entrambe le estremità.

## Tracciamento prestazioni
- Ogni operazione O(1) ammortizzato (resize raro). Sequenze di n push + n pop dovrebbero costare O(n) totale.
- Empirico: prova n = 10, 100, 1_000, 10_000; controlla che ogni operazione resti ~costante mediamente.

## Note per linguaggio (adatta)
- C: gestione wrap con modulo; resize con copia in ordine logico; controlla errori malloc/realloc.
- C++: `std::deque` esiste, ma qui implementa manualmente con vector+indici.
- Python: `collections.deque` esiste; implementazione manuale serve solo a capire il modello.
- Assembly: buffer statico e indici; attenzione ai decrementi e modulo.

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

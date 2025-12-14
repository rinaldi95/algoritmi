# AGENTS.md – Stack (LIFO)

## Scopo
- Implementare una pila (stack) LIFO con operazioni di base: push, pop, top/peek, is_empty (eventualmente size). In C: stack basato su array dinamico o su lista; qui partiamo da array dinamico.

## Input/Output
- Input: sequenza di operazioni push/pop/top su elementi (interi per iniziare). Capacità cresce con resize.
- Output: risultati di top/pop; gestione di underflow (se pop/top su stack vuoto) e eventuali errori/flag.

## Criteri di correttezza
- push inserisce in cima; pop rimuove e restituisce l’ultimo inserito; top legge senza rimuovere; is_empty indica se vuoto.
- Nessuna perdita di dati; l’ordine LIFO è preservato.
- Underflow gestito in modo sicuro (return sentinel/codice errore/stampa messaggio). Overflow non rilevante con resize dinamico.

## Edge case obbligatori
- Pop/top su stack vuoto.
- Sequenza alternata push/pop (testare che la dimensione non diventi negativa).
- Push multipli fino a causare il primo resize (es. capacità iniziale 4 -> push 5 elementi).
- Pop fino a vuoto dopo un resize (verifica che i dati restino corretti e nessun double-free).
- (Se implementi shrink) Pop che porta a ridurre la capacità.

## Pseudocodice sintetico (array dinamico)
- Struttura: buffer array, size attuale, capacity.
- push(x): se size == capacity, raddoppia capacity e realloc; buffer[size]=x; size++.
- pop(): se size==0 -> underflow; altrimenti size--, restituisci buffer[size].
- top(): se size==0 -> underflow; altrimenti restituisci buffer[size-1].
- is_empty(): size==0.

## Ordine dei micro-blocchi di codice
1) Definire la struct Stack { int *data; int size; int capacity; } e funzioni di init/destroy.
2) Implementare is_empty e (facoltativo) size accessor.
3) Implementare push con check di resize (capacity raddoppia) e gestione degli errori di allocazione.
4) Implementare pop e top con gestione underflow (sentinel o codice errore); decidere il tipo di ritorno e come segnalare l’errore.
5) Scrivere un main/driver con test statici che coprano edge case.
6) Aggiungere il tracciamento prestazioni (vedi sotto) se pertinente.

## Test minimi
- Stack vuoto: is_empty true; pop/top gestiscono underflow.
- Push di 1 elemento: top restituisce quell’elemento; pop lo rimuove; stack torna vuoto.
- Sequenza push/pop alternata (es. push 1, push 2, pop -> 2, top -> 1, pop -> 1, is_empty true).
- Resize: con capacity iniziale 4, push 5 elementi, verifica ordine LIFO con pop successivi.
- (Se implementi shrink) Dopo molti pop, capacità non deve scendere sotto una soglia minima.

## Tracciamento prestazioni
- Misura conteggi di operazioni: push/pop sono O(1) ammortizzato con resize raddoppio.
- Per verifica empirica: sequenze di n push seguiti da n pop e controlla tempo o iterazioni. Atteso: tempo ~ O(n) totale, per singola operazione ~ O(1) ammortizzato.
- Dimensioni input: n = 10, 100, 1_000, 10_000.

## Note per linguaggio (adatta)
- C: gestire alloc/free; controllare esiti malloc/realloc; inizializzare capacity minima (es. 4). Evitare leak.
- C++: usare `std::vector<int>` o implementare il proprio buffer; sfruttare RAII.
- Python: usare list come stack (`append`/`pop`), ma se implementi manualmente, concentrati su interfaccia e test.
- Assembly: usare un buffer statico e un indice di cima; gestire overflow/underflow.

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

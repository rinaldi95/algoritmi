# AGENTS.md – Singly Linked List

## Scopo
- Implementare una lista concatenata singola di interi con operazioni base: inserimento (in testa e/o in coda), ricerca di un valore, cancellazione di un nodo per valore, stampa/iterazione, controllo se vuota.

## Input/Output
- Input: sequenza di operazioni sulla lista; dati interi per iniziare.
- Output: esito delle operazioni (trovato/non trovato, successo/fallimento), eventuali valori restituiti (per operazioni che rimuovono/leggono).

## Criteri di correttezza
- I nodi sono collegati in una catena tramite `next`; nessun riferimento perso (niente memory leak sulle cancellazioni).
- Le operazioni mantengono la lista consistente (nessun ciclo non voluto, next corretti).
- Inserimento in testa/coda posiziona il nuovo nodo nel punto giusto; cancellazione rimuove solo il primo match (specifica) o tutti (se decidi così).

## Edge case obbligatori
- Lista vuota: ricerca fallisce, cancellazione fallisce, stampa non crasha.
- Lista con un solo elemento: inserimento e cancellazione gestite correttamente (diventa vuota dopo delete).
- Inserimento multiplo in testa/coda preserva l’ordine atteso.
- Cancellazione di un nodo in testa, in mezzo, in coda; cancellazione di valore assente.

## Pseudocodice sintetico
- `push_front(x)`: crea nodo, `node->next = head`, `head = node` (se tieni tail, aggiorna se era vuota).
- `push_back(x)`: se tail noto, collega `tail->next = node`, `tail = node`; se non hai tail, itera fino a fine.
- `find(x)`: scorri con puntatore corrente; se `curr->value == x`, restituisci riferimento/esito.
- `delete_value(x)`: gestisci caso head match, altrimenti scorri con `prev` e `curr`, collega `prev->next = curr->next`, free del nodo.
- `is_empty`: `head == NULL`.

## Ordine dei micro-blocchi di codice
1) Definire struct `Node { int value; Node *next; }` e struct `List { Node *head; Node *tail; }` (tail facoltativa ma utile per `push_back` O(1)).
2) Implementare init/destroy (destroy itera e libera tutti i nodi).
3) Implementare `is_empty`.
4) Implementare `push_front`; poi `push_back` (aggiorna tail, gestisci lista vuota).
5) Implementare `find` (restituisce puntatore o boolean/esito).
6) Implementare `delete_value` (gestisci head, nodo in mezzo, nodo in coda, valore assente).
7) Main/driver con test statici che coprano gli edge case.

## Test minimi
- Lista vuota: `is_empty` true; `find` fallisce; `delete` fallisce senza crash.
- Un elemento: inserisci (front o back), `find` ok, `delete` lo rimuove, lista torna vuota.
- Inserimenti multipli: `push_front(1), push_front(2), push_front(3)` -> ordine atteso 3,2,1; `push_back` mantiene ordine FIFO per la coda.
- Cancella head, cancella un valore in mezzo, cancella l’ultimo; cancella un valore assente.
- (Se usi tail) dopo cancellazioni controlla coerenza di tail.

## Tracciamento prestazioni
- Operazioni: `push_front` O(1); `push_back` O(1) se hai tail, altrimenti O(n); `find` e `delete` O(n) nel peggiore.
- Empirico: misura iterazioni/tempo su liste di n = 10, 100, 1_000, 10_000 per `find`/`delete`, verifica crescita ~ lineare.

## Note per linguaggio (adatta)
- C: gestire malloc/free per ogni nodo; attenzione a leak/dangling; aggiornare tail se la usi.
- C++: usare `new/delete` (o smart pointer se vuoi esercitarti); tail facoltativa.
- Python: implementazione a scopo didattico con oggetti (ma in pratica useresti list/deque); concentrati sul modello.
- Assembly: buffer di nodi statico o allocazioni manuali; molto più complesso, obiettivo minimo = capire la catena di puntatori.

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

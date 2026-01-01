# AGENTS.md – Doubly Linked List

## Scopo
- Implementare una lista doppiamente collegata di interi con operazioni base: inserimento in testa e coda, ricerca, cancellazione di un nodo per valore, inserimento dopo (o prima) di un nodo, iterazione in avanti (e se vuoi indietro), controllo se vuota. Puntatori `prev` e `next` permettono inserimenti/cancellazioni O(1) dato il nodo.

## Input/Output
- Input: sequenza di operazioni; dati interi.
- Output: esito delle operazioni (trovato/non trovato, successo/fallimento), eventuali valori restituiti.

## Criteri di correttezza
- Ogni nodo punta al precedente e al successivo in modo coerente (head->prev = NULL, tail->next = NULL).
- Inserimenti/cancellazioni mantengono i link corretti; nessun nodo perso (niente leak), nessun ciclo non voluto.
- Coerenza di head/tail dopo ogni operazione.

## Edge case obbligatori
- Lista vuota: find/delete falliscono senza crash, stampa/iterazione sicura.
- Un solo elemento: inserisci, cancella (torna vuota), reinserisci.
- Inserimento in testa/coda su lista non vuota aggiorna correttamente i link.
- Cancellazione del primo, di un nodo in mezzo, dell’ultimo; cancellazione di valore assente.
- Inserimento dopo (o prima) un nodo di mezzo; verifica dei link prev/next.

## Pseudocodice sintetico
- push_front(x): crea nodo; node->next = head; node->prev = NULL; aggiorna head (e tail se era vuota); fissa prev del vecchio head.
- push_back(x): simile ma in coda; aggiorna tail e next/prev coerenti.
- find(x): itera da head; restituisce nodo o NULL.
- insert_after(node, x): crea nodo; collega tra node e node->next, aggiorna prev/next; se node è tail, aggiorna tail.
- delete_value(x): trova primo match; aggiorna i link dei vicini (prev->next, next->prev), aggiorna head/tail se serve; free del nodo.
- is_empty: head == NULL.

## Ordine dei micro-blocchi di codice
1) Definire struct Node { int value; Node *prev; Node *next; } e List { Node *head; Node *tail; }.
2) Implementare init/destroy (destroy itera e libera tutti i nodi).
3) Implementare is_empty.
4) Implementare push_front e push_back con gestione della lista vuota e aggiornamento prev/next.
5) Implementare find.
6) Implementare insert_after (facoltativo insert_before) per esercitare i link doppi.
7) Implementare delete_value (head, middle, tail, assente).
8) Main/driver con test per tutti gli edge case.

## Test minimi
- Lista vuota: is_empty true; find NULL; delete fallisce.
- Un elemento: push_front, verifica head/tail; delete lo rimuove; lista vuota.
- Inserimenti multipli front/back: controlla head/tail e coerenza dei link (head->prev NULL, tail->next NULL).
- Inserimento dopo un nodo di mezzo: controlla prev/next dei nodi toccati.
- Cancellazione di head, di un nodo interno, di tail; cancellazione di valore assente.

## Tracciamento prestazioni
- push_front/back O(1); insert_after O(1) dato il nodo; find/delete O(n) peggiore.
- Empirico: misura iterazioni o tempo su n = 10, 100, 1_000, 10_000 per find/delete, verifica crescita ~ lineare.

## Note per linguaggio (adatta)
- C: gestire malloc/free, aggiornare entrambi i puntatori; attenzione a non lasciare prev/next sporchi.
- C++: `new/delete` o smart pointer; logica identica.
- Python: implementazione didattica con oggetti; utile per capire i link doppi.
- Assembly: molto impegnativo; obiettivo minimo = capire i campi prev/next e iterare in avanti.

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

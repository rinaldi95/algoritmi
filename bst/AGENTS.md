# AGENTS.md – BST (Binary Search Tree)

## Mini-contratto (in parole semplici)
- Operazioni: inserisci un valore, cerca un valore, cancella un valore, visita in ordine (in-order) per ottenere i valori ordinati.
- Input: valori interi (inizio). Duplicati: decidi policy (rifiuta, conta, o manda a sinistra/destra in modo coerente).
- Output: insert indica successo/duplicato; search trova o no; delete rimuove se esiste.
- Edge case: albero vuoto, nodo foglia, nodo con un figlio, nodo con due figli, cancellazione della radice.
- Complessità attesa: O(h) per operazioni, dove h è l’altezza dell’albero (peggiore O(n), migliore O(log n)).
- Invarianti: per ogni nodo, tutto a sinistra < nodo < tutto a destra (o <=/>= se decidi gestione duplicati).

## Scopo
- Implementare un BST con insert/search/delete e traversali (in-order, pre-order, post-order opzionali).

## Input/Output
- Input: chiave (int). Duplicati gestiti con policy chiara.
- Output: `insert` ritorna 0/1 (nuovo/duplicato), `find` ritorna 1/0, `delete` ritorna 1/0. Traversali stampano o riempiono un array.

## Criteri di correttezza
- Proprietà BST mantenuta dopo ogni operazione.
- Insert posiziona il nuovo nodo nel posto corretto.
- Delete gestisce correttamente i 3 casi: foglia, un figlio, due figli (usa successore o predecessore).

## Edge case obbligatori
- Albero vuoto: search/delete falliscono senza crash.
- Inserimento primo nodo (radice).
- Cancellazione foglia.
- Cancellazione nodo con un figlio.
- Cancellazione nodo con due figli.
- Cancellazione della radice in tutti i casi sopra.
- Duplicati secondo la policy scelta.

## Pseudocodice sintetico
- Insert: scendi a sinistra/destra fino a trovare NULL, inserisci lì.
- Search: scendi a sinistra/destra confrontando con il valore cercato.
- Delete:
  - se nodo è foglia: rimuovi.
  - se ha un solo figlio: sostituisci il nodo con il figlio.
  - se ha due figli: trova il successore (minimo nel sottoalbero destro), copia valore, poi cancella il successore.
- In-order: visita sinistra, nodo, destra (produce ordine crescente).

## Ordine dei micro-blocchi di codice
1) Definire Node { key, left, right } e Tree { root }.
2) Implementare init/destroy (destroy ricorsivo).
3) Implementare search.
4) Implementare insert (con policy duplicati).
5) Implementare in-order traversal (stampa o fill array).
6) Implementare delete (tre casi + helper per successore).
7) Test su casi base e edge case.

## Test minimi
- Inserisci [5,3,7,2,4,6,8], verifica in-order -> 2,3,4,5,6,7,8.
- Search per presente/assente.
- Delete: foglia (2), nodo con un figlio, nodo con due figli (5), e radice.
- Inserisci duplicato secondo policy (testa aggiornamento o rifiuto).

## Tracciamento prestazioni
- O(h) per insert/search/delete; h dipende dalla forma dell’albero.
- Empirico: confronta input ordinato (peggiora verso lista) vs input random (più bilanciato).

## Note per linguaggio (adatta)
- C: malloc/free per ogni nodo; attenzione a leak; delete corretto per i tre casi.
- C++: usare `new/delete` o smart pointer; stessa logica.
- Python: implementazione didattica con classi.
- Assembly: avanzato; obiettivo minimo = capire la struttura dei nodi e la ricorsione/iterazione.

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

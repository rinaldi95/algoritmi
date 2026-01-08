# AGENTS.md – Hash Table (Chaining)

## Mini-contratto (in parole semplici)
- Operazioni: inserisci una coppia (chiave, valore), cerca per chiave, cancella per chiave.
- Input: chiavi intere (inizio), valori interi (inizio). Dimensione tabella fissa o ridimensionabile.
- Output: inserimento segnala successo/fallimento; ricerca restituisce valore o “non trovato”; cancellazione segnala se l’elemento esisteva.
- Collisioni: più chiavi nello stesso indice → si tengono in una lista in quel bucket.
- Edge case: chiave assente, chiave duplicata (decidi se aggiornare o rifiutare), tabella vuota, bucket con più elementi.
- Complessità attesa: O(1) medio, O(n) peggiore se molti finiscono nello stesso bucket.
- Invarianti: ogni elemento è in esattamente un bucket; le liste sono coerenti.

## Scopo
- Implementare una hash table con chaining (array di liste). Ogni bucket contiene una lista di nodi con chiave/valore.

## Input/Output
- Input: chiave (int), valore (int).
- Output: `insert` restituisce 0/1 (successo/aggiornamento/errore); `find` restituisce esito e valore via out param; `delete` restituisce 1 se rimosso, 0 se non trovato.

## Criteri di correttezza
- `find` restituisce il valore associato alla chiave se presente.
- `insert` aggiunge una nuova coppia o aggiorna la chiave esistente (decidi policy e documentala).
- `delete` rimuove la coppia corretta senza perdere altri nodi del bucket.

## Edge case obbligatori
- Tabella vuota: find/delete falliscono senza crash.
- Collisioni: più chiavi nello stesso bucket, tutte recuperabili.
- Duplicati: inserimento della stessa chiave (aggiorna valore o rifiuta, ma coerente).
- Cancellazione di head/middle/tail nella lista del bucket.
- Resize (se implementato): mantenere tutti gli elementi dopo il rehash.

## Pseudocodice sintetico
- Hash: `index = key % capacity` (per chiavi positive; gestisci negative se serve).
- Insert: calcola index; cerca nella lista; se presente aggiorna; altrimenti aggiungi nodo in testa.
- Find: calcola index; cerca nella lista; se presente restituisci valore.
- Delete: calcola index; rimuovi nodo dalla lista (prev/curr).
- Resize (opzionale ma consigliato): quando load factor supera soglia, raddoppia capacità e reinserisci tutti i nodi (rehash).

## Ordine dei micro-blocchi di codice
1) Definire struct Node { key, value, next } e struct HashTable { buckets, capacity, size }.
2) Implementare init/destroy (alloca array di bucket e libera tutte le liste).
3) Implementare hash function e index (gestisci chiavi negative se vuoi).
4) Implementare insert (con policy duplicati: update o no).
5) Implementare find (out param).
6) Implementare delete.
7) (Opzionale) Implementare resize/rehash con load factor.
8) Test: vuota, collisioni, duplicati, delete in bucket, resize.

## Test minimi
- Inserisci 3 chiavi che collidono (stesso bucket) e verifica find per tutte.
- Inserisci una chiave già presente: se policy “update”, il valore cambia; se “reject”, resta quello vecchio.
- Delete di una chiave in testa, in mezzo, in coda nella lista del bucket.
- Find di chiave assente.
- (Se resize) Inserisci molte chiavi, verifica che tutte si trovino dopo il rehash.

## Tracciamento prestazioni
- Tempo medio O(1), peggiore O(n) se tutti nello stesso bucket.
- Empirico: inserisci n=100,1000,10000 chiavi pseudo-random e misura tempo medio di find/insert.
- Confronta load factor (size/capacity) e lunghezze medie delle liste.

## Note per linguaggio (adatta)
- C: gestire malloc/free dei nodi e dell’array bucket; attenzione a leak in delete/destroy.
- C++: usare struct e new/delete o smart pointers.
- Python: dizionario è già hash table, ma qui si implementa a mano per capire.
- Assembly: molto complesso; obiettivo minimo = capire l’idea di bucket + lista.

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

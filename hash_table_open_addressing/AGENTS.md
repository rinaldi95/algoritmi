# AGENTS.md – Hash Table (Open Addressing)

## Mini-contratto (in parole semplici)
- Operazioni: inserisci (chiave, valore), cerca per chiave, cancella per chiave.
- Input: chiavi intere, valori interi.
- Output: insert segnala nuovo/aggiornato/errore; find restituisce valore o “non trovato”; delete segnala se ha rimosso.
- Collisioni: risolte cercando un altro slot libero (probing).
- Edge case: chiave assente, chiave duplicata (aggiorna valore), tabella piena, cancellazioni (slot “deleted”).
- Complessità attesa: O(1) medio, O(n) peggiore quando la tabella è piena.
- Invarianti: ogni chiave è in uno slot; slot può essere EMPTY, OCCUPIED, DELETED.

## Scopo
- Implementare una hash table con open addressing. Ogni bucket contiene al massimo un elemento; in caso di collisione si usa probing.

## Input/Output
- Input: chiave (int), valore (int).
- Output: `insert` ritorna 0 (nuovo), 1 (update), -1 (errore). `find` ritorna 1/0, con out_value. `remove` ritorna 1/0.

## Criteri di correttezza
- `find` trova la chiave se presente, anche se ci sono slot DELETED in mezzo.
- `insert` aggiorna se la chiave esiste; altrimenti usa il primo slot EMPTY o DELETED.
- `remove` marca lo slot come DELETED (non può diventare EMPTY se la catena di probing continua).

## Edge case obbligatori
- Tabella vuota: find/remove falliscono senza crash.
- Collisioni: più chiavi finiscono nello stesso indice, probing trova posizioni diverse.
- Duplicati: insert sulla stessa chiave aggiorna il valore.
- Tabella piena: insert deve fallire o forzare resize.
- Slot DELETED: find deve continuare, insert può riusare.

## Pseudocodice sintetico
- Hash: `index = key % capacity` (gestisci negative).
- Probing (lineare): `i = (index + step) % capacity`, step = 0..capacity-1.
- Insert: scansiona con probing; se trova key -> update; altrimenti inserisce nel primo EMPTY/DELETED.
- Find: scansiona finché trova key o uno slot EMPTY (che interrompe la ricerca).
- Remove: scansiona; se trova key marca DELETED.
- Resize: quando load factor supera soglia, raddoppia e reinserisci (rehash) solo gli slot OCCUPIED.

## Ordine dei micro-blocchi di codice
1) Definire struct Entry { key, value, state } con state = EMPTY/OCCUPIED/DELETED; HashTable con array, capacity, size.
2) Implementare init/destroy.
3) Implementare hash e probing (lineare per iniziare).
4) Implementare insert con update e riuso DELETED.
5) Implementare find (deve ignorare DELETED e fermarsi su EMPTY).
6) Implementare remove (marca DELETED).
7) Implementare resize/rehash.
8) Test: collisioni, duplicati, deleted slot, resize.

## Test minimi
- Inserisci chiavi che collidono e verifica find per tutte.
- Inserisci la stessa chiave: valore aggiornato.
- Remove di una chiave e verifica che find fallisca; inserisci nuova chiave che riusa lo slot DELETED.
- Fill fino a triggerare resize; verifica che tutte le chiavi siano ancora trovabili.
- Ricerca di chiave assente che attraversa slot DELETED.

## Tracciamento prestazioni
- O(1) medio, O(n) peggiore quando load factor alto.
- Empirico: n = 100, 1_000, 10_000; misura tempo medio di find/insert e numero di probe.
- Confronta prestazioni a load factor diversi (0.5, 0.7, 0.9).

## Note per linguaggio (adatta)
- C: usare enum per lo state; attenzione ai probe e al resize; gestire negative keys.
- C++: stessa logica, ma con vector e enum class.
- Python: implementazione didattica con liste.
- Assembly: molto complesso; obiettivo minimo = capire probing e stati degli slot.

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

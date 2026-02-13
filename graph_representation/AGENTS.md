# AGENTS.md – Graph Representation (Core condiviso Fase 5)

## Missione della cartella
- Questa cartella contiene solo il **core di rappresentazione** del grafo.
- Qui si implementano **liste di adiacenza** e **matrice di adiacenza** con API coerente.
- Questo core verrà riusato dai progetti in `../graph_algorithms/`.
- In questa cartella **non** si implementano BFS, DFS, Dijkstra, Bellman-Ford, Prim, Kruskal o Floyd-Warshall.

## Piano di lavoro (modello ibrido)
- Non fare un programma monolitico unico per tutta la Fase 5.
- Non rifare da zero la rappresentazione per ogni algoritmo.
- Fare un core condiviso qui, poi moduli separati per algoritmo in `graph_algorithms`.

### Ripartizione delle responsabilita
- `graph_representation/`: struttura dati, validazioni input, add/remove edge, adiacenza, vicini, test del core.
- `graph_algorithms/`: un modulo per algoritmo (BFS, DFS, componenti, topo, shortest path, MST) con test dedicati.

## Mini-contratto (in parole semplici)
- Operazioni: creare un grafo, distruggere un grafo, aggiungere/rimuovere un arco, verificare adiacenza, ottenere i vicini.
- Input: numero di nodi `n`, coppie `(u,v)` con `0 <= u,v < n`, grafo inizialmente non pesato.
- Output: esito operazioni con codici chiari; `is_adjacent(u,v)` vero/falso; `neighbors(u)` restituisce tutti i vicini.
- Edge case: grafo vuoto, nodo fuori range, arco duplicato, self-loop.
- Complessita attesa: lista O(grado) per ricerca adiacenza; matrice O(1) per ricerca adiacenza.
- Invarianti: se grafo non diretto e c'e `(u,v)`, deve esserci anche `(v,u)`; nessuna struttura corrotta dopo errori.

## Decisioni da congelare prima del codice
1. Grafo diretto o non diretto (default consigliato: non diretto).
2. Self-loop consentito o vietato.
3. Gestione duplicati: ignorare o segnalare errore.
4. Convenzione return code uniforme per tutte le API.

## Input/Output formale
- Input: `n` (intero `>= 0`) e archi `(u,v)` entro range.
- Output:
  - `add/remove`: successo/fallimento con codice.
  - `is_adjacent(u,v)`: 1/0.
  - `neighbors(u)`: collezione dei vicini validi di `u`.

## Criteri di correttezza
- Le strutture rappresentano esattamente gli archi presenti.
- `add_edge` inserisce l'arco rispettando policy su duplicati e direzione.
- `remove_edge` rimuove solo l'arco richiesto e lascia invariato il resto.
- `neighbors(u)` restituisce tutti e soli i vicini di `u`.

## Edge case obbligatori
- `n = 0`, `n = 1`.
- Nodi fuori range.
- Arco duplicato.
- Rimozione di arco inesistente.
- Self-loop.

## Pseudocodice sintetico
- Lista di adiacenza: array di liste; `add_edge(u,v)` inserisce `v` nei vicini di `u` (e simmetrico se non diretto).
- Matrice di adiacenza: matrice `n x n` booleana; `add_edge(u,v)` imposta cella/e a 1.
- `is_adjacent(u,v)`: lookup diretto in matrice o ricerca nei vicini in lista.

## Ordine dei micro-blocchi
1. Definire strutture dati e convenzione codici di ritorno.
2. Implementare `init/destroy` per lista e matrice.
3. Implementare `add_edge/remove_edge` con validazioni.
4. Implementare `is_adjacent`.
5. Implementare `neighbors(u)`.
6. Test edge case su entrambe le rappresentazioni.
7. Verifica coerenza tra backend (stessi risultati a parita di operazioni).

## Test minimi del core
- `n=5`, add `(0,1)`, `(0,2)`, `(3,4)`, verifica adiacenze.
- remove di un arco presente e verifica assenza.
- duplicato su arco esistente, verifica policy scelta.
- remove di arco non presente, nessuna corruzione.
- test out-of-range.
- se non diretto: simmetria `(u,v)` e `(v,u)`.

## Tracciamento prestazioni (solo core)
- Lista: `is_adjacent` O(grado), `neighbors` O(grado), spazio O(n+m).
- Matrice: `is_adjacent` O(1), spazio O(n^2).
- Confronto concettuale: grafi sparsi vs densi.

## Quando aprire una nuova variante separata
- Passaggio a grafo pesato (struttura arco cambia).
- Cambio netto di invarianti I/O (es. all-pairs con matrice distanze dedicata).
- Implementazione autonoma in linguaggio diverso per esercizio didattico.

## Regole di interazione (sempre)
- Pochissimo codice per volta.
- Spiegazione chiara di ogni mini-blocco.
- Conferma esplicita utente prima del blocco successivo.
- Nessuna modifica file automatica senza richiesta esplicita.

## Tracking locale
- Data inizio:
- Stato: specifica congelata / core lista avviato / core matrice avviato / test core ok / pronto per `graph_algorithms`
- Linguaggi coperti:
- Problemi aperti / note:

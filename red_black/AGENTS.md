AGENTS.md — Red–Black Tree (istanza locale)
==========================================

Mini-contratto (in parole semplici)
----------------------------------
Operazioni: inserisci, cerca, cancella in un albero di ricerca bilanciato (auto-bilanciamento con colori).
Input attesi: chiavi comparabili (inizialmente interi), eventuali duplicati da gestire con una politica esplicita.
Output: successo/fallimento delle operazioni; in caso di ricerca, posizione/nodo o “non trovato”.
Casi limite: albero vuoto, inserimento del primo nodo (radice), cancellazione della radice, duplicati, catene sbilanciate.
Complessità attesa: O(log n) per search/insert/delete.
Invarianti: radice nera; nessun nodo rosso con figlio rosso; ogni cammino radice‑foglia ha lo stesso numero di nodi neri.

Scopo dell’algoritmo
--------------------
Implementare un albero Red–Black che mantenga il bilanciamento tramite regole di colore e rotazioni, garantendo operazioni logaritmiche. L’obiettivo didattico è capire:
- come le regole di colore forzano un’altezza O(log n),
- come le rotazioni e i ricolori riparano le violazioni dopo insert/delete,
- differenze concettuali con AVL (meno rigido, meno rotazioni in media).

Definizione formale del problema
--------------------------------
Input: sequenze di operazioni (insert, search, delete) su chiavi confrontabili.
Output: per ogni operazione, stato corretto dell’albero RB, mantenendo le invarianti RB.
Criterio di correttezza: dopo ogni operazione, l’albero è un BST valido e soddisfa tutte le proprietà Red–Black.

Edge case obbligatori
---------------------
- Inserimento in albero vuoto.
- Inserimento che crea nodo rosso con padre rosso.
- Inserimento con zio rosso (solo ricolorazioni).
- Inserimento con zio nero (rotazioni singola o doppia).
- Cancellazione di nodo rosso (nessun fix‑up).
- Cancellazione di nodo nero con figlio rosso (color flip semplice).
- Cancellazione di nodo nero con figlio nero / NIL (fix‑up completo).
- Cancellazione della radice.
- Duplicati: comportamento esplicito (rifiuta / conta / sostituisci).

Pseudocodice sintetico (passi logici)
-------------------------------------
1) Insert BST standard.
2) Colora il nuovo nodo rosso.
3) Ripara violazioni (caso padre rosso):
   - zio rosso → ricolorazioni e risali.
   - zio nero → rotazioni + ricolorazione.
4) Assicura radice nera.

1) Delete BST standard (sostituzione con successore se necessario).
2) Se rimuovi un nero → avvia fix‑up:
   - fratello rosso → rotazione + ricolorazione.
   - fratello nero con figli neri → ricolorazione e risali.
   - fratello nero con figlio rosso “esterno” → rotazione + ricolorazione.
3) Assicura radice nera.

Ordine dei micro‑blocchi di codice (da proporre all’utente)
-----------------------------------------------------------
1) Definizione nodo (chiave, colore, parent, left, right, NIL se usato).
2) Funzioni di utilità minime (color, parent getter, NIL checks).
3) Rotazione sinistra.
4) Rotazione destra.
5) Insert BST (solo aggancio nodo).
6) Fix‑up per insert (casi).  
7) Search BST.
8) Transplant / sostituzione per delete.
9) Fix‑up per delete (casi).
10) Verifiche di invarianti (anche solo assert/debug).

Test minimi suggeriti
---------------------
- Insert: 10, 5, 15, 1, 7, 12, 18 (verifica proprietà RB).
- Insert con zio rosso (esempio classico: 7, 3, 18, 10, 22, 8, 11, 26).
- Insert con zio nero e rotazione (es. 10, 5, 1).
- Delete foglia rossa.
- Delete nodo nero con un figlio rosso.
- Delete nodo nero con NIL come figli (fix‑up completo).
- Ricerca di elemento presente e assente.

Note specifiche per linguaggio (da adattare nella sottocartella)
---------------------------------------------------------------
- C: gestire sentinella NIL esplicita; attenzione a malloc/free; evitare memory leak.
- C++: preferire std::unique_ptr o gestione chiara degli ownership; evitare raw pointer quando non serve.
- Python: semplicità e leggibilità; test con assert; oggetti Node leggeri.
- Assembly: concentrarsi sul flusso di controllo e sulle rotazioni; commentare ogni istruzione.

Tracciamento prestazioni
------------------------
Misura: numero di confronti o altezza dell’albero dopo sequenze di insert/delete; tempo solo come indicatore grossolano.
Input crescenti: n = 1, 10, 100, 1_000, 10_000 con sequenze casuali e già ordinate.
Atteso: altezza O(log n) e operazioni logaritmiche.

Regole di interazione per l’istanza locale
------------------------------------------
- Pochissimo codice alla volta: micro‑blocchi piccoli e compilabili.
- Dopo ogni blocco, chiedere conferma esplicita che l’utente ha scritto e capito.
- Nessun copia‑incolla: guidare in micro‑step.
- Spiegazioni dettagliate di cosa fa il blocco e perché.
- Nessuna scrittura o modifica file non richiesta dall’utente.
- Focus: solo Red–Black Tree.

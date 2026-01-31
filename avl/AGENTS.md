# AGENTS.md – AVL Tree

## Mini-contratto (in parole semplici)
- Operazioni: inserisci un valore, cerca un valore, cancella un valore, stampa in-order.
- Input: valori interi. Duplicati: scegli policy (di solito rifiutati o aggiornati).
- Output: insert/search/delete indicano successo/fallimento.
- Edge case: albero vuoto, nodo foglia, nodo con un figlio, nodo con due figli, cancellazione della radice.
- Complessità attesa: O(log n) per insert/search/delete grazie al bilanciamento.
- Invarianti: proprietà BST + fattore di bilanciamento in ogni nodo (altezza sinistra - destra) in {-1,0,1}.

## Scopo
- Implementare un BST auto‑bilanciato (AVL). Dopo ogni insert/delete, l’albero può ruotare per rimanere bilanciato.

## Input/Output
- Input: chiave (int). Duplicati gestiti con policy chiara.
- Output: `insert` ritorna 0/1, `find` ritorna 1/0, `delete` ritorna 1/0.

## Criteri di correttezza
- Proprietà BST preservata.
- Per ogni nodo: |height(left) - height(right)| <= 1.
- Rotazioni corrette (LL, RR, LR, RL) applicate quando necessario.

## Edge case obbligatori
- Inserimenti che causano ciascuna delle 4 rotazioni.
- Cancellazione che causa ribilanciamento.
- Albero vuoto e singolo nodo.
- Duplicati secondo la policy scelta.

## Pseudocodice sintetico
- Insert: come BST; dopo l’inserimento aggiorna altezza e calcola balance; se fuori range, ruota.
- Delete: come BST; dopo la cancellazione aggiorna altezza e bilancia lungo il percorso di ritorno.
- Rotazioni: left_rotate, right_rotate, e combinazioni (LR, RL).

## Ordine dei micro-blocchi di codice
1) Definire Node { key, height, left, right } e Tree { root }.
2) Implementare height(node) e update_height(node).
3) Implementare rotazioni (left/right) e funzioni di bilanciamento.
4) Implementare insert con ribilanciamento.
5) Implementare delete con ribilanciamento.
6) Implementare find e in-order.
7) Test con sequenze che forzano LL/RR/LR/RL.

## Test minimi
- Inserimento: [30,20,10] -> rotazione LL.
- Inserimento: [10,20,30] -> rotazione RR.
- Inserimento: [30,10,20] -> rotazione LR.
- Inserimento: [10,30,20] -> rotazione RL.
- Cancellazioni che richiedono ribilanciamento.
- In-order produce sequenza ordinata.

## Tracciamento prestazioni
- O(log n) per insert/search/delete; confronto con BST su input ordinato.

## Note per linguaggio (adatta)
- C: attenzione a aggiornare height e a gestire i puntatori dopo le rotazioni.
- C++: idem, con new/delete o smart pointer.
- Python: implementazione didattica con classi.
- Assembly: molto avanzato; obiettivo minimo = capire rotazioni e altezze.

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

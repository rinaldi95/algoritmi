# AGENTS.md – Template algoritmo <NOME>

## Scopo
- Breve descrizione dell’algoritmo e del problema che risolve.

## Input/Output
- Input: tipo, formato, precondizioni.
- Output: formato, modalità di segnalazione casi speciali (es. "non trovato").

## Criteri di correttezza
- Cosa deve essere vero per considerare l’output corretto.

## Edge case obbligatori
- Elenco puntato (array vuoto/1 elemento, duplicati, input ordinato/inverso, grafi con cicli, pesi negativi se rilevante, ecc.).

## Pseudocodice sintetico
- 4-8 passi logici, niente muro di codice.

## Ordine dei micro-blocchi di codice
1) …
2) …
3) …

## Test minimi
- Casi base:
- Casi degenerati:
- Casi tipici:

## Note per linguaggio (adatta)
- C: buffer, alloc/dealloc, piccoli passi compilabili.
- C++: usa `std::vector`/RAII, evita raw pointers inutili.
- Python: leggibilità, `if __name__ == "__main__":`, assert/unittest leggeri.
- Assembly: micro-step, commenta registri e flusso, testa input semplici.

## Regole di interazione (da seguire sempre)
- Pochissimo codice per volta; spiegazione di ogni blocco.
- Nessun copia-incolla; l’utente scrive a mano.
- Conferma esplicita prima di passare al blocco successivo.
- Non creare/modificare file automaticamente; proporre soltanto.

## Tracking locale
- Data inizio:
- Stato: spiegato / codifica avviata / test minimi ok
- Linguaggi coperti:
- Problemi aperti / note:

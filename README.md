# chess_cpp

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Language: C++](https://img.shields.io/badge/Language-C%2B%2B11-blue.svg)
![Platform: Windows](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

Gioco di scacchi per terminale a due giocatori, sviluppato in C++ con architettura orientata agli oggetti.

---

## Demo

```
CHESS GAME         (by Alessio Paccino)

     A   B   C   D   E   F   G   H          Turno del BLU
 1   T   C   A   Q   K   A   C   T
 2   P   P   P   '   P   '   P   P
 3   '   '   '   P   '   P   '   '
 4   '   '   '   '   '   '   '   '
 5   '   '   '   '   '   '   '   '
 6   '   '   '   p   '   p   '   '
 7   p   p   p   '   p   '   p   p
 8   t   c   a   q   k   a   c   t

Inserire la casella iniziale e finale del pezzo
> e2 e4
```

Pezzi **BLU** (maiuscole) e pezzi **ROSSO** (minuscole) sono visualizzati con colori ANSI distinti nel terminale.

---

## Funzionalità

- Scacchiera 8×8 con disposizione iniziale standard
- Movimenti validi per tutti e sei i tipi di pezzo (torre, cavallo, alfiere, regina, re, pedone)
- Gestione turni alternati (BLU inizia per primo)
- Controllo anti-teamkill: impossibile catturare pezzi alleati
- Prima mossa del pedone: salto doppio
- Cattura in diagonale del pedone
- Promozione del pedone a torre, regina, cavallo o alfiere
- Rilevamento scacco tramite analisi delle traiettorie (`is_checked`)
- Logging degli errori su file (`scacchiera_log.txt`)
- Input in notazione algebrica semplificata (es. `a1 b3`)

---

## Architettura e scelte progettuali

### Gerarchia OOP

```
pieces  (classe base astratta)
├── tower   (torre)
├── queen   (regina)
├── bishop  (alfiere)
├── knight  (cavallo)
├── pawn    (pedone)
└── king    (re)
```

La classe base `pieces` dichiara `valid_movement_controller` come metodo virtuale. Ogni sottoclasse sovrascrive questo metodo incapsulando la propria logica di movimento, senza che `Scacchiera` debba distinguere i tipi a runtime per validare le mosse.

### Matrice di puntatori nullable

```cpp
pieces *scacchiera[8][8];
```

L'uso di puntatori a `pieces` permette di rappresentare una casella vuota con `nullptr`, senza enum aggiuntivi o flag booleani paralleli. Il tipo del pezzo rimane incapsulato nell'oggetto stesso.

### `controls()` centralizzato nella classe base

Il metodo `controls()` in `pieces` gestisce il controllo anti-teamkill per tutti i pezzi. Ogni `valid_movement_controller` lo chiama prima di validare la traiettoria specifica: evita duplicazione di logica identica in sei classi diverse.

### `dynamic_cast` per `is_checked`

Il metodo `is_checked` analizza le minacce al re ed è specifico della classe `king`. Il metodo `king_finder()` in `Scacchiera` usa `dynamic_cast<king*>` per accedere a questo metodo senza esporre `is_checked` nella classe base, mantenendo l'interfaccia di `pieces` minimale.

### Colori ANSI senza dipendenze esterne

I colori del terminale sono gestiti con sequenze di escape ANSI definite come macro:

```cpp
#define BLUE_CONSOLE  "\033[34m"
#define RED_CONSOLE   "\033[31m"
```

Nessuna libreria grafica esterna: il gioco funziona su qualsiasi terminale compatibile ANSI senza dipendenze aggiuntive.

---

## Struttura del progetto

| File | Responsabilità |
|------|---------------|
| `main.cpp` | Classe `Scacchiera`, game loop, validazione input, rendering |
| `pieces.h` | Classe base `pieces` e tutte le sottoclassi dei pezzi |
| `functions_for_logs.cpp` | `error_log()` (scrittura su file), `console_cleaner()` (clear cross-platform) |
| `functions_for_logs.h` | Dichiarazioni delle funzioni di utilità |
| `macro.h` | Include guard e forward declaration |
| `scacchiera_log.txt` | Log degli errori generato a runtime |

---

## Compilazione e avvio

### Prerequisiti

- Compilatore C++11 o superiore (g++ o MSVC)
- Terminale con supporto ANSI (Windows Terminal, PowerShell, bash)

### Compilazione

```bash
g++ -g main.cpp functions_for_logs.cpp -o chess
```

### Avvio

```bash
./chess        # Linux / macOS
chess.exe      # Windows
```

> **Nota:** Il logging su file (`scacchiera_log.txt`) usa `fopen_s`, disponibile nativamente su Windows. Su Linux la funzione viene comunque compilata ma potrebbe richiedere un compilatore aggiornato.

---

## Come giocare

1. Il giocatore **BLU** (pezzi maiuscoli) muove per primo
2. Inserire la casella di partenza e quella di arrivo in notazione algebrica:
   ```
   > a1 a3
   ```
   - La lettera indica la colonna (`a`–`h`)
   - Il numero indica la riga (`1`–`8`)
3. Il programma valida la mossa e passa il turno al giocatore **ROSSO** (pezzi minuscoli)
4. Il gioco termina quando un re viene catturato

### Legenda pezzi

| Simbolo | Pezzo |
|---------|-------|
| `T` / `t` | Torre |
| `C` / `c` | Cavallo |
| `A` / `a` | Alfiere |
| `Q` / `q` | Regina |
| `K` / `k` | Re |
| `P` / `p` | Pedone |
| `'` | Casella vuota |

---

## Roadmap

- [ ] Rilevamento scacco matto completo (attualmente il gioco termina alla cattura del re)
- [ ] Arrocco
- [ ] En passant
- [ ] Salvataggio risultato partita su file JSON
- [ ] Compatibilità Linux completa

---

## Licenza

Distribuito sotto licenza MIT. Vedi [`LICENSE`](LICENSE) per i dettagli.

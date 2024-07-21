# Projektbericht (Team 150): Simulation, Analyse und Optimierung der Mehrstufigen-Cache
## Hintergrund
Caches sind eine Strategie zur Minderung des Von-Neumann-Flaschenhalses.
Caches sind kleine, aber schnelle Speicher, die Daten und Anweisungen vorübergehend speichern, um die Zugriffszeit auf häufig verwendete Daten zu reduzieren. In modernen Prozessoren sind L1- (Level 1) und L2- (Level 2) Caches von entscheidender Bedeutung. Der L1-Cache ist der kleinste und schnellste Speicher, während der L2-Cache größer, aber etwas langsamer ist.
## Methodik
### Simulierte Cache-Architektur:
- Erstellung eines Simulationsmodells der L1- und L2-Cache-Struktur mit C, C++, SystemC.
- Leistungstests und Analyse der Anfangszustände (ohne Optimierung).
- Implementierung einer Befehlszeilenschnittstelle zur Eingabe von Simulationsparametern wie der Cache-Zeilengröße.
### Implementierung von Optimierungsstrategien:
- Store-Back-Buffer: Einführung eines Puffers, der Schreiboperationen zwischenspeichert, um den Speicherbus zu entlasten.
- Prefetch-Buffer: Vorabrufen häufig benötigter Daten mit einer benutzerdefinierten Puffergröße.
### Analysierte Algorithmen
17 Varianten von Matrixmultiplikationen werden nach ihrer Zyklenanzahl und Trefferquote analysiert. Zwei 16 x 16 float-Matrizen A und B werden zur Ergebnismatrix C multipliziert (`A[i][k]` x  `B[k][j]` =  `C[i][j]`). Je nach Zugriffsmuster kommt es zu verschiedenen Lokalitäten. Insgesamt gibt es 6 Zugriffsmuster, von außen nach innen in der Schleife: `ijk`, `ikj`, `jik`, `jki`, `kij`, `kji`. Jede Anordnung hat 2 mögliche Optimierungen außer `ikj`, welche nur 1 hat.
### Analyse und Bewertung:
Analyse der Entwicklung von Zyklenanzahl, Hit-Rate, usw. der Matrixmultiplikationen nach Cachezeilengröße, L1- und L2- Zeilenanzahl, mit oder ohne Optimierungen. Es wird standardmäßig zur Analyse eine Cache mit 4 Zyklen für L1-Latenzzeit, 12 Zyklen für L2-Latenzzeit, und 100 Zyklen für Memory benutzt. Als Basis hat L1 4 Zeilen, L2 hat 16 Zeilen, und die Cachezeilengröße beträgt 16 Bytes.
## Ergebnisse
### Matrix
Zwischen der Basisvarianten ist `ikj` am schnellsten, während insgesamt ist `ijk_opt2` am schnellsten. Bei einer exponentiell ansteigender Cachezeilengröße senkt die Zyklenanzahl alle Matrixmultiplikationen außer `kji_opt1` und `jki_opt1`.
Bei steigender L1 Zeilenanzahl ergibt sich kaum Verbesserung an Zyklenanzahl, während bei steigender L2 Zeilenanzahl nur kij und ikj erkennbare Senkung haben.
### Auswirkungen der Optimierungsstrategien:
Store-Back-Buffer: Verringerte durchschnittliche Zyklenanzahl um 161.679 Zyklen mit einer Puffergröße von 1 Wort.
Prefetch-Buffer: Erhöhte Trefferquoten im L2-Cache von 23,8 % auf 40,5 % durch Vorabrufen häufig benötigter Daten mit einer Puffergröße von 4 Cache-Zeilen.
## Mitwirkende
### Alexander Anthony Tang
war für src/modules verantwortlich, mit zusätzlichen Beiträgen zur src/main/simulator.cpp.
### Lie Leon Alexius
war für examples, src/main und src/assets verantwortlich, mit zusätzlichen Beiträgen zur src/modules.
### Trang Nguyen
war für src/modules verantwortlich.

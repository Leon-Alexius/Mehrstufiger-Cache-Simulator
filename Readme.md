# Projektbericht: Simulation, Analyse und Optimierung des Speicherhierarchiesystems

## Einführung
Dieser Bericht dokumentiert die Ergebnisse der Projektgruppe 150 zur Simulation, Analyse und Optimierung der L1- und L2-Cache-Speicherhierarchiesysteme in modernen Computersystemen.

## Hintergrund
Caches sind eine Strategie zur Minderung des Von-Neumann-Flaschenhalses.

Caches sind kleine, aber schnelle Speicher, die Daten und Anweisungen vorübergehend speichern, um die Zugriffszeit auf häufig verwendete Daten zu reduzieren. In modernen Prozessoren sind L1- (Level 1) und L2- (Level 2) Caches von entscheidender Bedeutung. Der L1-Cache ist der kleinste und schnellste Speicher, während der L2-Cache größer, aber etwas langsamer ist.

## Methodik
### Simulation der aktuellen Cache-Architektur:
- Erstellung eines Simulationsmodells der L1- und L2-Cache-Struktur.
- Leistungstests und Analyse der Anfangszustände (ohne Optimierung).
- Implementierung einer Befehlszeilenschnittstelle zur Eingabe von Simulationsparametern wie der Cache-Zeilengröße.

### Implementierung von Optimierungsstrategien:
- **Store-Back-Buffer**: Einführung eines Puffers, der Schreiboperationen zwischenspeichert, um den Speicherbus zu entlasten.
- **Prefetch-Buffer**: Vorabrufen häufig benötigter Daten mit einer benutzerdefinierten Puffergröße.

### Analysierte Algorithmen
- 17 Varianten von Matrixmultiplikationen werden analysiert, und zwar nach ihre Lokalität. Gegeben sind zwei 16 x 16 `float`-Matrizen A und B und sie werden miteinander zu einer Ergebnismatrix C multipliziert. Der Zugriff auf einzelnen Elementen wird durch die Indizierung `A[i][j]`, `B[j][k]`, und `C[i][k]` ausgeführt. Je nach welcher Index am weitesten draußen oder drinnen in der Schleife angeordnet wird, kann es zu verschiedene Lokalitäten kommen. Insgesamt gibt es 6 Anordnungen, nämlich von außen zu innen: `ijk`. `ikj`, `jik`, `jki`, `kij`, `kji`. Jede Anordnung hat 2 mögliche Optimierungen außer `jik`, welches nur 1 hat.

### Analyse und Bewertung:
- Analyse der Entwicklung von Zyklenanzahl, Hit-Rate, usw. der Matrixmultiplikaltionen nach Cachezeilengröße, L1- und L2- Zeilenanzahl, und verschiedene Latenzzeiten.  Es wird standardmäßig zur Analyse eine Cache mit 4 Zyklen für L1-Latenzzeit, 12 Zyklen für L2-Latenzzeit, und 100 Zyklen für Memory benutzt.
- Vergleich der Leistung vor und nach der Implementierung der Optimierungen.
- Messung der Zugriffszeiten, Trefferquoten und der Gesamtleistung des Systems.

## Ergebnisse
### Simulation der Speicherhierarchiesystem-Architektur:
- Engpässe in der Simulation werden hauptsächlich durch das Warten auf das Abrufen von Daten aus dem Speicher verursacht.

### Matrix
- Bei allen Fällen ist `jik_opt2` am schnellsten zu erledigen. Bei einer exponentiell ansteigender Cachezeilengröße senkt die Zyklenanzahl alle Matrixmultiplikationen außer `ijk_opt1`, `ijk`, `kji`, `jki`, `kji_opt1`, `jki_opt1`.
- Bei steigender L1 Zeilenanzahl ergibt sich kaum Verbesserung an Zyklenanzahl, während bei steigender L2 Zeilenanzahl nur `kij` und `ikj` erkennbare Senkung haben. Interessanterweise hat `jki_opt1` gar keine Treffer in L1 und `kji_opt1` auch keine in L2.

### Implementierung und Auswirkungen der Optimierungsstrategien:
- **Store-Back-Buffer**: Verringerte durchschnittliche Schreibzugriffszeiten um ??
- **Prefetch-Buffer**: Erhöhte Trefferquoten im L2-Cache bei unserem Matrixmultiplikationsalgorithmus von 23,8 % auf 40,5 % durch Vorabrufen häufig benötigter Daten mit einer Puffergröße von 4 Cache-Zeilen.

### Analyse und Bewertung:
- Die optimierten Caches mit Store-Back-Buffer zeigten eine verbesserte Effizienz in Bezug auf Zyklen.
- Die optimierten Caches mit Prefetch-Buffer zeigten eine signifikant verbesserte Effizienz in Bezug auf die Trefferquoten im L2-Cache.

## Diskussion
Die Implementierung des Store-Back-Buffers und des Prefetch-Buffers zeigte das Potenzial zur Verbesserung der Cache-Leistung, insbesondere die Reduzierung der Schreibzugriffszeiten durch den Store-Back-Buffer und die Erhöhung der Trefferquoten durch das Vorabrufen von Daten.

## Mitwirkende
### Alexander Anthony Tang
Anthony war hauptsächlich für die Entwicklung von src/modules verantwortlich, mit zusätzlichen Beiträgen zur Entwicklung von src/main/simulator.cpp.

Weitere Beiträge umfassen:
- Schreiben des Makefile
- Schreiben des build.sh-Skripts
- Initiierung, Optimierung und Dokumentation des Projekts

### Lie Leon Alexius
Leon war hauptsächlich für die Entwicklung von examples, src/main und src/assets verantwortlich, mit zusätzlichen Beiträgen zur Entwicklung von src/modules.

Weitere Beiträge umfassen:
- Schreiben des Makefile
- Schreiben der build.sh-Skripte
- Verfassen der Readme.md
- Implementierung automatisierter Tests
- Initiierung, Optimierung, Strukturierung und Dokumentation des Projekts

### Trang Nguyen
Trang war hauptsächlich für die Entwicklung von src/modules verantwortlich.

Weitere Beiträge umfassen:
- Initiierung, Optimierung und Dokumentation des Projekts


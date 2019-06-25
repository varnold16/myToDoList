# Beschreibung

Mit dem Programm kann eine Tabelle verwaltet werden, die eine ToDo-Liste darstellen soll.

Man kann neue Listen (= Tabelle) erstellen oder vorhandene laden.
Speichern und zurücksetzen der aktuellen Liste sind möglich.
Der Dateipfad zur aktuellen Liste oder ein Default-Pfad wird eingeblendet.

Einträge können bearbeitet, gelöscht, sortiert hinzugefügt und auch durchsucht werden.
Wenn gewünscht können über das Menü auch alle Eintrage auf einmal gelöscht werden.

Eine Beispiel-ToDo-Liste liegt bei (Beispiel.txt).

==========================================================================================

# Details zur Funktionalität

## Menübalken/Aktionen

* Neue Liste
  - leere Tabelle (löscht alle Reihen)
  - Default-Dateipfad/-name


* Öffne Liste...
  - Box zum öffnen von *.txt-Dateien
  - prüft, ob gewählte Datei richtigen Header beinhaltet (sonst Fehlermeldung)
  - liest Tabelle ein, Dateipfad/-name wird eingeblendet (Bestätitigt erfolgreiches Laden)

* Änderungen verwerfen
  - Aktuelle Liste (über Dateipfad/-name) wir nochmals eingelesen
    -> dasselbe wie bei "Öffne Liste...", nur dass die Box zur Dateiauswahl fehlt.

* Alle Aufgaben löschen
  - leert die Tabelle
  - Dateipfad/-name bleibt bestehen

==========================================================================================

## Buttons

* Bearbeiten...
  - übernimmt Felder aus Tabelle für Dialogbox
  - Vorgang kann bestätigt/abgebrochen werden
  - Fehlermeldungen:
    ~ keine Liste geladen
    ~ keine Auswahl getroffen
    ~ mehr als eine Aufgabe gewählt (wenn mehr als 5 Items ausgewählt sind)

* Löschen
  - gewählte Aufgabenauswahl wird gelöscht
    ~ Mehrauswahl gestattet

* sortierbar
  - Checkbox ob Spaltenweises sortieren erlaubt ist

* Hinzufügen...
  - Dialogbox öffnet sich
  - aktuelles Datum als Default, mit Kalenderfunktion
  - Priorität auf 1 als Default, mit Spinbox
  - Vorgang kann bestätigt/abgebrochen werden
  - leere Eingaben erlaubt

* Suchen
  - sucht Texteingabe in Tabelle nach Position (ignoriert Groß-/Kleinschreibung)
  - wenn Ende der Tabelle erreicht ist: Infobox und Position auf erste Zelle

* Speichern
  - Dateipfad/-name übernommen, muss auf *.txt enden (sonst Warnung)
  - wenn Datei schon existiert, wird man gefragt, ob man überschreiben will (abbrechen/bestätigen)
  - Datei enthält
    ~ Header ("# Datei erstellt für myToDoList.pro\tdd.MM.yyyy\n\n" mit aktuellem Datum)
    ~ Spaltentitel ("# Projekt\tFälligkeit\t\tAufgabe\tSiehe\n"
    ~ Felder als tab-getrennter Text 
  - erfolgreiches Speichern wird bestätitigt



# VAA (Verteilte Algorithmen und Anwendungen)
Dieses Git-Repository stellt die Lösungen zur Vorlesung Verteilte Algorithmen und Anwendungen zum Wintersemester 2019/20 an der Hochschule für Technik und Wirtschaft bereit.

## 1.Aufgabe
Die Aufgabe besteht darin ein Programm zu erstellen, dass  mehrere Prozesse anhand einer Datei initialisieren soll. Diese sollen eigenständig, also nicht als Threads ablaufen.
Die dazu einzulesende Datei sieht wi folgt aus:  
  *1 isl-s-01:5000*  
  *2 isl-s-01:5001*  
  *3 isl-s-01:5002*  
  *4 127.0.0.1:8888*  
  
 Hierbei wird jedem Prozess, auch als Node bennant, eine Id, eine Ip-Adresse (bzw. Namespace) und Port zugeordnet. Wenn dieser Prozess erstellt wird, soll dessen Id über die Datei als Komandozeilenparamter übergeben werden. Zusätzlich soll der Port der Id in Listen-Modus gesetzt werden. Dieser kann wieder über die Kommandozeile mitgeliefert werden. Desweiteren soll der Prozess 3 weitere Nachbarknoten auswähen und merken.  
 exec("programm" int id, int port, struct Node0, struct Node1, struct Node2);
 
 Weiter mit dem Ausführungsverhalten de Processe...

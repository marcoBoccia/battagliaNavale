# battagliaNavale
Abbiamo creato un programma di battaglia navale che si svolge fra un giocatore e il CPU su una matrice di 10x10 celle che prevede per ciascuna parte l'utilizzo della seguente flotte:
N navi = 7
<!-- 
 inserire navi da tot celle con i relativi nomi 'sottomarino ecc'
 
 
 
 
-->

La numerazione delle coordinate delle celle è coerente con il sistema di indicizzazione della matrice stessa:
per cui la cella in alto a sinistra sarà indicata con coordinate A,1 e quella in basso a destra con cordinate J,10.

L'inserimento di coordinate maggiori di 9 o minori di 0 causa instantaneamente l'interruzione della partita.

Il principale problema che si pone, riguarda la collocazione delle navi entro i limiti della matrice 10x10,seguito dalla necessità di non far intersecare fra di loro le navi collocate sulla griglia.

La collocazione delle navi avviene in modo random sia sulla griglia del giocatore che su quella del CPU.

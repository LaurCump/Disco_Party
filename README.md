# Disco_Party
În cadrul acestui proiect trebuie realizat un ring de dans ce va simula iluminarea venită din podea, de la un set de reflectoare și de la un glob disco atașat de tavan. Ringul de dans va fi înconjurat de ziduri și în centru se vor afla câteva forme geometrice ce se vor deplasa aleator și vor reprezenta dansatorii noștri.

## Iluminarea emisă de suprafața ringului de dans
Podeaua ringului de dans va fi formată dintr-un grid de 8×8 celule, unde fiecare celulă va avea o culoare aleasă aleator. Iluminarea emisă de o singură celulă va fi simulată prin utilizarea unei surse de lumină punctiformă ce se află în centrul celulei. Geometria unei celule va fi desenată doar cu componenta emisivă, fără a se lua în calcul celelalte componente ale sursei de lumină utilizate pentru aproximarea iluminării emise de către celulă. Componenta emisivă va avea aceeași culoare ca a sursei de lumină asociate celulei.

Pentru a nu se introduce în calculul de iluminare a zidurilor și a dansatorilor toate sursele de lumină de pe ringul de dans, se vor introduce anumite constrangeri. Se va limita distanța până la care poate ajunge lumina de la o sursă de lumină a unei celule. Dacă presupunem că un dansator este luminat doar de celula în care se află și de celulele vecine, sunt suficiente doar 9 lumini pentru a lumina dansatorul. Zidurile se pot crea modular din mai multe coloane unite pentru a limita numărul de lumini.

Limitarea distanței la care ajunge lumina de la o sursă se realizează cu factorul de atenuare. Astfel, este necesar un factor de atenuare care să permită acest control.

## Calculul de iluminare pentru mai multe surse de lumină
Pentru a putea calcula iluminarea ce provine de la mai multe surse de lumină, trebuie trimise toate aceste surse către shader (poziție, culoare). Iluminarea finală este suma iluminării fiecărei surse de lumină în parte.

## Iluminarea ce provine de la reflectoare
Iluminarea ce provine de la reflectoare se va simula cu surse de lumină de tip spot.

În plus, pentru a se îmbunătăți efectul vizual, se va simula zona de lumină a spot-ului prin desenarea unui con transparent suprapus cu direcția de iluminare a spot-ului.

Pentru a obține conul de lumină, trebuie generată geometria unui con. Pentru simplitate, geometria conului este similară cu cea a unui disc de cerc de rază 1, cu centrul la poziția (0, -1, 0) și creată în planul XOZ. Singura modificare față de discul de cerc este că vertexul din centrul discului se află în originea sistemului de coordonate. Poligoanele bazei nu este necesar sa fie generate. 

Dacă geometria conului este aceeași cu cea descrisă mai sus, conul generat poate fi suprapus cu direcția de iluminare a sursei de lumină de tip spot prin transformările:

* Scalare cu **glm::vec3 (tan(spot_angle), 1, tan(spot_angle)) * inaltime**, unde *spot_angle* este unghiul sursei de lumină de tip spot și inaltime reprezintă înălțimea pe care o va avea conul.
* Transformările de rotație față de OX, OY sau OZ aplicate asupra direcției de iluminare a sursei de lumină de tip spot.
* Translație la poziția sursei de lumină.
Pentru a se aplica transparența, aceste conuri vor fi desenate ultimele.

Pentru a simula atmosfera unui ring de dans, reflectoarele vor avea o mișcare de rotație aleatoare.

## Iluminarea globului disco
Iluminarea globului disco se va simula cu o sursă de lumină punctiformă pentru care culoarea se va obține dintr-o textură.

Această textură va fi generată pe CPU din cod și va conține o culoare aleasă aleator pentru fiecare pixel. Pentru o simulare mai bună a globului disco, se va utiliza o rezoluție de 16×16 pixeli. Proprietățile GL_TEXTURE_MAG_FILTER și GL_TEXTURE_MIN_FILTER vor fi GL_NEAREST.

La poziția globului disco se va desena o sferă pe care va fi aplicată această textură. Coordonatele de textură utilizate pentru eșantionare vor fi calculate din fragment shader. Se consideră că poziția sursei de lumină punctiformă utilizată pentru simularea iluminării globului disco este în centrul acestei sferei. Coordonatele de textură se pot calcula în fragment shader cu o mapare specială cunoscută sub numele de mapare sferică.

Calcularea iluminării sursei de lumină punctiformă pe restul obiectelor din scenă se va realiza similar ca pentru orice altă lumină de acest tip, cu excepția faptului că se va extrage culoarea sursei de lumină din textura generată. Această culoare se va obține în fragment shader cu același tip de mapare descris în codul de mai sus. În această situație, **color** va fi culoarea sursei de lumină pentru poziția **world_position**.

Pentru o atmosferă mai apropiată unui ring de dans, se va simula o rotație continuă a globului disco, realizată prin translația coordonatelor de textură.

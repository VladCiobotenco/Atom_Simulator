# Atom simulator

## O scurta descriere

__Atom simulator__ este o aplicatie educationala destinat elevilor ce doresc sa inteleaga principiile fundamentale ale chimiei. Programul permite utilizatorului sa simuleze molecule, sa vizualizeze legaturile chimice care se stabilesc intre atomi pe baza valentelor acestora si sa descopere informatii despre elemente precum _oxigenul_, _hidrogenul_, _carbonul_ si altele.

## Notiuni de chimie

Versiunea actuala a aplicatiei permite utilizatorului sa vizualizeze o molecule ce contine 6 atomi (reprezentati prin cercuri de culori si dimensiuni diferite):
- Carbon - negru
- Oxigen - rosu
- Hidrogen - alb
- Azot - albastru
- Sulf - galben
- Sodiu (in forma de ion) - roz
- Clor (in forma de ion) - verde

Pentru utilizarea simulatorului utilizatorul trebuie sa cunoasca cateva reguli importante din chimie:
- atomul de hidrogen accepta maxim o legatura
- atomul de oxigen accepta maxim 2 legaturi
- atomul de azot accepta maxim 3 legaturi
- atomul de carbon accepta maxim 4 legaturi 
- atomul de sulf accepta maxim 6 legaturi (functionalitatea atomului de sulf a fost redusa in contextul acestei aplicatii)
- atomul de sodiu accepta maxim o legatura
- atomul de clor accepta maxim o legatura

## Mecanica simulatorului
Aplicatia dezvoltata este foarte intuitiva. Scopul utilizatorului este de a crea molecule cu ajutorul unor atomi prestabiliti si a legaturilor ce se formeaza intre atomi. Modul de functionare al simulatorului este exemplificat mai jos:
1) se selecteaza un tip de atom din meniul de atomi
2) se efectueaza _click_stanga_ in spatiu de lucru pentru a adauga atomi
3) se efectueaza _click_dreapta_ pe atomi pentru a-i selecta
   - pentru a crea o legatura -> _click_dreapta pe alt atom care are electroni disponibili_
   - pentru a sterge o legatura -> _click_dreapta_ pe atomul cu care imparte legatura pe care vrem sa o stergem
   - pentru a sterge atomul selectat -> tasta _delete_
4) pentru a reseta spatiul de lucru -> tasta _R_
5) pentru inchiderea simulatorului -> tasta _ESC_

Odata ce toti atomii din spatiul de lucru sunt __conectati__, programul va afisa numele moleculei formate si masa moleculara. Daca molecula creata este __recunoscuta__ de aplicatie, va afisa si denumirea ei. 

## Alte functionalitati
Pe langa modelarea diverselor molecule, simulatorul permite 2 functionalitati subtile.
- pentru catenele de carbon cu legatura dubla => se poate seta tipul configuratiei geometrice (pentru schimbarea configuratiei se face _click_stanga_ pe legatura dubla)
- pentru moleculele formate numai din legaturi simple => se poate apasa butonul _play_ pentru setarea modului fizic (legaturile simple se rotesc)

## GUI
Simulatorul are un GUI minimalist ce permite oricarui utilizator sa inteleaga cum functioneaza aplicatia. Pasi de urmat (pentru cei lipsiti de arta deducerii):
1) se alege rezolutia dorita (momentan sunt doar 2 rezolutii disponibile, mai mult in viitor)
2) utilizatorul poate alege sa faca orice doreste din meniul simulatorului (indicat ar fi sa apese pe butonul _?_)
3) se activeaza modul __Help Overlay__ pentru explicarea functionalitatilor; se poate apasa pe orice tasta pentru a iesi
4) se activeaza modul __Tutorial__ pentru un scurt tutorial ce include atomii care sunt inclusi in simulator
5) se activeaza modul __Sandbox__ pentru intelegerea functionalitatilor; aici utilizatorul poate interactiona cu atomii; daca o molecula creata corespunde cu o molecula din realitate, simulatorul va preciza acest lucru
6) se activeaza modul __Trivia__ pentru testarea cunostintelor acumulate; aici utilizatorul este trecut prin mai multe molecule cu scopul de a le reproduce dupa nume; fiecare intrebare aduca un punct utilizatorului; la iesire va aparea un input box pentru numele care va apare in leaderboard
7) se activeaza modul __Leaderboard__ pentru vizualizarea Top-10 celor mai bune rezultate inregistrate local

## Resurse
Resursele mentionate mai sus nu imi apartin. Acestea au fost utilizate pentru a crea o aplicatie atractiva din punct de vedere vizual si auditiv. O parte din aceste resurse au fost convertite din formatul original din motive de compatibilitate. Autorii acestor resurse detin dreptul de autor.

- [SFML](https://github.com/SFML/SFML/tree/3.0.2) (Zlib)
- Directorul __assets__ (IMAGES)
  - __periodic-table.jpg__: [sursa](https://npr.brightspotcdn.com/legacy/sites/wuwm/files/201912/AdobeStock_238869971.jpg)
  - __menu-background.jpg__: [sursa](https://img.freepik.com/free-vector/science-logo-banner-with-lab-tools-equipment_1308-157213.jpg?semt=ais_hybrid&w=740&q=80)
- Directorul __assets__ (SOUNDS)
  - __trivia-corect.wav__: [sursa](https://www.myinstants.com/media/sounds/kids-saying-yay-sound-effect_3.mp3)
  - __trivia-gresit.wav__: [sursa](https://www.myinstants.com/media/sounds/wrong-answer_L4HjrqG.mp3)
  - __click-selectare.wav__:
  - __click-stergere.wav__:
  - __TutorialMusic.ogg__: [sursa](https://www.youtube.com/watch?v=jj0ChLVTpaA)
  - __Leaderboard.ogg__: [sursa](https://www.myinstants.com/media/sounds/fnaf-1-music-box.mp3)
  - __MainMusic.ogg__: 
- Directorul __assets__ (FONT)
  - __Roboto-VariableFont_wdth,wght.ttf__: [sursa](https://fonts.google.com/specimen/Roboto)


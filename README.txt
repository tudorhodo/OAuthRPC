Tema 1 SPRC
Student: Hodoboc-Velescu Tudor
Grupa: 342C3

Folosire sleep days: As dori sa folosesc doua din cele trei sleep days.

Insturctiuni de rulare:
1. Fisierele RPC sunt deja generate.
2. Se compileaza cu make
3. Server-ul primeste ca argumente fisierul cu utilizatori, cel de resurse si cel de approvals.
    Pentru timpul de validitate a jetonului de acces, se cauta fisierul readme in directorul curent.
    Daca se gaseste, se citeste continutul, ultimul element citit fiind TTL-ul. Daca nu se gaseste,
    TTL-ul va fi by default 1. Chekcer-ul a fost modificat astfel incat sa copieze pentru fiecare
    test fisierul de readme(a fost adaugat pentru testul 1 un fisier readme cu TTL-ul 1).
4. Clientul primeste ca argumente adresa server-ului si fisierul de intrare reprezentand comenzile
    date de utilizator.
5. Pentru a rula testele, se ruleaza scriptul checker.sh cu argumentul all.

Surse server:
1. tema_rpc_server.cpp
2. tema_xdr.c
3. utils.cpp
4. protocols_server.cpp

Surse client:
1. tema_rpc_client.cpp
2. tema_xdr.c
3. tema_clnt.c

Excluzand fisierele rpc generate, restul surselor vor contine comentarii descriind
functionalitatea acestora. 

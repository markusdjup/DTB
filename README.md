# DTB

DTB (Den Trønderske Bank) er et forsøk på å lage en svært enkel digital bank som kan lagre brukere, bankkontoer og transaksjonshistorie i CSV-filer.

## Kjernen

Den viktigste klassen i kjernen er Ledger. Den eier vektorene users, accounts og transactionLog, og er ansvarlig for alle endringene som skjer på disse. Det vil si at Ledger har funksjoner for oppretting av kontoer og brukere, og utføring av transaksjoner.
Transaksjonene som kan gjøres av Ledger er deposit(), withdraw() og transfer(). transfer() gjøres mellom to bankkontoer, mens deposit() og withdraw() gjøres på én.

BankAccount-klassen er en baseklasse som SavingsAccount og CheckingAccount arver fra.
Den har medlemsvariabene id, owner og balance. All valuta i bankkontoer er lagret som long long for å unngå flyttalsfeil når det gjøres operasjoner på de. BankAccount har Ledger som friend class, og Ledger-objekter kan dermed bruke de private funksjonene setBalance() og setOwner() til å gjøre endringer på balance- og owner-variabelene til BankAccount-objekter.
I tillegg til medlemsvariablene til BankAccount, inneholder SavingsAccount og CheckingAccount henholdsvis interestRate og overdraftLimit. Disse to kontoene er de eneste som brukes i DTB, men det er god mulighet for å senere legge til andre kontotyper ved å arve fra BankAccount.

En annen viktig klasse i kjernen er LedgerSerializer. Denne kan enten lagre innholdet i Ledgers medlemsvariabler til csv-filer eller laste opp csv-filenes innhold til en Ledgers medlemsvariabler. LedgerSerializer::load() kalles i konstruktøren til Ledger og LedgerSerializer::save() kalles i destruktøren. I tillegg kalles save() hver gang Ledger utfører en transaksjon. Csv-filene lagres i en mappe med navn data/ som opprettes av programmet når save() kjøres første gang.

Kjernen inneholder også structene Transaction og User. Alle medlemsvariablene i Transaction er const fordi transaksjoner ikke skal kunne endres på. I tillegg lagres amount-medlemsvariabelen også som en long long for å unngå flyttallsfeil.

## Unntakshåndtering

Prosjektet har en egen mappe med navn exceptions/ som inneholder to filer med unntak relevant for filhåndteringen og for ledgeren. Unntakene i LedgerExceptions.h kastes gjerne når en bruker forsøker å gjøre en ulovlig transaksjon, for eksempel å sette inn en negativ mengde inn på en konto, eller å sende penger til kontoer som ikke eksisterer.
Unntakene i FileExceptions.h kastes hvis LedgerSerializer ikke får tilgang til filene eller hvis det er blitt gjort endringer på filene som gjør at ikke LedgerSerializer klarer å tyde dem.

Spesielt unntakene i LedgerExceptions er også nyttige i GUI-delen. Der har skjermene en statusMessage-variabel som kan settes lik what() om det oppstår et unntak, slik at hva som gikk galt enkelt kan tegnes på skjermen.

## GUI

GUI-delen av prosjektet består av 3 klasser som arver fra AnimationWindow-klassen fra TDT4102.
Den første er LoginScreen som tar inn en ledger og kommuniserer med denne for å registrere eller logge inn en bruker.

DashboardScreen er den viktigste skjermen. Den tar inn en ledger, samt et brukernavn til en registrert og innlogget bruker. Det er her brukere kan få tilgang til kontoene sine og gjøre transaksjoner mellom eller på dem. Ved å fjerne at to->getOwner() må være lik username i DashboardScreen::handleTransfer() kan man veldig enkelt utvide programmet til at brukere også kan sende penger til kontoer de ikke eier selv, altså det som er betaling i nettbanker. Dette er ikke implementert her, tranfer() skjer kun mellom kontoer med samme eiere. 

CreateAccountScreen er skjermen som oppretter kontoer. Den tar inn en ledger og et brukernavn, slik som DashboardScreen. I klassens cpp-fil er bankens overdraftLimit og interestRate hardkodet. Brukere kan altså ikke endre på disse. Ved å jobbe utenfor GUIen, kan man imidlertid endre på medlemsvariablene til spesifike bankkontoer ved å kommunisere direkte med ledgeren som skjermene tar inn (uten GUIen). I framtiden kan man utvide GUIen slik at den kan ha en admin-login/bruker.

GUIen til prosjektet ble dessverre ikke helt ferdig før det ble levert.
Et stort problem er at skjermene henger sammen. Dette gjør at login-skjermen ikke sender brukere til dashboard-skjermen, osv. Altså må man i main manuelt opprette et objekt av hver av skjermene for å havne i den ønskede skjermen. Det gjør at man også med DashboardScreen og CreateAccountScreen må skrive inn brukernavnet til en registrert bruker når man oppretter et objekt.

Utils i gui/ består av funksjonene parseKr() og formatAmount(). parseKr() tar valuta skrevet på GUIen og konverterer det til long long valuta som brukes i klassene. formatAmount() gjør det motsatte.

## Ressurser og verktøy
Claude og ChatGPT har blitt brukt gjennom hele prosjektet. Hovedssaklig har verktøyene blitt brukt til å få hjelp til struktur og planlegging. Det har også blitt mye brukt til feilsøking.
Ingen kode er rent KI-generert og blindt limt inn i prosjektet, men særlig Utils.cpp og LedgerSerializer.cpp er skrevet med mye hjelp fra AI.
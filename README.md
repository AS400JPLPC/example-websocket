# test_websoket
mise a dispo de source lier à libwebsocket

études process

connection ok
echo ok
enrg file pour test ok

multi-user ok

cinematique ok

close global avec message ok

interaction programme serveur et client html ok

compilation -lwebsockets
c+14 iso  ok
"enable all warning" ok 

controle avec la  console web ok

ajouter dans votre répertoir logweb.txt

-----------------------------------------------------------------
Websocket

mon article porte sur la fonction temps réel synchronisé.

Officiellement validé par IBM / Microsoft / Google / Mozilla / Apple et tous les grands acteurs du monde Informatique début janvier 2016.

Notez : que la communication web est asynchrone et que cela ne pouvais pas continuer comme cela.
	 Le mode industriel ne l’accepte pas. (voir la tenu des enregistrement avec commit rollback sur votre base de données par exemple : mettre a jour votre commande et être sur que vous répondez au client en ligne (téléphone) de tel manière que  "what you see is what you get" ehhhhh bien, mauvaise nouvelle pour vous, c’est faux les communication sont asynchrone .

d’où mon article .
Websocket debut 2012… puis les grand acteurs ce penche sérieusement dessus… beaucoup d’essai.
aujourd’hui (pratiquement sans doc car elle est encours, des bibliothèques sont mise à dispo.
https://fr.wikipedia.org/wiki/WebSocket)

le principe : A ce jour 2016/01/01 vérifiez la compatibilité des navigateurs
un client(interne ou externe) ce connecte avec sont navigateurs sur votre site :
vous avez maintenant la possibilité de faire du temps réel en vous connectant avec le processus websocket  (plus de XMLHttpRequest) 1024 connexions par port,
la communication est plus rapide et moins lourde.

Vous géré les utilisateurs depuis votre serveur ( 13ko sans le code métier) une fantastique avancée dans le monde des communications. Les données et le protocole, respect l’architecture HTTP HTTPS, nulle besoin de tout repenser vos applications car elles ce marient très bien avec votre serveur Apache (par exemple)  par contre on entr’aperçois que le modèle d’il y a 40 ans le Proof sur IBM315 ou IBM34 est de nouveau d’actualité car vos applications vous devez les voir  comme un écran = un programme, donc pensée modulaire revenir à ce modèle, articuler votre base de donnée de type AXIAL (c’est un peu comme la théorie des ensembles).
La communication est interactive, le processus prend peu de mémoire , d’un coté comme de l’autre.
La communication ce fait par message (voir xml ou json) , tout la saisie de vos formulaires ou écrans  sont comme sur un AS400 ou PC en direct , ce genre de communication est INTERACTIVE après à vous  de délirer (voir vos applications devis commandes stock fabrication … facturation .. ) 
je me répète tout cela est accessible aujourd’hui et vous devez peut-être repenser l’accès de certaine de vos applications.
J’ai mis sur https://github.com/AS400JPLPC/test_websocket un exemple toutes les fonctions principales sont là pour faire vos premiers pas en c++ mais vous pouvez facilement l’adapter c# ou autre langage . 
Je fais du C++ en web (ou RPG-ILE) pourquoi ;
a) Rapidité , Fiabilité , Sécurité
b) Simplicité 
c) des bibliothèques solides 
d) l’écriture avec la possibilité  de faire du L4G (eh oui de toutes façon vos L4G sont écrit en c++)
e) pas d’interprétation 
si vous écrivez correctement vos projets alors on peux dire allègement de votre maintenance qui devrait tendre vers zéros(sauf évolution).

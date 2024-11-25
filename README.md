# Mini-Compilateur Pascal
> Implémentation d'un compilateur pour un sous-ensemble du langage Pascal en C

## Description
Compilateur basique permettant la traduction de programmes Pascal simples en code assembleur MIPS. Le compilateur effectue l'analyse lexicale, syntaxique et sémantique.

## 🛠 Fonctionnalités
- Analyse lexicale (tokenization)
- Analyse syntaxique (parsing)
- Table des symboles
- Gestion des erreurs basiques

## 📝 Exemple
```pascal
program Example;
var
 x: integer;
begin
 x := 42;
 write(x);
end.
```

## ✨ Fonctionnalités Supportées
- Types: integer, boolean
- Opérations arithmétiques simples
- Instructions if/then
- Boucles while
